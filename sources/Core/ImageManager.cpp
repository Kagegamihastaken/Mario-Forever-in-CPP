#include "Core/ImageManager.hpp"

#include <ranges>

#include "Core/Loading/Loading.hpp"
#include "Core/Logging.hpp"
#include "Core/Exception.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Loading/PhysFsStream.hpp"

std::unordered_map<std::string, std::unique_ptr<sf::Texture>> ImageManager::m_textures;
std::unordered_map<std::string, PreTexture> ImageManager::m_pre_textures;
std::unordered_map<std::string, sf::VertexArray> ImageManager::m_vertex_array;

size_t ImageManager::draw_count = 0;
std::unordered_set<std::string> ImageManager::m_vertex_set;
std::queue<std::string> ImageManager::m_vertex_queue;
sf::Texture tempTex;

void InitTempTex() {
	LoadTexture(tempTex, "data/resources/placeholder.png");
}
void ImageManager::Cleanup() {
	MFCPP::Log::InfoPrint("ImageManager cleanup...");
	m_textures.clear();
	m_pre_textures.clear();
	m_vertex_array.clear();
}
void ImageManager::ClearAllVertex() {
	for (auto &val: m_vertex_array | std::views::values)
		if (val.getVertexCount() > 0) val.clear();
}
void ImageManager::AddToVertex(const std::string &name_tex, const sf::IntRect& texRect, const sf::Transform &trans) {

	if (!m_textures.contains(name_tex)) throw MFCPP::Exception::NonExistElement(fmt::format("ImageManager: Texture {} not loaded, aborting drawing...", name_tex));

	if (!m_vertex_set.contains(name_tex)) {
		m_vertex_queue.push(name_tex);
		m_vertex_set.insert(name_tex);
	}
	sf::VertexArray& va = m_vertex_array[name_tex];
	const size_t previousVertexCount = va.getVertexCount();
	va.resize(previousVertexCount + 6);
	const std::unique_ptr<sf::Texture>& tex = m_textures[name_tex];
	const sf::Vector2f position(static_cast<float>(texRect.position.x), static_cast<float>(texRect.position.y));
	const sf::Vector2f size(static_cast<float>(texRect.size.x), static_cast<float>(texRect.size.y));
	sf::Vertex quad[4];
	quad[0].position = trans.transformPoint(sf::Vector2f(0.f, 0.f));
	quad[0].texCoords = position;
	quad[1].position = trans.transformPoint(sf::Vector2f(tex->getSize().x, 0.f));
	quad[1].texCoords = position + sf::Vector2f(size.x, 0.f);
	quad[2].position = trans.transformPoint(sf::Vector2f(0.f, tex->getSize().y));
	quad[2].texCoords = position + sf::Vector2f(0.f, size.y);
	quad[3].position = trans.transformPoint(sf::Vector2f(tex->getSize().x, tex->getSize().y));
	quad[3].texCoords = position + size;

	va[previousVertexCount] = quad[0];
	va[previousVertexCount + 1] = quad[1];
	va[previousVertexCount + 2] = quad[2];
	va[previousVertexCount + 3] = quad[1];
	va[previousVertexCount + 4] = quad[2];
	va[previousVertexCount + 5] = quad[3];
}
void ImageManager::DrawAllVertex() {
	while (!m_vertex_queue.empty()) {
		const std::string& tex_name = m_vertex_queue.front();
		window.draw(m_vertex_array.at(tex_name), m_textures.at(tex_name).get());
		m_vertex_queue.pop();
		++draw_count;
	}
	m_vertex_set.clear();
}

void ImageManager::PreloadTexture(const std::string &name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect, const bool &isRepeated) {
	if (m_pre_textures.contains(name_tex)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("ImageManager: Texture {} already exist.", name_tex));
	m_pre_textures[name_tex] = PreTexture(path, tex_rect, isRepeated);
}
void ImageManager::AddTexture(const std::string &name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect, const bool &isRepeated) {
	if (m_pre_textures.contains(name_tex)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("ImageManager: Texture {} already exist.", name_tex));
	m_pre_textures[name_tex] = PreTexture(path, tex_rect, isRepeated);
	LoadTexture(name_tex, path, tex_rect, isRepeated);
}
bool ImageManager::isExist(const std::string& name_tex) {
	return m_pre_textures.contains(name_tex);
}

void ImageManager::LoadTexture(const std::string &name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect,
                              const bool &isRepeated) {
	if (m_textures.contains(name_tex)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("ImageManager: Texture {} already exist.", name_tex));
	auto tex = std::make_unique<sf::Texture>();
	if (PhysFsStream loadFile(path.string().c_str()); !tex->loadFromStream(loadFile, false, tex_rect)) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", path.string()));
	tex->setRepeated(isRepeated);
	m_textures[name_tex] = std::move(tex);
	m_vertex_array[name_tex] = sf::VertexArray(sf::PrimitiveType::Triangles);
	MFCPP::Log::InfoPrint(fmt::format("ImageManager: Loaded Texture {}", name_tex));
}
void ImageManager::LoadTexture(const std::string& name_tex) {
	if (!m_pre_textures.contains(name_tex)) throw MFCPP::Exception::NonExistElement(fmt::format("ImageManager: Texture {} not preloaded.", name_tex));
	if (m_textures.contains(name_tex)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("ImageManager: Texture {} already exist.", name_tex));
	auto tex = std::make_unique<sf::Texture>();
	if (PhysFsStream loadFile(m_pre_textures[name_tex].path.string().c_str()); !tex->loadFromStream(loadFile, false, m_pre_textures[name_tex].rect)) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", m_pre_textures[name_tex].path.string()));
	tex->setRepeated(m_pre_textures[name_tex].isRepeat);
	m_textures[name_tex] = std::move(tex);
	m_vertex_array[name_tex] = sf::VertexArray(sf::PrimitiveType::Triangles);
	MFCPP::Log::InfoPrint(fmt::format("ImageManager: Loaded Texture {}", name_tex));
}
sf::Texture& ImageManager::GetTexture(const std::string& name)
{
	if (!m_textures.contains(name))
		//LoadTexture(name, m_pre_textures[name].path, m_pre_textures[name].rect, m_pre_textures[name].isRepeat);
		throw MFCPP::Exception::NonExistElement(fmt::format("ImageManager: cannot find {}", name));
	return *m_textures[name];
}
sf::Texture* ImageManager::GetReturnTexture(const std::string& name)
{
	if (!m_textures.contains(name))
		LoadTexture(name, m_pre_textures[name].path, m_pre_textures[name].rect, m_pre_textures[name].isRepeat);
		//throw MFCPP::Exception::NonExistElement(fmt::format("ImageManager: cannot find {}", name));
	return m_textures[name].get();
}
void ImageManager::printDrawCount() {
	MFCPP::Log::InfoPrint(fmt::format("Drawed {} times.", draw_count));
	draw_count = 0;
}
