#include "Core/MusicManager.hpp"

#include <ranges>
#include <soloud_wav.h>
#include <boost/unordered/unordered_flat_map.hpp>

#include "Core/AudioEngine.hpp"
#include "Core/Exception.hpp"
#include "Core/Logging.hpp"
#include "Core/Loading/PhysFsStream.hpp"

struct PreMusic {
	std::filesystem::path path;
	bool isLoop = false;
	float volume = 1.f;
};
boost::unordered_flat_map<std::string, std::unique_ptr<SoLoud::AudioSource>> g_musics;
boost::unordered_flat_map<std::string, SoLoud::handle> g_musics_handle;
boost::unordered_flat_map<std::string, PreMusic> g_pre_musics;
bool CheckValidWavType(std::string_view str) {
	return str == ".flac" || str == ".mp3" || str == ".ogg" || str == ".wav";
}
void MusicManager::ForceLoadMusic(std::string_view name) {
	auto it = g_pre_musics.find(name.data());
	if (it == g_pre_musics.end()) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	AddPlayMusic(name.data(), it->second.path);
}
void MusicManager::AddMusic(std::string_view name, const std::filesystem::path &path) {
	if (g_pre_musics.contains(name.data())) throw MFCPP::Exception::AlreadyExistElement(fmt::format("MusicManager: Already exist music with this name {}", name));
	g_pre_musics[name.data()] = PreMusic(path, false, 1.f);
}
void MusicManager::AddPlayMusic(std::string_view name, const std::filesystem::path &path) {
	const std::string extension = path.extension().string();
	if (g_musics.contains(name.data())) throw MFCPP::Exception::AlreadyExistElement(fmt::format("MusicManager: Already exist music with this name {}", name));

	PhysFsSoLoudStream file;
	if (!file.open(path.string().c_str())) throw std::runtime_error(fmt::format("PhysFS Error: Cannot open {}", path.string()));
	if (CheckValidWavType(extension)) {
		auto ogg = std::make_unique<SoLoud::Wav>();
		if (ogg->loadFile(&file) != SoLoud::SO_NO_ERROR)
			throw std::runtime_error(fmt::format("MusicManager: Cannot detected {} is music file supported or not.", path.string()));
		MFCPP::Log::InfoPrint(fmt::format("MusicManager: Loaded Music File: {}", path.string()));
		ogg->setSingleInstance(true);
		g_musics[name.data()] = std::move(ogg);
		return;
	}
	throw std::format_error(fmt::format("MusicManager: Cannot detected {} is music file supported or not.", path.string()));
}
void MusicManager::SetLoop(std::string_view name, bool loop) {
	auto it = g_pre_musics.find(name.data());
	if (it == g_pre_musics.end()) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	it->second.isLoop = loop;
}
void MusicManager::StopMusic(std::string_view name) {
	auto it = g_musics.find(name.data());
	if (it == g_musics.end()) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	AudioEngine::getAudioEngine().stopAudioSource(*it->second);
	g_musics_handle.erase(name.data());
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
void MusicManager::PlayMusic(std::string_view name) {
	auto pre_it = g_pre_musics.find(name.data());
	if (pre_it == g_pre_musics.end())
		throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: {} doesnt preloaded yet.", name));
	if (!g_musics.contains(name.data()))
		AddPlayMusic(name, pre_it->second.path);

		//throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	const SoLoud::handle handle = AudioEngine::getAudioEngine().play(*g_musics[name.data()]);
	AudioEngine::getAudioEngine().setProtectVoice(handle, true);
	AudioEngine::getAudioEngine().setLooping(handle, pre_it->second.isLoop);
	AudioEngine::getAudioEngine().setVolume(handle, pre_it->second.volume);
	if (AudioEngine::getAudioEngine().isValidVoiceHandle(handle))
		g_musics_handle[name.data()] = handle;
}
void MusicManager::PauseMusic(std::string_view name) {
	if (!g_pre_musics.contains(name.data())) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	auto it = g_musics_handle.find(name.data());
	if (!AudioEngine::getAudioEngine().isValidVoiceHandle(it->second)) {
		MFCPP::Log::WarningPrint(fmt::format("MusicManager: {} is not play yet.", name));
		return;
	}
	AudioEngine::getAudioEngine().setPause(it->second, !AudioEngine::getAudioEngine().getPause(it->second));
}
void MusicManager::CleanUp() {
	MFCPP::Log::InfoPrint("MusicManager cleanup...");
	g_musics.clear();
	g_musics_handle.clear();
}
void MusicManager::SetMusicVolume(std::string_view name, float volume) {
	auto it = g_pre_musics.find(name.data());
	if (it == g_pre_musics.end()) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	it->second.volume = volume;
}
bool MusicManager::IsMusicPlaying(std::string_view name) {
	if (!g_pre_musics.contains(name.data())) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	return AudioEngine::getAudioEngine().isValidVoiceHandle(g_musics_handle[name.data()]);
}
bool MusicManager::IsMusicStopped(std::string_view name) {
	if (!g_pre_musics.contains(name.data())) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	return !AudioEngine::getAudioEngine().isValidVoiceHandle(g_musics_handle[name.data()]);
}
bool MusicManager::IsMusicPaused(std::string_view name) {
	if (!g_pre_musics.contains(name.data())) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));

	if (!AudioEngine::getAudioEngine().isValidVoiceHandle(g_musics_handle[name.data()])) {
		MFCPP::Log::WarningPrint(fmt::format("MusicManager: {} is not play yet.", name));
		return false;
	}
	return AudioEngine::getAudioEngine().getPause(g_musics_handle[name.data()]);
}