#include "Core/ImageManager.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/Logging.hpp"
#include "Core/Exception.hpp"
#include "Core/Loading/PhysFsStream.hpp"

std::map<std::string, sf::Texture*> ImageManager::m_textures;
std::map<std::string, PreTexture> ImageManager::m_pre_textures;

sf::Texture tempTex;
void InitTempTex() {
	LoadTexture(tempTex, "data/resources/placeholder.png");
}
void ImageManager::Cleanup() {
	MFCPP::Log::InfoPrint("ImageManager cleanup...");
	m_textures.clear();
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
	auto tex = new sf::Texture();
	if (PhysFsStream loadFile(path.string().c_str()); !tex->loadFromStream(loadFile, false, tex_rect)) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", path.string()));
	tex->setRepeated(isRepeated);
	m_textures[name_tex] = std::move(tex);
	MFCPP::Log::InfoPrint(fmt::format("ImageManager: Loaded Texture {}", name_tex));
}
void ImageManager::LoadTexture(const std::string& name_tex) {
	if (!m_pre_textures.contains(name_tex)) throw MFCPP::Exception::NonExistElement(fmt::format("ImageManager: Texture {} not preloaded.", name_tex));
	if (m_textures.contains(name_tex)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("ImageManager: Texture {} already exist.", name_tex));
	auto tex = new sf::Texture();
	if (PhysFsStream loadFile(m_pre_textures[name_tex].path.string().c_str()); !tex->loadFromStream(loadFile, false, m_pre_textures[name_tex].rect)) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", m_pre_textures[name_tex].path.string()));
	tex->setRepeated(m_pre_textures[name_tex].isRepeat);
	m_textures[name_tex] = std::move(tex);
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
	return m_textures[name];
}