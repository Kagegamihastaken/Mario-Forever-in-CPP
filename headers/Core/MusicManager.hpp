#pragma once

#ifndef MUSIC_MANAGER_HPP
#define MUSIC_MANAGER_HPP

#include <map>
#include <string>
#include <SFML/Audio.hpp>
#include "ExternalHeaders/sfMod/sfMod.hpp"
#include <vector>
#include <modplug.h>

class MusicManager {
private:
	static std::map<std::string, sf::Music> m_ogg_musics;
	static std::map<std::string, sfmod::Mod> m_mod_musics;
public:
	static void AddOGGMusic(std::string name, std::string path);
	static void PlayOGGMusic(std::string name);
	static void PauseOGGMusic(std::string name);
	static void ClearUp();
	static void SetOGGLoop(std::string name, bool loop);
	static void StopOGGMusic(std::string name);
	static void StopAllOGGMusic();

	static void AddMODMusic(std::string name, std::string path);
	static void StopMODMusic(std::string name);
	static void SetMODLoop(std::string name, bool loop);
	static void PlayMODMusic(std::string name);
	static void PauseMODMusic(std::string name);
	static void StopAllMODMusic();
	static bool IsMODMusicPlaying(std::string name);
	static bool IsOGGMusicPlaying(std::string name);
	static bool IsMODMusicStopped(std::string name);
	static bool IsOGGMusicStopped(std::string name);
	static bool IsMODMusicPaused(std::string name);
	static bool IsOGGMusicPaused(std::string name);
};

#endif