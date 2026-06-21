#include "Core/ImageManager.hpp"

#include <ranges>

#include "Core/Loading/Loading.hpp"
#include "Core/Logging.hpp"
#include "Core/Exception.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Loading/PhysFsStream.hpp"

std::unordered_map<std::string, std::unique_ptr<sf::Texture>> ImageManager::m_textures;
std::unordered_map<std::string, PreTexture> ImageManager::m_pre_textures;
std::unordered_map<std::string, MFCPP::SimpleSprite> ImageManager::m_pre_compute_render;

void ImageManager::Cleanup() {
	MFCPP::Log::InfoPrint("ImageManager cleanup...");
	m_textures.clear();
	m_pre_textures.clear();
	m_pre_compute_render.clear();
}

void ImageManager::PreloadTexture(std::string_view name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect, const bool &isRepeated) {
	if (m_pre_textures.contains(name_tex.data())) throw MFCPP::Exception::AlreadyExistElement(fmt::format("ImageManager: Texture {} already exist.", name_tex));
	m_pre_textures[name_tex.data()] = PreTexture(path, tex_rect, isRepeated);
}
void ImageManager::AddTexture(std::string_view name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect, const bool &isRepeated) {
	if (m_pre_textures.contains(name_tex.data())) throw MFCPP::Exception::AlreadyExistElement(fmt::format("ImageManager: Texture {} already exist.", name_tex));
	m_pre_textures[name_tex.data()] = PreTexture(path, tex_rect, isRepeated);
	LoadTexture(name_tex, path, tex_rect, isRepeated);
}
bool ImageManager::isExist(std::string_view name_tex) {
	return m_pre_textures.contains(name_tex.data());
}

void ImageManager::LoadTexture(std::string_view name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect,
                              const bool &isRepeated) {
	if (m_textures.contains(name_tex.data())) throw MFCPP::Exception::AlreadyExistElement(fmt::format("ImageManager: Texture {} already exist.", name_tex));
	auto tex = std::make_unique<sf::Texture>();
	if (PhysFsStream loadFile(path.string().c_str()); !tex->loadFromStream(loadFile, false, tex_rect)) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", path.string()));
	tex->setRepeated(isRepeated);
	m_textures[name_tex.data()] = std::move(tex);
	m_pre_compute_render[name_tex.data()] = MFCPP::SimpleSprite(m_textures[name_tex.data()].get());
	MFCPP::Log::InfoPrint(fmt::format("ImageManager: Loaded Texture {}", name_tex));
}
void ImageManager::LoadTexture(std::string_view name_tex) {
	if (!m_pre_textures.contains(name_tex.data())) throw MFCPP::Exception::NonExistElement(fmt::format("ImageManager: Texture {} not preloaded.", name_tex));
	if (m_textures.contains(name_tex.data())) throw MFCPP::Exception::AlreadyExistElement(fmt::format("ImageManager: Texture {} already exist.", name_tex));
	auto tex = std::make_unique<sf::Texture>();
	if (PhysFsStream loadFile(m_pre_textures[name_tex.data()].path.string().c_str()); !tex->loadFromStream(loadFile, false, m_pre_textures[name_tex.data()].rect)) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", m_pre_textures[name_tex.data()].path.string()));
	tex->setRepeated(m_pre_textures[name_tex.data()].isRepeat);
	m_textures[name_tex.data()] = std::move(tex);
	m_pre_compute_render[name_tex.data()] = MFCPP::SimpleSprite(m_textures[name_tex.data()].get());
	MFCPP::Log::InfoPrint(fmt::format("ImageManager: Loaded Texture {}", name_tex));
}
sf::Texture& ImageManager::getTexture(std::string_view name)
{
	if (!m_textures.contains(name.data()))
		//LoadTexture(name, m_pre_textures[name].path, m_pre_textures[name].rect, m_pre_textures[name].isRepeat);
		throw MFCPP::Exception::NonExistElement(fmt::format("ImageManager: cannot find {}", name));
	return *m_textures[name.data()];
}
sf::Texture* ImageManager::getReturnTexture(std::string_view name)
{
	if (!m_textures.contains(name.data()))
		LoadTexture(name, m_pre_textures[name.data()].path, m_pre_textures[name.data()].rect, m_pre_textures[name.data()].isRepeat);
		//throw MFCPP::Exception::NonExistElement(fmt::format("ImageManager: cannot find {}", name));
	return m_textures[name.data()].get();
}

MFCPP::SimpleSprite &ImageManager::getSpritePreCompute(std::string_view name) {
	return m_pre_compute_render[name.data()];
}
