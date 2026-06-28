#pragma once

#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include "soloud_wav.h"
#include "Core/Loading/enum.hpp"

namespace SoundManager {
	void setEnvironmentID(uint8_t val);
	void SoundManagerInit();
	void AddSound(std::string_view name, const std::filesystem::path& path, bool changeEnv);
	[[nodiscard]] SoLoud::Wav GetSound(std::string_view name);
	void PlaySound(std::string_view name);
	void CleanUp();
	void SetLoop(std::string_view name, bool loop);
	void StopSound(std::string_view name);
	void StopAllSound();
	void SetEnvironment(SoundEnvironment val);
	[[nodiscard]] SoundEnvironment GetEnvironment();
};

//m_sound_bus.scheduleStop(handle, 1.f);

#endif