#include "Core/AnimationSequenceManager.hpp"

#include "Core/Exception.hpp"
#include <boost/unordered/unordered_flat_map.hpp>

#include "Core/Logging.hpp"
boost::unordered_flat_map<std::string, std::unique_ptr<MFCPP::AnimationSequence>> m_data;
void MFCPP::AnimationSequenceManager::clearData(std::string_view name) {
    m_data[name.data()].reset();
}
void MFCPP::AnimationSequenceManager::newData(std::string_view name) {
    m_data[name.data()] = std::make_unique<MFCPP::AnimationSequence>();
}
void MFCPP::AnimationSequenceManager::addSingleFrame(std::string_view name, std::string_view val) {
    if (!m_data.contains(name.data())) {
        Log::WarningPrint(fmt::format("AnimationSequenceManager: Cannot find Animation Sequence {}, abort.", name));
        return;
    }
    m_data[name.data()]->emplace_back(val.data());
}

MFCPP::AnimationSequence* MFCPP::AnimationSequenceManager::getData(std::string_view name) {
    if (!m_data.contains(name.data()))
        throw Exception::NonExistElement(fmt::format("AnimationSequenceManager: cannot find {}", name));
    return m_data[name.data()].get();
}
