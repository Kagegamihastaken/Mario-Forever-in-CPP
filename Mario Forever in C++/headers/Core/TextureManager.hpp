#pragma once

#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>

class TextureManager {
private:
	static std::map<std::string, sf::Texture*> m_textures;
	static std::map<std::string, std::vector<sf::Texture*>> m_animated_textures;
public:
	static void AddTexture(std::string name, sf::Texture* texture);
	static sf::Texture* GetTexture(std::string name);
	static void AddAnimatedTexture(std::string name, std::vector<sf::Texture*> textures);
	static std::vector<sf::Texture*> GetAnimatedTexture(std::string name);
	static void LoadingAnimatedTexture(int ID, std::string name, int start, int end, int y, int sizex, int sizey);
	static void Loadingtexture(int ID, std::string name, int x, int y, int sizex, int sizey);
};

#endif