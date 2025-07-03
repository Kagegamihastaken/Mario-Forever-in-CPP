#pragma once

#ifndef LOADING_HPP
#define LOADING_HPP

#include "Core/ExternalHeaders/sfMod/sfMod.hpp"

extern void IOInit();
extern void IODeinit();
extern void LoadTexture(sf::Texture& texture, const std::filesystem::path &path);
extern void LoadLvl(std::string& lvl, const std::filesystem::path &path);
extern std::vector<uint8_t> GetFileDataInByte(const std::filesystem::path &path);
extern void LoadImageFile(sf::Image& image, const std::filesystem::path &path);
#endif