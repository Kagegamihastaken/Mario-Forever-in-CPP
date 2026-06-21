#ifndef MFCPP_ANIMATIONSEQUENCEMANAGER_HPP
#define MFCPP_ANIMATIONSEQUENCEMANAGER_HPP

#include <string>
#include <unordered_map>
#include <vector>

namespace MFCPP {
    class AnimationSequenceManager final {
        static std::unordered_map<std::string, std::vector<std::string>> m_data;
    public:
        AnimationSequenceManager() = delete;
        AnimationSequenceManager(const AnimationSequenceManager&) = delete;
        AnimationSequenceManager(AnimationSequenceManager&&) = delete;
        AnimationSequenceManager& operator=(const AnimationSequenceManager&) = delete;
        AnimationSequenceManager& operator=(AnimationSequenceManager&&) = delete;

        static std::vector<std::string>& getData(std::string_view name);
        static void addData(std::string_view name, const std::vector<std::string>& data);
        static void clearData(std::string_view name);
    };
}

#endif //MFCPP_ANIMATIONSEQUENCEMANAGER_HPP
