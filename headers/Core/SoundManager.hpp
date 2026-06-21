#pragma once

#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include "soloud_echofilter.h"
#include "soloud_freeverbfilter.h"
#include "soloud_wav.h"
#include "Core/Loading/enum.hpp"

class SoundManager final {
	struct QueueData {
		std::string path = "";
		bool changeEnv = true;
	};

	static std::unordered_map<std::string, std::unique_ptr<SoLoud::Wav>> m_sounds;
	static std::unordered_map<std::string, QueueData> m_queue;
	static std::unordered_map<std::string, bool> m_changeEnv;
	static SoLoud::EchoFilter m_echo_filter;
	static SoLoud::FreeverbFilter m_reverb_filter;
	static SoundEnvironment m_env;

	static void AddPlaySound(const std::string& name, const std::string& path, bool changeEnv);
	static void UpdateSoundEnvironment(const std::string& name);
public:
	SoundManager() = delete;
	SoundManager(const SoundManager&) = delete;
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager& operator=(SoundManager&&) = delete;

	static void setEnvironmentID(uint8_t val);
	static void SoundManagerInit();
	static void AddSound(const std::string& name, const std::string& path, bool changeEnv);
	[[nodiscard]] static SoLoud::Wav GetSound(const std::string &name);
	static void PlaySound(const std::string &name);
	static void CleanUp();
	static void SetLoop(const std::string &name, bool loop);
	static void StopSound(const std::string &name);
	static void StopAllSound();
	static void SetEnvironment(SoundEnvironment val);
	[[nodiscard]] static SoundEnvironment GetEnvironment();
};

//m_sound_bus.scheduleStop(handle, 1.f);

#endif