#pragma once

#ifndef MUSIC_MANAGER_HPP
#define MUSIC_MANAGER_HPP

namespace MusicManager {
	static bool CheckValidWavType(std::string_view str);

	void ForceLoadMusic(const std::string& name);
	void AddMusic(const std::string &name, const std::filesystem::path &path);
	void AddPlayMusic(const std::string &name, const std::filesystem::path &path);
	void PlayMusic(const std::string &name);
	void PauseMusic(const std::string &name);
	void CleanUp();
	void SetLoop(const std::string &name, bool loop);
	void StopMusic(const std::string &name);
	void StopAllMusic();
	void SetMusicVolume(const std::string &name, float volume);

	bool IsMusicPlaying(const std::string &name);
	bool IsMusicStopped(const std::string &name);
	bool IsMusicPaused(const std::string &name);
};

#endif