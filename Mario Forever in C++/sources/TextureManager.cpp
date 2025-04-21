#include <iostream>

#include "../headers/Core/TextureManager.hpp"
#include "../headers/Core/Loading/Loading.hpp"

std::map<std::string, sf::Texture*> TextureManager::m_textures;
std::map<std::string, std::vector<sf::Texture*>> TextureManager::m_animated_textures;

sf::Texture tempTex;
void TextureManager::AddTexture(std::string name, sf::Texture* texture) {
	m_textures[name] = texture;
}
void TextureManager::AddAnimatedTexture(std::string name, std::vector<sf::Texture*> textures) {
	m_animated_textures[name] = textures;
	//m_animated_textures.at(name).assign(textures.begin(), textures.end());
}
sf::Texture* TextureManager::GetTexture(std::string name) {
	return m_textures[name];
}
std::vector<sf::Texture*> TextureManager::GetAnimatedTexture(std::string name) {
	return m_animated_textures[name];
}
void TextureManager::LoadingAnimatedTexture(int ID, std::string name, int start, int end, int y, int sizex, int sizey) {
	sf::Texture* texture = new sf::Texture();
	LoadTexture(*texture, ID);
	sf::Texture* tex = new sf::Texture();
	std::vector<sf::Texture*>* ani = new std::vector<sf::Texture*>();
	for (int i = start; i <= end; ++i) {
		tex->loadFromImage(texture->copyToImage(), false, sf::IntRect({ i * sizex, y * sizey }, { sizex, sizey }));
		ani->push_back(tex);
		tex = new sf::Texture();
	}
	AddAnimatedTexture(name, *ani);
	delete tex;
	delete ani;
}
void TextureManager::Loadingtexture(int ID, std::string name, int x, int y, int sizex, int sizey, bool isRepeated) {
	sf::Texture* texture = new sf::Texture();
	sf::Texture* tex = new sf::Texture();
	LoadTexture(*texture, ID);
	tex->loadFromImage(texture->copyToImage(), false, sf::IntRect({ x, y }, { sizex, sizey }));
	tex->setRepeated(isRepeated);
	AddTexture(name, tex);
	tex = new sf::Texture();
	delete tex;
	delete texture;
}