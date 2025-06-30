#pragma once

#ifndef MUSIC_MANAGER_HPP
#define MUSIC_MANAGER_HPP

#include "Core/ExternalHeaders/sfMod/sfMod.hpp"
#include "Core/Exception.hpp"

class MusicManager {
private:
	static std::map<std::string, std::vector<uint8_t>> m_ogg_data;
	static std::map<std::string, sf::Music> m_ogg_musics;
	static std::map<std::string, sfmod::Mod> m_mod_musics;
	static std::map<std::string, bool> m_musics_id;
public:
	MusicManager() = delete;
	static void AddMusic(const std::string &name, const std::string &path);
	static void PlayMusic(const std::string &name);
	static void PauseMusic(const std::string &name);
	static void CleanUp();
	static void SetLoop(const std::string &name, bool loop);
	static void StopMusic(const std::string &name);
	static void StopAllMusic();
	static void SetMusicVolume(const std::string &name, unsigned volume);

	static bool IsMusicPlaying(const std::string &name);
	static bool IsMusicStopped(const std::string &name);
	static bool IsMusicPaused(const std::string &name);
};

#endif