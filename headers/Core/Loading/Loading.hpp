#pragma once

#ifndef LOADING_HPP
#define LOADING_HPP

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include "../ExternalHeaders/sfMod/sfMod.hpp"

extern void IOInit();
extern void IODeinit();
extern void LoadTexture(sf::Texture& texture, const std::string &path);
extern void LoadAudio(sf::SoundBuffer& soundBuffer, const std::string &path);
extern void LoadLvl(std::string& lvl, const std::string &path);
extern void LoadMOD(sfmod::Mod& music, const std::string &path);
extern void LoadOGG(sf::Music& music, const std::string &path);
extern int ReadStrLine(const std::string& lvldata, std::string& out, int resume);
extern void LoadImageFile(sf::Image& image, const std::string &path);
#endif