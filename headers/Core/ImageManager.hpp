#pragma once

#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP
#include <queue>

namespace MFCPP {
	class SimpleSprite;
}

struct PreTexture {
	std::filesystem::path path;
	sf::IntRect rect;
	bool isRepeat;
};
class ImageManager {
	static std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_textures;
	static std::unordered_map<std::string, PreTexture> m_pre_textures;
	static std::unordered_map<std::string, MFCPP::SimpleSprite> m_pre_compute_render;
public:
	ImageManager() = delete;
	ImageManager(const ImageManager&) = delete;
	ImageManager(ImageManager&&) = delete;
	ImageManager& operator=(const ImageManager&) = delete;
	ImageManager& operator=(ImageManager&&) = delete;

	static void AddTexture(std::string_view name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect = {}, const bool &isRepeated = false);
	static void PreloadTexture(std::string_view name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect, const bool &isRepeated = false);
	static void LoadTexture(std::string_view name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect = {}, const bool &isRepeated = false);
	static void LoadTexture(std::string_view name_tex);
	static sf::Texture& getTexture(std::string_view name);
	static sf::Texture* getReturnTexture(std::string_view name);
	static bool isExist(std::string_view name_tex);
	static MFCPP::SimpleSprite& getSpritePreCompute(std::string_view name);
	static void Cleanup();
};

#endif