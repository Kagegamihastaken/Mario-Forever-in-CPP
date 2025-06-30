#pragma once

#ifndef LOADING_HPP
#define LOADING_HPP

#include "Core/ExternalHeaders/sfMod/sfMod.hpp"

extern void IOInit();
extern void IODeinit();
extern void LoadTexture(sf::Texture& texture, const std::string &path);
extern void LoadAudio(sf::SoundBuffer& soundBuffer, const std::string &path);
extern void LoadLvl(std::string& lvl, const std::string &path);
extern std::vector<uint8_t> GetFileDataInByte(const std::string &path);
extern void LoadImageFile(sf::Image& image, const std::string &path);
#endif