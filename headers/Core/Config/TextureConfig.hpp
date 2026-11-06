#ifndef MFCPP_TEXTURECONFIG_HPP
#define MFCPP_TEXTURECONFIG_HPP

#include <filesystem>

namespace MFCPP {
    namespace TextureConfig {
        void loadFile(const char* raw, bool force_preload = false);
    }
}
#endif //MFCPP_TEXTURECONFIG_HPP
