#include "Core/Config/ObstaclesConfig.hpp"
#include <boost/unordered/unordered_flat_map.hpp>
#include "Core/Loading/Loading.hpp"
#include <toml++/toml.hpp>

#include "Core/ImageManager.hpp"

boost::unordered_flat_map<int32_t, MFCPP::ObstacleConfig::ObstacleData> g_obstaclesData;
void MFCPP::ObstacleConfig::loadFile(const std::filesystem::path& path, const bool force_preload) {
    std::string TOMLFileRaw;
    IO::LoadRaw(TOMLFileRaw, path.string());
    toml::table TOMLFile = toml::parse(TOMLFileRaw);

    toml::array* obstaclesArray = TOMLFile["obstacles"].as_array();
    for (auto& obstacleNode : *obstaclesArray) {
        toml::table* currentObstacle = obstacleNode.as_table();

        const toml::node* idNode = currentObstacle->get("id");
        const toml::node* texXNode = currentObstacle->get("texture_x");
        const toml::node* texYNode = currentObstacle->get("texture_y");
        const toml::node* collisionIdNode = currentObstacle->get("collision_id");
        const toml::node* yLowNode = currentObstacle->get("y_low");
        const toml::node* yHighNode = currentObstacle->get("y_high");

        if (!idNode || !texXNode || !texYNode || !collisionIdNode || !yLowNode || !yHighNode)
            throw std::runtime_error("ObstaclesConfig: Missing required fields in obstacle configuration.");
        int32_t idNodeCheckValue = idNode->value_or(-1);

        if (idNodeCheckValue < 0)
            throw std::runtime_error("ObstaclesConfig: Invalid obstacle ID.");
        auto it = g_obstaclesData.find(idNodeCheckValue);
        if (it != g_obstaclesData.end())
            throw std::runtime_error(fmt::format("ObstaclesConfig: Duplicated obstacle ID {}", idNodeCheckValue));

        auto data = ObstacleData{
            texXNode->value_or(0),
            texYNode->value_or(0),
            collisionIdNode->value_or(0),
            yLowNode->value_or(0),
            yHighNode->value_or(32)
        };
        g_obstaclesData[idNodeCheckValue] = data;
        if (force_preload)
            ImageManager::PreloadTexture(fmt::format("TILE_{}", idNodeCheckValue), "data/resources/Tileset.png", sf::IntRect({data.tex_x, data.tex_y}, {32, 32}));
        else
            ImageManager::AddTexture(fmt::format("TILE_{}", idNodeCheckValue), "data/resources/Tileset.png", sf::IntRect({data.tex_x, data.tex_y}, {32, 32}));
    }
}
MFCPP::ObstacleConfig::ObstacleData MFCPP::ObstacleConfig::getData(int32_t id) {
    auto it = g_obstaclesData.find(id);
    if (it == g_obstaclesData.end())
        throw std::runtime_error("ObstaclesConfig: Obstacle data not found.");
    return it->second;
}