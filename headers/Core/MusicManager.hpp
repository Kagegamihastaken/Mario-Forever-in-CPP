#pragma once

#ifndef MUSIC_MANAGER_HPP
#define MUSIC_MANAGER_HPP
#include "Enumeration/MusicEnum.hpp"

namespace MusicManager {
	void ForceLoadMusic(MusicID name);
	void AddMusic(MusicID name, const std::filesystem::path &path);
	void AddPlayMusic(MusicID name, const std::filesystem::path &path);
	void PlayMusic(MusicID name);
	void PauseMusic(MusicID name);
	void CleanUp();
	void SetLoop(MusicID name, bool loop);
	void StopMusic(MusicID name);
	void StopAllMusic();
	void SetMusicVolume(MusicID name, float volume);

	bool IsMusicPlaying(MusicID name);
	bool IsMusicStopped(MusicID name);
	bool IsMusicPaused(MusicID name);
};

#endif