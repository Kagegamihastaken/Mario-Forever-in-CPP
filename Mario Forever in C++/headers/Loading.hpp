#pragma once

#ifndef LOADING_HPP
#define LOADING_HPP

#include <SFML/Audio/SoundBuffer.hpp>
#include "sfMod.hpp"
extern void LoadTexture(sf::Texture& texture, int resID);
extern void LoadAudio(sf::SoundBuffer& soundBuffer, int resID);
extern void LoadLvl(std::string& lvl, int resID);
extern void LoadMOD(sfmod::Mod& music, int resID, int channel, int samplerate);
extern void LoadOGG(sf::SoundBuffer& music, int resID);
extern int ReadStrLine(std::string& lvldata, std::string& out, int resume);
#endif