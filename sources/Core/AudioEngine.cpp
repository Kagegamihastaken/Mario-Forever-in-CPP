#include <stdexcept>
#include "Core/AudioEngine.hpp"

#include "Core/Logging.hpp"
#include "Core/SoundManager.hpp"
SoLoud::Soloud audioEngine;
void AudioEngine::Init() {
    if (audioEngine.init() != 0) throw std::runtime_error("AudioEngine: Failed to Init");
    MFCPP::Log::InfoPrint("AudioEngine Successfully Initialized.");
    audioEngine.setMaxActiveVoiceCount(85);
}
void AudioEngine::SoundManagerParamsInit() {
    //SoundManager
    //SoundManager::getBuffer().
    SoundManager::SoundManagerInit();
    //audio_engine.setVolume(i, 1.5f);
}
void AudioEngine::DeInit() {
    audioEngine.deinit();
    MFCPP::Log::InfoPrint("AudioEngine Deinitialized...");
}

SoLoud::Soloud &AudioEngine::getAudioEngine() {
    return audioEngine;
}
