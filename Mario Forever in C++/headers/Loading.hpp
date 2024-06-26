#pragma once

#ifndef LOADING_HPP
#define LOADING_HPP

#include <SFML/Audio/SoundBuffer.hpp>
extern void LoadTexture(sf::Texture& texture, int resID, int resType);
extern void LoadAudio(sf::SoundBuffer& soundBuffer, int resID, int resType);
extern void LoadLvl(std::string& lvl, int resID, int resType);
extern int ReadStrLine(std::string& lvldata, std::string& out, int resume);
#endif