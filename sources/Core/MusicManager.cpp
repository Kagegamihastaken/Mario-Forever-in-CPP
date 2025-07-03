#include "Core/MusicManager.hpp"

#include <ranges>

#include "Core/AudioEngine.hpp"
#include "Core/Exception.hpp"
#include "Core/Logging.hpp"
#include "Core/Loading/Loading.hpp"

std::map<std::string, SoLoud::Wav> MusicManager::m_ogg_musics;
std::map<std::string, SoLoud::Openmpt> MusicManager::m_mod_musics;
std::map<std::string, bool> MusicManager::m_musics_id;
std::map<std::string, SoLoud::handle> MusicManager::m_musics_handle;
//false for OGG, true for MOD
bool MusicManager::CheckValidWavType(const std::string_view& str) {
	return str == ".flac" || str == ".mp3" || str == ".ogg" || str == ".wav";
}
void MusicManager::AddMusic(const std::string &name, const std::filesystem::path &path) {
	const std::string extension = path.extension().string();
	//std::filesystem::path::extension
	//custom loading
	if (m_musics_id.contains(name)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("MusicManager: Already exist music with this name {}", name));
	const std::vector<uint8_t> Data = GetFileDataInByte(path.string());
	bool flag = false;
	if (CheckValidWavType(extension)) {
		if (m_ogg_musics[name].loadMem(Data.data(), Data.size(), true))
			throw std::format_error(fmt::format("MusicManager: Cannot detected {} is music file supported or not.", path.string()));
		MFCPP::Log::InfoPrint(fmt::format("MusicManager: Loaded OGG File: {}", path.string()));
		m_ogg_musics[name].setSingleInstance(true);
	}
	else {
		if (m_mod_musics[name].loadMem(Data.data(), Data.size(), true))
			throw std::format_error(fmt::format("MusicManager: Cannot detected {} is music file supported or not.", path.string()));
		MFCPP::Log::InfoPrint(fmt::format("MusicManager: Loaded MOD File: {}", path.string()));
		m_mod_musics[name].setSingleInstance(true);
		flag = true;
	}
	m_musics_id[name] = flag;
	//LoadOGG(m_ogg_musics[name], path);
}
void MusicManager::SetLoop(const std::string &name, const bool loop) {
	if (!m_musics_id.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	if (!m_musics_id[name])
		m_ogg_musics[name].setLooping(loop);
	else
		m_mod_musics[name].setLooping(loop);
}
void MusicManager::StopMusic(const std::string &name) {
	if (!m_musics_id.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	if (!m_musics_id[name])
		audio_engine.stopAudioSource(m_ogg_musics[name]);
	else
		audio_engine.stopAudioSource(m_mod_musics[name]);
	m_musics_handle.erase(name);
}
void MusicManager::StopAllMusic() {
	std::set<std::string> keys;
	for (const auto &fst: m_musics_id | std::views::keys) {
		if (!m_musics_id[fst])
			audio_engine.stopAudioSource(m_ogg_musics[fst]);
		else
			audio_engine.stopAudioSource(m_mod_musics[fst]);
		keys.insert(fst);
	}
	for (const auto &i : keys) {
		m_musics_handle.erase(i);
	}
}
void MusicManager::PlayMusic(const std::string &name) {
	if (!m_musics_id.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	SoLoud::handle handle;
	if (!m_musics_id[name])
		handle = audio_engine.playBackground(m_ogg_musics[name]);
	else
		handle = audio_engine.playBackground(m_mod_musics[name]);
	audio_engine.setProtectVoice(handle, true);
	audio_engine.setVolume(handle, 0.725f);
	if (audio_engine.isValidVoiceHandle(handle))
		m_musics_handle[name] = handle;
}
void MusicManager::PauseMusic(const std::string &name) {
	if (!m_musics_id.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));

	if (!audio_engine.isValidVoiceHandle(m_musics_handle[name])) {
		MFCPP::Log::WarningPrint(fmt::format("MusicManager: {} is not play yet.", name));
		return;
	}
	audio_engine.setPause(m_musics_handle[name], !audio_engine.getPause(m_musics_handle[name]));
}
void MusicManager::CleanUp() {
	m_ogg_musics.clear();
	m_mod_musics.clear();
	m_musics_id.clear();
	m_musics_handle.clear();
}
void MusicManager::SetMusicVolume(const std::string &name, const float volume) {
	if (!m_musics_id.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	if (!m_musics_id[name])
		m_ogg_musics[name].setVolume(volume);
	else
		m_mod_musics[name].setVolume(volume);
}
bool MusicManager::IsMusicPlaying(const std::string &name) {
	if (!m_musics_id.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	return audio_engine.isValidVoiceHandle(m_musics_handle[name]);
}
bool MusicManager::IsMusicStopped(const std::string &name) {
	if (!m_musics_id.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	return !audio_engine.isValidVoiceHandle(m_musics_handle[name]);
}
bool MusicManager::IsMusicPaused(const std::string &name) {
	if (!m_musics_id.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));

	if (!audio_engine.isValidVoiceHandle(m_musics_handle[name])) {
		MFCPP::Log::WarningPrint(fmt::format("MusicManager: {} is not play yet.", name));
		return false;
	}
	return audio_engine.getPause(m_musics_handle[name]);
}