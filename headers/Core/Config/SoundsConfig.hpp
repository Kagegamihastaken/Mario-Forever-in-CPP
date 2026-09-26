#ifndef MFCPP_SOUNDSCONFIG_HPP
#define MFCPP_SOUNDSCONFIG_HPP
#include <filesystem>

namespace MFCPP {
    namespace SoundsConfig {
        void loadFile(const std::filesystem::path& path);
    }
}

#endif //MFCPP_SOUNDSCONFIG_HPP
