#pragma once

#ifndef MUSIC_MANAGER_HPP
#define MUSIC_MANAGER_HPP

#include "soloud_openmpt.h"
#include "soloud_wav.h"

struct PreMusic {
	std::filesystem::path path;
	bool isLoop = false;
	float volume = 1.f;
};

class MusicManager {
private:
	static std::map<std::string, std::unique_ptr<SoLoud::AudioSource>> m_musics;
	static std::map<std::string, SoLoud::handle> m_musics_handle;
	static std::map<std::string, PreMusic> m_pre_musics;

	static bool CheckValidWavType(const std::string_view& str);
public:
	MusicManager() = delete;
	static void ForceLoadMusic(const std::string& name);
	static void AddMusic(const std::string &name, const std::filesystem::path &path);
	static void AddPlayMusic(const std::string &name, const std::filesystem::path &path);
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