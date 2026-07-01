#pragma once

#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include "soloud_wav.h"
#include "Enumeration/enum.hpp"
#include "Enumeration/SoundEnum.hpp"

namespace SoundManager {
	void setEnvironmentID(uint8_t val);
	void SoundManagerInit();
	void AddSound(SoundID name, const std::filesystem::path& path, bool changeEnv);
	[[nodiscard]] SoLoud::Wav GetSound(SoundID name);
	void PlaySound(SoundID name);
	void CleanUp();
	void SetLoop(SoundID name, bool loop);
	void StopSound(SoundID name);
	void StopAllSound();
	void SetEnvironment(SoundEnvironment val);
	[[nodiscard]] SoundEnvironment GetEnvironment();
};

//m_sound_bus.scheduleStop(handle, 1.f);

#endif