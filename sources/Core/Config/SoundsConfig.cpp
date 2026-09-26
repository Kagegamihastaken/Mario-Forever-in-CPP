#include "Core/Config/SoundsConfig.hpp"
#include <toml++/toml.hpp>

#include "Core/SoundManager.hpp"
#include "Core/Enumeration/SoundEnum.hpp"
#include "Core/Loading/Loading.hpp"

void MFCPP::SoundsConfig::loadFile(const std::filesystem::path& path) {
    std::string TOMLFileRaw;
    IO::LoadRaw(TOMLFileRaw, path.string());
    toml::table TOMLFile = toml::parse(TOMLFileRaw);

    toml::array* soundsArray = TOMLFile["sounds"].as_array();
    for (auto& soundNode : *soundsArray) {
        toml::table* currentSound = soundNode.as_table();

        const toml::node* idNode = currentSound->get("id");
        const toml::node* pathNode = currentSound->get("path");
        const toml::node* changeEnvNode = currentSound->get("change_environment");

        if (!idNode || !pathNode || !changeEnvNode)
            throw std::runtime_error("SoundsConfig: Missing required fields in sound configuration.");

        std::string idNodeCheckValue = idNode->value_or("NULL_");
        auto idValue = magic_enum::enum_cast<SoundID>(idNodeCheckValue, magic_enum::case_insensitive);
        if (idValue.has_value()) {
            std::string pathNodeCheckValue = pathNode->value_or("NULL_");
            if (pathNodeCheckValue == "NULL_")
                throw std::runtime_error(fmt::format("SoundsConfig: Invalid path for sound ID '{}'", idNodeCheckValue));
            SoundManager::AddSound(idValue.value(), pathNode->value_or("NULL_"), changeEnvNode->value_or(false));
        } else {
            throw std::runtime_error(fmt::format("SoundsConfig: Invalid sound ID '{}'", idNodeCheckValue));
        }
    }
}
