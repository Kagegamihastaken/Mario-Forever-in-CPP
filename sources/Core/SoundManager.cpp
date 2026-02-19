#include "Core/SoundManager.hpp"

#include <ranges>

#include "Core/AudioEngine.hpp"
#include "Core/Exception.hpp"
#include "Core/Logging.hpp"
#include "Core/Loading/PhysFsStream.hpp"

std::map<std::string, std::unique_ptr<SoLoud::Wav>> SoundManager::m_sounds;
std::map<std::string, std::string> SoundManager::m_queue;
SoLoud::EchoFilter SoundManager::m_echo_filter;
SoLoud::FreeverbFilter SoundManager::m_reverb_filter;
SoundEnvironment SoundManager::m_env = OVERWORLD;

void SoundManager::UpdateSoundEnvironment(const std::string& name) {
	switch (m_env) {
		case UNDERGROUND:
			m_sounds[name]->setFilter(0, &m_echo_filter);
			m_sounds[name]->setFilter(1, &m_reverb_filter);
			break;
		case OVERWORLD:
			m_sounds[name]->setFilter(0, nullptr);
			m_sounds[name]->setFilter(1, nullptr);
			break;
		default: ;
	}
}

void SoundManager::SoundManagerInit() {
	m_echo_filter.setParams(0.04f, 0.5f);
	m_reverb_filter.setParams(0, 0.45f, 0.5f, 0.86f);
}
void SoundManager::AddSound(const std::string& name, const std::string& path) {
	if (m_queue.contains(name)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("SoundManager: Already exist sound with this name {}", name));
	m_queue[name] = path;
}
void SoundManager::AddPlaySound(const std::string& name, const std::string& path, const float delay, const float decay) {
	if (m_sounds.contains(name)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("SoundManager: Already exist sound with this name {}", name));
	auto wav = std::make_unique<SoLoud::Wav>();
	PhysFsSoLoudStream file;
	if (!file.open(path.c_str())) throw std::runtime_error(fmt::format("PhysFS Error: Cannot open {}", path));
	if (wav->loadFile(&file) != SoLoud::SO_NO_ERROR) throw std::runtime_error(fmt::format("SoundManager: Cannot load sound {}", path));
	m_sounds[name] = std::move(wav);
	UpdateSoundEnvironment(name);
	//const std::vector<uint8_t> Data = GetFileDataInByte(path);
	//m_sounds[name].loadMem(Data.data(), Data.size(), true);
	//m_sounds[name].setSingleInstance(true);
}
SoLoud::Wav SoundManager::GetSound(const std::string &name) {
	return *m_sounds[name];
}
void SoundManager::SetEnvironment(const SoundEnvironment val) {
	m_env = val;
	for (auto &vl: m_sounds | std::views::keys) {
		UpdateSoundEnvironment(vl);
	}
}
SoundEnvironment SoundManager::GetEnvironment() {
	return m_env;
}
void SoundManager::SetLoop(const std::string &name, const bool loop) {
	if (!m_sounds.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("SoundManager: Cannot find {}", name));
	m_sounds[name]->setLooping(loop);
}
void SoundManager::StopSound(const std::string &name) {
	if (!m_sounds.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("SoundManager: Cannot find {}", name));
	m_sounds[name]->stop();
}
void SoundManager::StopAllSound() {
	for (auto &val: m_sounds | std::views::values) {
		val->stop();
	}
}
void SoundManager::PlaySound(const std::string &name) {
	if (!m_sounds.contains(name))
		AddPlaySound(name, m_queue[name]);
		//throw MFCPP::Exception::NonExistElement(fmt::format("SoundManager: Cannot find {}", name));
	if (audio_engine.getActiveVoiceCount() > 80) return;
	if (audio_engine.getActiveVoiceCount() > 48) m_sounds[name]->stop();
	const SoLoud::handle handle = audio_engine.play(*m_sounds[name]);
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
	MFCPP::Log::InfoPrint("SoundManager cleanup...");
	m_sounds.clear();
}