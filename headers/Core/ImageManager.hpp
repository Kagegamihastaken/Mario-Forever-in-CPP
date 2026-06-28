#pragma once

#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP
#include <boost/unordered/unordered_flat_map.hpp>
#include "SimpleSprite.hpp"

namespace ImageManager {
	void AddTexture(std::string_view name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect = {}, const bool &isRepeated = false);
	void PreloadTexture(std::string_view name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect, const bool &isRepeated = false);
	void LoadTexture(std::string_view name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect = {}, const bool &isRepeated = false);
	void LoadTexture(std::string_view name_tex);
	sf::Texture& getTexture(std::string_view name);
	sf::Texture* getReturnTexture(std::string_view name);
	bool isExist(std::string_view name_tex);
	MFCPP::SimpleSprite* getSpritePreCompute(std::string_view name);
	void Cleanup();
};

#endif