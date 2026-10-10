#include "Core/SoundManager.hpp"

#include <ranges>
#include <magic_enum/magic_enum.hpp>
#include <boost/unordered/unordered_flat_map.hpp>
#include "soloud_echofilter.h"
#include "soloud_freeverbfilter.h"
#include "Core/AudioEngine.hpp"
#include "Core/Exception.hpp"
#include "Core/Logging.hpp"
#include "Core/Enumeration/SoundEnum.hpp"
#include "Core/Loading/PhysFsStream.hpp"

struct QueueData {
	std::filesystem::path path;
	bool changeEnv = true;
	float volume = 1.f;
	bool looping = false;
};
struct PlayData {
	SoLoud::WavStream sound;
	bool changeEnv = true;
};

boost::unordered_flat_map<SoundID, std::unique_ptr<PlayData>> g_sounds;
boost::unordered_flat_map<SoundID, QueueData> g_queue;

SoLoud::Bus g_cave_bus;
SoLoud::Bus g_normal_bus;
SoLoud::EchoFilter g_echo_filter;
SoLoud::FreeverbFilter g_reverb_filter;
SoundEnvironment g_env;
bool g_play = false;

void AddPlaySound(SoundID name, const std::filesystem::path& path, bool changeEnv) {
	if (g_sounds.contains(name)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("SoundManager: Already exist sound with this name {}", magic_enum::enum_name(name)));
	auto wav = std::make_unique<PlayData>();
	PhysFsSoLoudStream file;
	if (!file.open(path.string().c_str())) throw std::runtime_error(fmt::format("PhysFS Error: Cannot open {}", path.string()));
	if (wav->sound.loadFileToMem(&file) != SoLoud::SO_NO_ERROR) throw std::runtime_error(fmt::format("SoundManager: Cannot load sound {}", path.string()));
	wav->changeEnv = changeEnv;
	wav->sound.mVolume = g_queue[name].volume;
	wav->sound.setLooping(g_queue[name].looping);
	g_sounds[name] = std::move(wav);
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
	g_echo_filter.setParams(0.1f, 0.35f, 0.25f);
	g_reverb_filter.setParams(0.f, 0.72f, 0.45f, 0.80f);

	g_cave_bus.setFilter(0, &g_echo_filter);
	g_cave_bus.setFilter(1, &g_reverb_filter);
	const auto handle = AudioEngine::getAudioEngine().play(g_cave_bus, 0.8f);
	AudioEngine::getAudioEngine().setFilterParameter(handle, 1, SoLoud::FreeverbFilter::WET, 0.4f);
	AudioEngine::getAudioEngine().play(g_normal_bus, 1.6f);

}
void SoundManager::AddSound(SoundID name, const std::filesystem::path& path, const bool changeEnv) {
	if (g_queue.contains(name)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("SoundManager: Already exist sound with this name {}", magic_enum::enum_name(name)));
	QueueData& dat = g_queue[name];
	dat.path = path;
	dat.changeEnv = changeEnv;
	dat.volume = 1.f;
	dat.looping = false;
}
SoLoud::WavStream SoundManager::GetSound(SoundID name) {
	return g_sounds[name]->sound;
}
void SoundManager::SetEnvironment(const SoundEnvironment val) {
	g_env = val;
}
SoundEnvironment SoundManager::GetEnvironment() {
	return g_env;
}
void SoundManager::SetLoop(SoundID name, bool loop) {
	auto it = g_queue.find(name);
	if (it == g_queue.end()) throw MFCPP::Exception::NonExistElement(fmt::format("SoundManager: Sound {} doesnt preloaded yet.", magic_enum::enum_name(name)));
	it->second.looping = loop;
}
void SoundManager::SetVolume(SoundID name, float val) {
	auto it = g_queue.find(name);
	if (it == g_queue.end()) throw MFCPP::Exception::NonExistElement(fmt::format("SoundManager: Sound {} doesnt preloaded yet.", magic_enum::enum_name(name)));
	it->second.volume = val;
}
void SoundManager::StopSound(SoundID name) {
	auto it = g_sounds.find(name);
	if (it == g_sounds.end()) throw MFCPP::Exception::NonExistElement(fmt::format("SoundManager: Cannot find {}", magic_enum::enum_name(name)));
	it->second->sound.stop();
}
void SoundManager::StopAllSound() {
	for (auto &val: g_sounds | std::views::values) {
		val->sound.stop();
	}
}
void SoundManager::PlaySound(SoundID name) {
	if (!g_sounds.contains(name)) {
		QueueData& dat = g_queue[name];
		AddPlaySound(name, dat.path.string(), dat.changeEnv);
	}
	if (AudioEngine::getAudioEngine().getActiveVoiceCount() > 80) return;
	if (AudioEngine::getAudioEngine().getActiveVoiceCount() > 48) g_sounds[name]->sound.stop();
	SoLoud::handle handle;
	if (!g_sounds[name]->changeEnv) handle = AudioEngine::getAudioEngine().play(g_sounds[name]->sound);
	else {
		switch (g_env) {
			case SoundEnvironment::OVERWORLD:
				handle = g_normal_bus.play(g_sounds[name]->sound);
				break;
			case SoundEnvironment::UNDERGROUND:
				handle = g_cave_bus.play(g_sounds[name]->sound);
				break;
			default:
				handle = g_normal_bus.play(g_sounds[name]->sound);
				break;
		}
	}
	AudioEngine::getAudioEngine().setInaudibleBehavior(handle,false, false);
	AudioEngine::getAudioEngine().setVolume(handle, g_sounds[name]->sound.mVolume);
}
void SoundManager::CleanUp() {
	MFCPP::Log::InfoPrint("SoundManager cleanup...");
	g_sounds.clear();
}