#include "Core/MusicManager.hpp"

#include <ranges>

#include "Core/AudioEngine.hpp"
#include "Core/Exception.hpp"
#include "Core/Logging.hpp"
#include "Core/Loading/PhysFsStream.hpp"

std::map<std::string, std::unique_ptr<SoLoud::AudioSource>> MusicManager::m_musics;
std::map<std::string, SoLoud::handle> MusicManager::m_musics_handle;
std::map<std::string, PreMusic> MusicManager::m_pre_musics;
//false for OGG, true for MOD
bool MusicManager::CheckValidWavType(const std::string_view& str) {
	return str == ".flac" || str == ".mp3" || str == ".ogg" || str == ".wav";
}
void MusicManager::ForceLoadMusic(const std::string& name) {
	if (!m_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	AddPlayMusic(name, m_pre_musics[name].path);
}
void MusicManager::AddMusic(const std::string &name, const std::filesystem::path &path) {
	if (m_pre_musics.contains(name)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("MusicManager: Already exist music with this name {}", name));
	m_pre_musics[name] = PreMusic(path, false, 1.f);
}
void MusicManager::AddPlayMusic(const std::string &name, const std::filesystem::path &path) {
	const std::string extension = path.extension().string();
	if (m_musics.contains(name)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("MusicManager: Already exist music with this name {}", name));

	PhysFsSoLoudStream file;
	if (!file.open(path.string().c_str())) throw std::runtime_error(fmt::format("PhysFS Error: Cannot open {}", path.string()));
	if (CheckValidWavType(extension)) {
		auto ogg = std::make_unique<SoLoud::Wav>();
		if (ogg->loadFile(&file) != SoLoud::SO_NO_ERROR)
			throw std::runtime_error(fmt::format("MusicManager: Cannot detected {} is music file supported or not.", path.string()));
		MFCPP::Log::InfoPrint(fmt::format("MusicManager: Loaded Music File: {}", path.string()));
		ogg->setSingleInstance(true);
		//ogg->setLooping(isLoop);
		m_musics[name] = std::move(ogg);
	}
	else {
		throw std::format_error(fmt::format("MusicManager: Cannot detected {} is music file supported or not.", path.string()));
	}
	//LoadOGG(m_ogg_musics[name], path);
}
void MusicManager::SetLoop(const std::string &name, const bool loop) {
	if (!m_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	m_pre_musics[name].isLoop = loop;
}
void MusicManager::StopMusic(const std::string &name) {
	if (!m_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	audio_engine.stopAudioSource(*m_musics[name]);
	m_musics_handle.erase(name);
}
void MusicManager::StopAllMusic() {
	std::set<std::string> keys;
	for (const auto &fst: m_musics | std::views::keys) {
		audio_engine.stopAudioSource(*m_musics[fst]);
		keys.insert(fst);
	}
	for (const auto &i : keys) {
		m_musics_handle.erase(i);
	}
}
void MusicManager::PlayMusic(const std::string &name) {
	if (!m_musics.contains(name))
		AddPlayMusic(name, m_pre_musics[name].path);
		//throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	const SoLoud::handle handle = audio_engine.play(*m_musics[name]);
	audio_engine.setProtectVoice(handle, true);
	audio_engine.setLooping(handle, m_pre_musics[name].isLoop);
	audio_engine.setVolume(handle, m_pre_musics[name].volume);
	if (audio_engine.isValidVoiceHandle(handle))
		m_musics_handle[name] = handle;
}
void MusicManager::PauseMusic(const std::string &name) {
	if (!m_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));

	if (!audio_engine.isValidVoiceHandle(m_musics_handle[name])) {
		MFCPP::Log::WarningPrint(fmt::format("MusicManager: {} is not play yet.", name));
		return;
	}
	audio_engine.setPause(m_musics_handle[name], !audio_engine.getPause(m_musics_handle[name]));
}
void MusicManager::CleanUp() {
	MFCPP::Log::InfoPrint("MusicManager cleanup...");
	m_musics.clear();
	m_musics_handle.clear();
}
void MusicManager::SetMusicVolume(const std::string &name, const float volume) {
	if (!m_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	m_pre_musics[name].volume = volume;
}
bool MusicManager::IsMusicPlaying(const std::string &name) {
	if (!m_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	return audio_engine.isValidVoiceHandle(m_musics_handle[name]);
}
bool MusicManager::IsMusicStopped(const std::string &name) {
	if (!m_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	return !audio_engine.isValidVoiceHandle(m_musics_handle[name]);
}
bool MusicManager::IsMusicPaused(const std::string &name) {
	if (!m_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));

	if (!audio_engine.isValidVoiceHandle(m_musics_handle[name])) {
		MFCPP::Log::WarningPrint(fmt::format("MusicManager: {} is not play yet.", name));
		return false;
	}
	return audio_engine.getPause(m_musics_handle[name]);
}