#include "Core/MusicManager.hpp"

#include <ranges>
#include <soloud_wavstream.h>
#include <boost/unordered/unordered_flat_map.hpp>

#include "Core/AudioEngine.hpp"
#include "Core/Exception.hpp"
#include "Core/Logging.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Enumeration/MusicEnum.hpp"
#include "Core/Loading/PhysFsStream.hpp"

struct PreMusic {
	std::filesystem::path path;
	bool isLoop = false;
	float volume = 1.f;
};
boost::unordered_flat_map<MusicID, std::unique_ptr<SoLoud::AudioSource>> g_musics;
boost::unordered_flat_map<MusicID, SoLoud::handle> g_musics_handle;
boost::unordered_flat_map<MusicID, PreMusic> g_pre_musics;
SoLoud::Bus g_bus;
bool CheckValidMusicType(std::string_view str) {
	return str == ".flac" || str == ".mp3" || str == ".ogg" || str == ".wav";
}

void MusicManager::MusicManagerInit() {
	AudioEngine::getAudioEngine().play(g_bus);
}

void MusicManager::ForceLoadMusic(MusicID name) {
	auto it = g_pre_musics.find(name);
	if (it == g_pre_musics.end()) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", magic_enum::enum_name(name)));
	AddPlayMusic(name, it->second.path);
}
void MusicManager::AddMusic(MusicID name, const std::filesystem::path &path) {
	if (g_pre_musics.contains(name)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("MusicManager: Already exist music with this name {}", magic_enum::enum_name(name)));
	g_pre_musics[name] = PreMusic(path, false, 1.f);
}
void MusicManager::AddPlayMusic(MusicID name, const std::filesystem::path &path) {
	const std::string extension = path.extension().string();
	if (g_musics.contains(name)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("MusicManager: Already exist music with this name {}", magic_enum::enum_name(name)));

	PhysFsSoLoudStream file;
	if (!file.open(path.string().c_str())) throw std::runtime_error(fmt::format("PhysFS Error: Cannot open {}", path.string()));
	if (CheckValidMusicType(extension)) {
		auto audioSource = std::make_unique<SoLoud::WavStream>();
		if (audioSource->loadFileToMem(&file) != SoLoud::SO_NO_ERROR)
			throw std::runtime_error(fmt::format("MusicManager: Cannot detected {} is music file supported or not.", path.string()));
		MFCPP::Log::InfoPrint(fmt::format("MusicManager: Loaded Music File: {}", path.string()));
		audioSource->setSingleInstance(true);
		g_musics[name] = std::move(audioSource);
		return;
	}
	throw std::format_error(fmt::format("MusicManager: Cannot detected {} is music file supported or not.", path.string()));
}
void MusicManager::SetLoop(MusicID name, bool loop) {
	auto it = g_pre_musics.find(name);
	if (it == g_pre_musics.end()) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", magic_enum::enum_name(name)));
	it->second.isLoop = loop;
}
void MusicManager::StopMusic(MusicID name) {
	auto it = g_musics.find(name);
	if (it == g_musics.end()) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", magic_enum::enum_name(name)));
	AudioEngine::getAudioEngine().stopAudioSource(*it->second);
	g_musics.erase(it);
	g_musics_handle.erase(name);
}
void MusicManager::StopAllMusic() {
	for (const auto &[id, source] : g_musics) {
		AudioEngine::getAudioEngine().stopAudioSource(*source);
	}
	g_musics.clear();
	g_musics_handle.clear();
}
void MusicManager::PlayMusic(MusicID name) {
	auto pre_it = g_pre_musics.find(name);
	if (pre_it == g_pre_musics.end())
		throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: {} doesnt preloaded yet.", magic_enum::enum_name(name)));
	if (!g_musics.contains(name))
		AddPlayMusic(name, pre_it->second.path);

	const SoLoud::handle handle = g_bus.play(*g_musics[name]);
	AudioEngine::getAudioEngine().setProtectVoice(handle, true);
	AudioEngine::getAudioEngine().setLooping(handle, pre_it->second.isLoop);
	AudioEngine::getAudioEngine().setVolume(handle, pre_it->second.volume);
	AudioEngine::getAudioEngine().setSamplerate(handle, g_musics[name]->mBaseSamplerate * WindowFrame::getTimestep().getTimeSpeed());
	if (AudioEngine::getAudioEngine().isValidVoiceHandle(handle))
		g_musics_handle[name] = handle;
}
void MusicManager::PauseMusic(MusicID name) {
	if (!g_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", magic_enum::enum_name(name)));
	auto it = g_musics_handle.find(name);
	if (it == g_musics_handle.end() || !AudioEngine::getAudioEngine().isValidVoiceHandle(it->second)) {
		MFCPP::Log::WarningPrint(fmt::format("MusicManager: {} is not play yet.", magic_enum::enum_name(name)));
		return;
	}
	AudioEngine::getAudioEngine().setPause(it->second, !AudioEngine::getAudioEngine().getPause(it->second));
}
void MusicManager::PauseAllMusic() {
	for (const auto &[id, source] : g_musics) {
		AudioEngine::getAudioEngine().setPause(g_musics_handle[id], !AudioEngine::getAudioEngine().getPause(g_musics_handle[id]));
	}
}
void MusicManager::CleanUp() {
	MFCPP::Log::InfoPrint("MusicManager cleanup...");
	g_musics.clear();
	g_musics_handle.clear();
}
void MusicManager::SetMusicVolume(MusicID name, float volume) {
	auto it = g_pre_musics.find(name);
	if (it == g_pre_musics.end()) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", magic_enum::enum_name(name)));
	it->second.volume = volume;
}
bool MusicManager::IsMusicPlaying(MusicID name) {
	if (!g_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", magic_enum::enum_name(name)));
	auto it = g_musics_handle.find(name);
	if (it == g_musics_handle.end()) return false;
	return AudioEngine::getAudioEngine().isValidVoiceHandle(it->second);
}
bool MusicManager::IsMusicStopped(MusicID name) {
	if (!g_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", magic_enum::enum_name(name)));
	auto it = g_musics_handle.find(name);
	if (it == g_musics_handle.end()) return true;
	return !AudioEngine::getAudioEngine().isValidVoiceHandle(it->second);
}
bool MusicManager::IsMusicPaused(MusicID name) {
	if (!g_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", magic_enum::enum_name(name)));

	auto it = g_musics_handle.find(name);
	if (it == g_musics_handle.end() || !AudioEngine::getAudioEngine().isValidVoiceHandle(it->second)) {
		MFCPP::Log::WarningPrint(fmt::format("MusicManager: {} is not play yet.", magic_enum::enum_name(name)));
		return false;
	}
	return AudioEngine::getAudioEngine().getPause(it->second);
}