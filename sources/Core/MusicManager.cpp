#include "Core/MusicManager.hpp"

#include <ranges>
#include <soloud_wav.h>
#include <boost/unordered/unordered_flat_map.hpp>

#include "Core/AudioEngine.hpp"
#include "Core/Exception.hpp"
#include "Core/Logging.hpp"
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
bool CheckValidWavType(std::string_view str) {
	return str == ".flac" || str == ".mp3" || str == ".ogg" || str == ".wav";
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
	if (CheckValidWavType(extension)) {
		auto ogg = std::make_unique<SoLoud::Wav>();
		if (ogg->loadFile(&file) != SoLoud::SO_NO_ERROR)
			throw std::runtime_error(fmt::format("MusicManager: Cannot detected {} is music file supported or not.", path.string()));
		MFCPP::Log::InfoPrint(fmt::format("MusicManager: Loaded Music File: {}", path.string()));
		ogg->setSingleInstance(true);
		g_musics[name] = std::move(ogg);
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
	g_musics_handle.erase(name);
}
void MusicManager::StopAllMusic() {
	std::set<MusicID> keys;
	for (const auto &fst: g_musics | std::views::keys) {
		AudioEngine::getAudioEngine().stopAudioSource(*g_musics[fst]);
		keys.insert(fst);
	}
	for (const auto &i : keys) {
		g_musics_handle.erase(i);
	}
}
void MusicManager::PlayMusic(MusicID name) {
	auto pre_it = g_pre_musics.find(name);
	if (pre_it == g_pre_musics.end())
		throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: {} doesnt preloaded yet.", magic_enum::enum_name(name)));
	if (!g_musics.contains(name))
		AddPlayMusic(name, pre_it->second.path);

		//throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	const SoLoud::handle handle = AudioEngine::getAudioEngine().play(*g_musics[name]);
	AudioEngine::getAudioEngine().setProtectVoice(handle, true);
	AudioEngine::getAudioEngine().setLooping(handle, pre_it->second.isLoop);
	AudioEngine::getAudioEngine().setVolume(handle, pre_it->second.volume);
	if (AudioEngine::getAudioEngine().isValidVoiceHandle(handle))
		g_musics_handle[name] = handle;
}
void MusicManager::PauseMusic(MusicID name) {
	if (!g_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", magic_enum::enum_name(name)));
	auto it = g_musics_handle.find(name);
	if (!AudioEngine::getAudioEngine().isValidVoiceHandle(it->second)) {
		MFCPP::Log::WarningPrint(fmt::format("MusicManager: {} is not play yet.", magic_enum::enum_name(name)));
		return;
	}
	AudioEngine::getAudioEngine().setPause(it->second, !AudioEngine::getAudioEngine().getPause(it->second));
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
	return AudioEngine::getAudioEngine().isValidVoiceHandle(g_musics_handle[name]);
}
bool MusicManager::IsMusicStopped(MusicID name) {
	if (!g_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", magic_enum::enum_name(name)));
	return !AudioEngine::getAudioEngine().isValidVoiceHandle(g_musics_handle[name]);
}
bool MusicManager::IsMusicPaused(MusicID name) {
	if (!g_pre_musics.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", magic_enum::enum_name(name)));

	if (!AudioEngine::getAudioEngine().isValidVoiceHandle(g_musics_handle[name])) {
		MFCPP::Log::WarningPrint(fmt::format("MusicManager: {} is not play yet.", magic_enum::enum_name(name)));
		return false;
	}
	return AudioEngine::getAudioEngine().getPause(g_musics_handle[name]);
}