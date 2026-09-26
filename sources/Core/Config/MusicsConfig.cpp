#include "Core/Config/MusicsConfig.hpp"
#include <toml++/toml.hpp>
#include "Core/Loading/Loading.hpp"
#include "Core/MusicManager.hpp"

void MFCPP::MusicsConfig::loadFile(const std::filesystem::path& path) {
    std::string TOMLFileRaw;
    IO::LoadRaw(TOMLFileRaw, path.string());
    toml::table TOMLFile = toml::parse(TOMLFileRaw);

    toml::array* musicsArray = TOMLFile["musics"].as_array();
    for (auto& musicNode : *musicsArray) {
        toml::table* currentMusic = musicNode.as_table();

        const toml::node* idNode = currentMusic->get("id");
        const toml::node* pathNode = currentMusic->get("path");

        if (!idNode || !pathNode)
            throw std::runtime_error("MusicsConfig: Missing required fields in music configuration.");
        std::string idNodeCheckValue = idNode->value_or("NULL_");
        auto idValue = magic_enum::enum_cast<MusicID>(idNodeCheckValue, magic_enum::case_insensitive);
        if (idValue.has_value()) {
            std::string pathNodeCheckValue = pathNode->value_or("NULL_");
            if (pathNodeCheckValue == "NULL_")
                throw std::runtime_error(fmt::format("MusicsConfig: Invalid path for music ID '{}'", idNodeCheckValue));
            MusicManager::AddMusic(idValue.value(), pathNode->value_or("NULL_"));
        } else {
            throw std::runtime_error(fmt::format("MusicsConfig: Invalid music ID '{}'", idNodeCheckValue));
        }
    }
}