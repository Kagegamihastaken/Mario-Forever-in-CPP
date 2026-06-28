#pragma once

#ifndef MUSIC_MANAGER_HPP
#define MUSIC_MANAGER_HPP

namespace MusicManager {
	void ForceLoadMusic(std::string_view name);
	void AddMusic(std::string_view name, const std::filesystem::path &path);
	void AddPlayMusic(std::string_view name, const std::filesystem::path &path);
	void PlayMusic(std::string_view name);
	void PauseMusic(std::string_view name);
	void CleanUp();
	void SetLoop(std::string_view name, bool loop);
	void StopMusic(std::string_view name);
	void StopAllMusic();
	void SetMusicVolume(std::string_view name, float volume);

	bool IsMusicPlaying(std::string_view name);
	bool IsMusicStopped(std::string_view name);
	bool IsMusicPaused(std::string_view name);
};

#endif