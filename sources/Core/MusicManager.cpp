#include "Core/MusicManager.hpp"

#include <ranges>
#include <soloud_wav.h>

#include "Core/AudioEngine.hpp"
#include "Core/Exception.hpp"
#include "Core/Logging.hpp"
#include "Core/Loading/PhysFsStream.hpp"

struct PreMusic {
	std::filesystem::path path;
	bool isLoop = false;
	float volume = 1.f;
};

std::unordered_map<std::string, std::unique_ptr<SoLoud::AudioSource>> g_musics;
std::unordered_map<std::string, SoLoud::handle> g_musics_handle;
std::unordered_map<std::string, PreMusic> g_pre_musics;
//false for OGG, true for MOD
bool MusicManager::CheckValidWavType(std::string_view str) {
	return str == ".flac" || str == ".mp3" || str == ".ogg" || str == ".wav";
}
void MusicManager::ForceLoadMusic(const std::string& name) {
	if (!g_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	AddPlayMusic(name, g_pre_musics[name].path);
}
void MusicManager::AddMusic(const std::string &name, const std::filesystem::path &path) {
	if (g_pre_musics.contains(name)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("MusicManager: Already exist music with this name {}", name));
	g_pre_musics[name] = PreMusic(path, false, 1.f);
}
void MusicManager::AddPlayMusic(const std::string &name, const std::filesystem::path &path) {
	const std::string extension = path.extension().string();
	if (g_musics.contains(name)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("MusicManager: Already exist music with this name {}", name));

	PhysFsSoLoudStream file;
	if (!file.open(path.string().c_str())) throw std::runtime_error(fmt::format("PhysFS Error: Cannot open {}", path.string()));
	if (CheckValidWavType(extension)) {
		auto ogg = std::make_unique<SoLoud::Wav>();
		if (ogg->loadFile(&file) != SoLoud::SO_NO_ERROR)
			throw std::runtime_error(fmt::format("MusicManager: Cannot detected {} is music file supported or not.", path.string()));
		MFCPP::Log::InfoPrint(fmt::format("MusicManager: Loaded Music File: {}", path.string()));
		ogg->setSingleInstance(true);
		//ogg->setLooping(isLoop);
		g_musics[name] = std::move(ogg);
	}
	else {
		throw std::format_error(fmt::format("MusicManager: Cannot detected {} is music file supported or not.", path.string()));
	}
	//LoadOGG(m_ogg_musics[name], path);
}
void MusicManager::SetLoop(const std::string &name, const bool loop) {
	if (!g_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	g_pre_musics[name].isLoop = loop;
}
void MusicManager::StopMusic(const std::string &name) {
	if (!g_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	AudioEngine::getAudioEngine().stopAudioSource(*g_musics[name]);
	g_musics_handle.erase(name);
}
void MusicManager::StopAllMusic() {
	std::set<std::string> keys;
	for (const auto &fst: g_musics | std::views::keys) {
		AudioEngine::getAudioEngine().stopAudioSource(*g_musics[fst]);
		keys.insert(fst);
	}
	for (const auto &i : keys) {
		g_musics_handle.erase(i);
	}
}
void MusicManager::PlayMusic(const std::string &name) {
	if (!g_musics.contains(name))
		AddPlayMusic(name, g_pre_musics[name].path);
		//throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	const SoLoud::handle handle = AudioEngine::getAudioEngine().play(*g_musics[name]);
	AudioEngine::getAudioEngine().setProtectVoice(handle, true);
	AudioEngine::getAudioEngine().setLooping(handle, g_pre_musics[name].isLoop);
	AudioEngine::getAudioEngine().setVolume(handle, g_pre_musics[name].volume);
	if (AudioEngine::getAudioEngine().isValidVoiceHandle(handle))
		g_musics_handle[name] = handle;
}
void MusicManager::PauseMusic(const std::string &name) {
	if (!g_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));

	if (!AudioEngine::getAudioEngine().isValidVoiceHandle(g_musics_handle[name])) {
		MFCPP::Log::WarningPrint(fmt::format("MusicManager: {} is not play yet.", name));
		return;
	}
	AudioEngine::getAudioEngine().setPause(g_musics_handle[name], !AudioEngine::getAudioEngine().getPause(g_musics_handle[name]));
}
void MusicManager::CleanUp() {
	MFCPP::Log::InfoPrint("MusicManager cleanup...");
	g_musics.clear();
	g_musics_handle.clear();
}
void MusicManager::SetMusicVolume(const std::string &name, const float volume) {
	if (!g_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	g_pre_musics[name].volume = volume;
}
bool MusicManager::IsMusicPlaying(const std::string &name) {
	if (!g_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	return AudioEngine::getAudioEngine().isValidVoiceHandle(g_musics_handle[name]);
}
bool MusicManager::IsMusicStopped(const std::string &name) {
	if (!g_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	return !AudioEngine::getAudioEngine().isValidVoiceHandle(g_musics_handle[name]);
}
bool MusicManager::IsMusicPaused(const std::string &name) {
	if (!g_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));

	if (!AudioEngine::getAudioEngine().isValidVoiceHandle(g_musics_handle[name])) {
		MFCPP::Log::WarningPrint(fmt::format("MusicManager: {} is not play yet.", name));
		return false;
	}
	return AudioEngine::getAudioEngine().getPause(g_musics_handle[name]);
}