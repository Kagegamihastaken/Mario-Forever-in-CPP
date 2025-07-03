#pragma once

#ifndef MUSIC_MANAGER_HPP
#define MUSIC_MANAGER_HPP

#include "soloud_openmpt.h"
#include "soloud_wav.h"
#include "Core/ExternalHeaders/sfMod/sfMod.hpp"

class MusicManager {
private:
	static std::map<std::string, SoLoud::Wav> m_ogg_musics;
	static std::map<std::string, SoLoud::Openmpt> m_mod_musics;
	static std::map<std::string, bool> m_musics_id;
	static std::map<std::string, SoLoud::handle> m_musics_handle;

	static bool CheckValidWavType(const std::string_view& str);
public:
	MusicManager() = delete;
	static void AddMusic(const std::string &name, const std::filesystem::path &pat);
	static void PlayMusic(const std::string &name);
	static void PauseMusic(const std::string &name);
	static void CleanUp();
	static void SetLoop(const std::string &name, bool loop);
	static void StopMusic(const std::string &name);
	static void StopAllMusic();
	static void SetMusicVolume(const std::string &name, float volume);

	static bool IsMusicPlaying(const std::string &name);
	static bool IsMusicStopped(const std::string &name);
	static bool IsMusicPaused(const std::string &name);
};

#endif