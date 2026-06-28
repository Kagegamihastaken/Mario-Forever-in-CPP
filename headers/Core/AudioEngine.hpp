#ifndef AUDIOENGINE_HPP
#define AUDIOENGINE_HPP

#include "soloud.h"

namespace AudioEngine {
    [[nodiscard]] SoLoud::Soloud& getAudioEngine();
    void Init();
    void DeInit();
    void SoundManagerParamsInit();
};
#endif //AUDIOENGINE_HPP
