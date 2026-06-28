#include "Core/SoundManager.hpp"

#include <ranges>
#include <boost/unordered/unordered_flat_map.hpp>
#include "soloud_echofilter.h"
#include "soloud_freeverbfilter.h"
#include "Core/AudioEngine.hpp"
#include "Core/Exception.hpp"
#include "Core/Logging.hpp"
#include "Core/Loading/PhysFsStream.hpp"

struct QueueData {
	std::filesystem::path path;
	bool changeEnv = true;
};
struct PlayData {
	SoLoud::Wav sound;
	bool changeEnv = true;
};

boost::unordered_flat_map<std::string, std::unique_ptr<PlayData>> g_sounds;
boost::unordered_flat_map<std::string, QueueData> g_queue;

SoLoud::EchoFilter g_echo_filter;
SoLoud::FreeverbFilter g_reverb_filter;
SoundEnvironment g_env = SoundEnvironment::UNDERGROUND;

void UpdateSoundEnvironment(std::string_view name) {
	PlayData* dat = g_sounds[name.data()].get();
	if (!dat->changeEnv) {
		dat->sound.setFilter(0, nullptr);
		dat->sound.setFilter(1, nullptr);
		return;
	}

	switch (g_env) {
		case SoundEnvironment::UNDERGROUND:
			dat->sound.setFilter(0, &g_echo_filter);
			dat->sound.setFilter(1, &g_reverb_filter);
			break;
		case SoundEnvironment::OVERWORLD:
			dat->sound.setFilter(0, nullptr);
			dat->sound.setFilter(1, nullptr);
			break;
		default: ;
	}
}
void AddPlaySound(std::string_view name, const std::filesystem::path& path, bool changeEnv) {
	if (g_sounds.contains(name.data())) throw MFCPP::Exception::AlreadyExistElement(fmt::format("SoundManager: Already exist sound with this name {}", name));
	auto wav = std::make_unique<PlayData>();
	PhysFsSoLoudStream file;
	if (!file.open(path.string().c_str())) throw std::runtime_error(fmt::format("PhysFS Error: Cannot open {}", path.string()));
	if (wav->sound.loadFile(&file) != SoLoud::SO_NO_ERROR) throw std::runtime_error(fmt::format("SoundManager: Cannot load sound {}", path.string()));
	wav->changeEnv = changeEnv;
	g_sounds[name.data()] = std::move(wav);
	UpdateSoundEnvironment(name.data());
}

void SoundManager::setEnvironmentID(uint8_t val) {
	switch (val) {
		case 0:
			g_env = SoundEnvironment::OVERWORLD;
			break;
		case 1:
			g_env = SoundEnvironment::UNDERGROUND;
			break;
		default:
			g_env = SoundEnvironment::OVERWORLD;
			break;
	}
}

void SoundManager::SoundManagerInit() {
	g_echo_filter.setParams(0.04f, 0.5f);
	g_reverb_filter.setParams(0, 0.45f, 0.5f, 0.86f);
}
void SoundManager::AddSound(std::string_view name, const std::filesystem::path& path, const bool changeEnv) {
	if (g_queue.contains(name.data())) throw MFCPP::Exception::AlreadyExistElement(fmt::format("SoundManager: Already exist sound with this name {}", name));
	QueueData& dat = g_queue[name.data()];
	dat.path = path;
	dat.changeEnv = changeEnv;
}
SoLoud::Wav SoundManager::GetSound(std::string_view name) {
	return g_sounds[name.data()]->sound;
}
void SoundManager::SetEnvironment(const SoundEnvironment val) {
	g_env = val;
	for (auto &vl: g_sounds | std::views::keys) {
		UpdateSoundEnvironment(vl);
	}
}
SoundEnvironment SoundManager::GetEnvironment() {
	return g_env;
}
void SoundManager::SetLoop(std::string_view name, const bool loop) {
	auto it = g_sounds.find(name.data());
	if (it == g_sounds.end()) throw MFCPP::Exception::NonExistElement(fmt::format("SoundManager: Cannot find {}", name));
	it->second->sound.setLooping(loop);
}
void SoundManager::StopSound(std::string_view name) {
	auto it = g_sounds.find(name.data());
	if (it == g_sounds.end()) throw MFCPP::Exception::NonExistElement(fmt::format("SoundManager: Cannot find {}", name));
	it->second->sound.stop();
}
void SoundManager::StopAllSound() {
	for (auto &val: g_sounds | std::views::values) {
		val->sound.stop();
	}
}
void SoundManager::PlaySound(std::string_view name) {
	if (!g_sounds.contains(name.data())) {
		QueueData& dat = g_queue[name.data()];
		AddPlaySound(name, dat.path.string(), dat.changeEnv);
	}
	//throw MFCPP::Exception::NonExistElement(fmt::format("SoundManager: Cannot find {}", name));
	if (AudioEngine::getAudioEngine().getActiveVoiceCount() > 80) return;
	if (AudioEngine::getAudioEngine().getActiveVoiceCount() > 48) g_sounds[name.data()]->sound.stop();
	const SoLoud::handle handle = AudioEngine::getAudioEngine().play(g_sounds[name.data()]->sound);
	AudioEngine::getAudioEngine().setInaudibleBehavior(handle,false, false);
	if (!g_sounds[name.data()]->changeEnv) {
		AudioEngine::getAudioEngine().setVolume(handle, 1.6f);
		return;
	}
	switch (g_env) {
		case SoundEnvironment::UNDERGROUND:
			AudioEngine::getAudioEngine().setFilterParameter(handle, 1, SoLoud::FreeverbFilter::WET, 0.4f);
			AudioEngine::getAudioEngine().setVolume(handle, 0.8f);
			break;
		case SoundEnvironment::OVERWORLD:
			AudioEngine::getAudioEngine().setVolume(handle, 1.6f);
			break;
		default: ;
	}
}
void SoundManager::CleanUp() {
	MFCPP::Log::InfoPrint("SoundManager cleanup...");
	g_sounds.clear();
}