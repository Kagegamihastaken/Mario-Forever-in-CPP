#include "Core/SoundManager.hpp"

#include <ranges>

#include "soloud_lofifilter.h"
#include "Core/AudioEngine.hpp"
#include "Core/Exception.hpp"
#include "Core/Logging.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Loading/Loading.hpp"

std::map<std::string, SoLoud::WavStream> SoundManager::m_sounds;
SoLoud::EchoFilter SoundManager::m_echo_filter;
SoLoud::FreeverbFilter SoundManager::m_reverb_filter;
SoundEnvironment SoundManager::m_env = OVERWORLD;
bool SoundManager::m_played = false;

void SoundManager::SoundManagerInit() {
	m_echo_filter.setParams(0.04f, 0.5f);
	m_reverb_filter.setParams(0, 0.45f, 0.5f, 0.86f);
}

void SoundManager::AddSound(const std::string& name, const std::string& path, const float delay, const float decay) {
	if (m_sounds.contains(name)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("SoundManager: Already exist sound with this name {}", name));
	const std::vector<uint8_t> Data = GetFileDataInByte(path);
	m_sounds[name].loadMem(Data.data(), Data.size(), true);
	//m_sounds[name].setSingleInstance(true);
}
SoLoud::WavStream SoundManager::GetSound(const std::string &name) {
	return m_sounds[name];
}
void SoundManager::SetEnvironment(const SoundEnvironment val) {
	m_env = val;
	for (auto &i : m_sounds) {
		switch (val) {
			case UNDERGROUND:
				i.second.setFilter(0, &m_echo_filter);
				i.second.setFilter(1, &m_reverb_filter);
				break;
			case OVERWORLD:
				i.second.setFilter(0, nullptr);
				i.second.setFilter(1, nullptr);
		}
	}
}
SoundEnvironment SoundManager::GetEnvironment() {
	return m_env;
}
void SoundManager::SetLoop(const std::string &name, const bool loop) {
	if (!m_sounds.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("SoundManager: Cannot find {}", name));
	m_sounds[name].setLooping(loop);
}
void SoundManager::StopSound(const std::string &name) {
	if (!m_sounds.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("SoundManager: Cannot find {}", name));
	m_sounds[name].stop();
}
void SoundManager::StopAllSound() {
	for (auto &i : m_sounds) {
		i.second.stop();
	}
}
void SoundManager::PlaySound(const std::string &name) {
	if (!m_sounds.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("SoundManager: Cannot find {}", name));
	if (audio_engine.getActiveVoiceCount() > 80) return;
	if (audio_engine.getActiveVoiceCount() > 48) m_sounds[name].stop();
	const SoLoud::handle handle = audio_engine.play(m_sounds[name]);
	audio_engine.setInaudibleBehavior(handle,false, false);
	switch (m_env) {
		case UNDERGROUND:
			audio_engine.setFilterParameter(handle, 1, SoLoud::FreeverbFilter::WET, 0.4f);
			audio_engine.setVolume(handle, 0.8f);
			break;
		case OVERWORLD:
			audio_engine.setVolume(handle, 1.6f);
			break;
		default: ;
	}
}
void SoundManager::CleanUp() {
	m_sounds.clear();
}