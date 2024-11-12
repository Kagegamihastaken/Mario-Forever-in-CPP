#include "../headers/Core/TextureManager.hpp"

std::map<std::string, sf::Texture*> TextureManager::m_textures;
std::map<std::string, std::vector<sf::Texture*>> TextureManager::m_animated_textures;

void TextureManager::AddTexture(std::string name, sf::Texture* texture) {
	m_textures[name] = texture;
}
void TextureManager::AddAnimatedTexture(std::string name, std::vector<sf::Texture*> textures) {
	m_animated_textures[name] = textures;
}
sf::Texture* TextureManager::GetTexture(std::string name) {
	return m_textures[name];
}
std::vector<sf::Texture*> TextureManager::GetAnimatedTexture(std::string name) {
	return m_animated_textures[name];
}