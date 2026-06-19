#include "Core/AnimationSequenceManager.hpp"

#include "Core/Exception.hpp"

std::unordered_map<std::string, std::vector<std::string>> MFCPP::AnimationSequenceManager::m_data;

void MFCPP::AnimationSequenceManager::clearData(std::string_view name) {
    m_data[name.data()].clear();
}

void MFCPP::AnimationSequenceManager::addData(std::string_view name, const std::vector<std::string> &data) {
    clearData(name);
    m_data[name.data()].assign(data.begin(), data.end());
}

std::vector<std::string> &MFCPP::AnimationSequenceManager::getData(std::string_view name) {
    if (!m_data.contains(name.data()))
        throw Exception::NonExistElement(fmt::format("AnimationSequenceManager: cannot find {}", name));
    return m_data[name.data()];
}
