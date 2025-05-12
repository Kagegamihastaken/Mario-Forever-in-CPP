#pragma once

#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
extern sf::Texture tempTex;
extern void InitTempTex();
class TextureManager {
private:
	static std::map<std::string, sf::Texture*> m_textures;
	static std::map<std::string, std::vector<sf::Texture*>> m_animated_textures;
public:
	static void AddTexture(const std::string &name, sf::Texture* texture);
	static sf::Texture* GetTexture(const std::string &name);
	static void AddAnimatedTexture(const std::string &name, const std::vector<sf::Texture*> &textures);
	static std::vector<sf::Texture*> GetAnimatedTexture(const std::string &name);
	static void LoadingAnimatedTexture(const std::string &path, const std::string &name, int start, int end, int y, int sizex, int sizey);
	static void Loadingtexture(const std::string &path, const std::string &name, int x, int y, int sizex, int sizey, bool isRepeated = false);
};

#endif