#include "Core/ImageManager.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/Logging.hpp"
#include "Core/Exception.hpp"

#include "cmake-build-relwithdebinfo-vs/_deps/sfml-src/extlibs/headers/glad/include/glad/gl.h"
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
void ImageManager::AddTexture(const std::string &name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect, const bool &isRepeated) {
	if (m_pre_textures.contains(name_tex)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("ImageManager: Texture {} already exist.", name_tex));
	m_pre_textures[name_tex] = PreTexture(path, tex_rect, isRepeated);
}

void ImageManager::LoadTexture(const std::string &name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect,
                              const bool &isRepeated) {
	if (m_textures.contains(name_tex)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("ImageManager: Texture {} already exist.", name_tex));
	auto tex = new sf::Texture();
	if (PhysFsStream loadFile(path.string().c_str()); !tex->loadFromStream(loadFile, false, tex_rect)) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", path.string()));
	tex->setRepeated(isRepeated);
	m_textures[name_tex] = std::move(tex);

	const GLint texID = m_textures[name_tex]->getNativeHandle();

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindTexture(GL_TEXTURE_2D, 0);

	MFCPP::Log::InfoPrint(fmt::format("ImageManager: Loaded Texture {}", name_tex));
}
sf::Texture& ImageManager::GetTexture(const std::string& name)
{
	if (!m_textures.contains(name))
		LoadTexture(name, m_pre_textures[name].path, m_pre_textures[name].rect, m_pre_textures[name].isRepeat);
		//throw MFCPP::Exception::NonExistElement(fmt::format("ImageManager: cannot find {}", name));
	return *m_textures[name];
}
sf::Texture* ImageManager::GetReturnTexture(const std::string& name)
{
	if (!m_textures.contains(name))
		LoadTexture(name, m_pre_textures[name].path, m_pre_textures[name].rect, m_pre_textures[name].isRepeat);
		//throw MFCPP::Exception::NonExistElement(fmt::format("ImageManager: cannot find {}", name));
	return m_textures[name];
}