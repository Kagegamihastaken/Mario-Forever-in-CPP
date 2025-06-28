#pragma once

#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

extern sf::Texture tempTex;
extern void InitTempTex();
class ImageManager {
private:
	static std::map<std::string, sf::Image> m_images;
	static std::map<std::string, sf::Texture*> m_textures;
public:
	static void AddImage(const std::string &name, const std::string& path);
	static sf::Image GetImage(const std::string &name);
	static void ConvertToTexture(sf::Texture* tex, const std::string& name, const sf::IntRect& rect, const bool& isFlipped);
	static void ConvertToTexture(sf::Texture* tex, const std::string& name, const bool& isFlipped);
	static void AddTexture(const std::string &img_name, const sf::IntRect& tex_rect, const std::string& name_tex, const bool& isRepeated = false, const bool& isFlipped = false);
	static void AddTexture(const std::string &img_name, const std::string& name_tex, const bool& isRepeated = false, const bool& isFlipped = false);
	static sf::Texture& GetTexture(const std::string &name);
	static sf::Texture* GetReturnTexture(const std::string& name);
	static void Cleanup();
	static void CreateTestImage(const std::string& name, const std::string& new_name);
};

#endif