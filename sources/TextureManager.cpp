#include <iostream>

#include "../headers/Core/TextureManager.hpp"
#include "../headers/Core/Loading/Loading.hpp"

std::map<std::string, sf::Texture*> TextureManager::m_textures;
std::map<std::string, std::vector<sf::Texture*>> TextureManager::m_animated_textures;

sf::Texture tempTex;
void InitTempTex() {
	LoadTexture(tempTex, "data/resources/placeholder.png");
}
void TextureManager::AddTexture(const std::string &name, sf::Texture* texture) {
	m_textures[name] = texture;
}
void TextureManager::AddAnimatedTexture(const std::string &name, const std::vector<sf::Texture*> &textures) {
	m_animated_textures[name] = textures;
	//m_animated_textures.at(name).assign(textures.begin(), textures.end());
}
sf::Texture* TextureManager::GetTexture(const std::string &name) {
	return m_textures[name];
}
std::vector<sf::Texture*> TextureManager::GetAnimatedTexture(const std::string &name) {
	return m_animated_textures[name];
}
void TextureManager::LoadingAnimatedTexture(const std::string &path, const std::string &name, const int start, const int end, const int y, int sizex, int sizey) {
	const auto texture = new sf::Texture();
	LoadTexture(*texture, path);
	auto tex = new sf::Texture();
	const auto ani = new std::vector<sf::Texture*>();
	for (int i = start; i <= end; ++i) {
		tex->loadFromImage(texture->copyToImage(), false, sf::IntRect({ i * sizex, y * sizey }, { sizex, sizey }));
		ani->push_back(tex);
		tex = new sf::Texture();
	}
	AddAnimatedTexture(name, *ani);
	delete tex;
	delete ani;
}
void TextureManager::Loadingtexture(const std::string &path, const std::string &name, int x, int y, int sizex, int sizey, const bool isRepeated) {
	const auto texture = new sf::Texture();
	auto tex = new sf::Texture();
	LoadTexture(*texture, path);
	tex->loadFromImage(texture->copyToImage(), false, sf::IntRect({ x, y }, { sizex, sizey }));
	tex->setRepeated(isRepeated);
	AddTexture(name, tex);
	tex = new sf::Texture();
	delete tex;
	delete texture;
}