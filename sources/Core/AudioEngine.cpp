#include "Core/AudioEngine.hpp"
#include <stdexcept>
#include "Core/SoundManager.hpp"
SoLoud::Soloud audio_engine;
void AudioEngineInit() {
    if (audio_engine.init(SoLoud::Soloud::CLIP_ROUNDOFF, SoLoud::Soloud::AUTO, SoLoud::Soloud::AUTO)!= 0) throw std::runtime_error("AudioEngine: Failed to Init");
    audio_engine.setMaxActiveVoiceCount(85);
}
void AudioEnginePlay() {
    //SoundManager
    //SoundManager::getBuffer().
    SoundManager::SoundManagerInit();
    //audio_engine.setVolume(i, 1.5f);

}
void AudioEngineDeInit() {
    audio_engine.deinit();
}