#pragma once

#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

extern sf::Texture tempTex;
extern void InitTempTex();
struct PreTexture {
	std::filesystem::path path;
	sf::IntRect rect;
	bool isRepeat;
};
class ImageManager {
private:
	static std::map<std::string, sf::Texture*> m_textures;
	static std::map<std::string, PreTexture> m_pre_textures;
public:
	ImageManager() = delete;
	static void AddTexture(const std::string &name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect = {}, const bool &isRepeated = false);
	static void LoadTexture(const std::string &name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect = {}, const bool &isRepeated = false);
	static sf::Texture& GetTexture(const std::string &name);
	static sf::Texture* GetReturnTexture(const std::string& name);
	static bool isExist(const std::string& name_tex);
	static void Cleanup();
};

#endif