#ifndef AUDIOENGINE_HPP
#define AUDIOENGINE_HPP

#include "soloud.h"

class AudioEngine final {
public:
    AudioEngine() = delete;
    AudioEngine(const AudioEngine&) = delete;
    AudioEngine(AudioEngine&&) = delete;
    AudioEngine& operator=(const AudioEngine&) = delete;
    AudioEngine& operator=(AudioEngine&&) = delete;

    [[nodiscard]] static SoLoud::Soloud& getAudioEngine();
    static void Init();
    static void DeInit();
    static void SoundManagerParamsInit();
private:
    static SoLoud::Soloud audioEngine;
};
#endif //AUDIOENGINE_HPP
