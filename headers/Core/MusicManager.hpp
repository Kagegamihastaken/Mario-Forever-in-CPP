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
	static void AddOGGMusic(const std::string &name, const std::string &path);
	static void PlayOGGMusic(const std::string &name);
	static void PauseOGGMusic(const std::string &name);
	static void ClearUp();
	static void SetOGGLoop(const std::string &name, bool loop);
	static void StopOGGMusic(const std::string &name);
	static void StopAllOGGMusic();

	static void AddMODMusic(const std::string &name, const std::string &path);
	static void StopMODMusic(const std::string &name);
	static void SetMODLoop(const std::string &name, bool loop);
	static void PlayMODMusic(const std::string &name);
	static void PauseMODMusic(const std::string &name);
	static void StopAllMODMusic();
	static bool IsMODMusicPlaying(const std::string &name);
	static bool IsOGGMusicPlaying(const std::string &name);
	static bool IsMODMusicStopped(const std::string &name);
	static bool IsOGGMusicStopped(const std::string &name);
	static bool IsMODMusicPaused(const std::string &name);
	static bool IsOGGMusicPaused(const std::string &name);
};

#endif