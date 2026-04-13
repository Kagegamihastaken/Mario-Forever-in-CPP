#include "Object/SceneryHelper.hpp"

#include "Core/Scene/GameScene.hpp"
#include "Object/Scenery/BlueCloud.hpp"
#include "Object/Scenery/BlueGrass.hpp"

std::vector<std::string> BlueCloudAnimName;
static constexpr int CLOUD_IMAGE_WIDTH = 192;
static constexpr int CLOUD_WIDTH = 64;
static constexpr int CLOUD_HEIGHT = 64;
std::vector<std::string> BlueGrassAnimName;
static constexpr int GRASS_IMAGE_WIDTH = 192;
static constexpr int GRASS_WIDTH = 64;
static constexpr int GRASS_HEIGHT = 32;
void SceneryInit() {
    for (int i = 0; i < CLOUD_IMAGE_WIDTH / CLOUD_WIDTH; i++) {
        ImageManager::AddTexture(fmt::format("BlueCloud_{}", i), "data/resources/Scenery/BlueCloud.png", sf::IntRect({i * CLOUD_WIDTH, 0}, {CLOUD_WIDTH, CLOUD_HEIGHT}));
        BlueCloudAnimName.push_back(fmt::format("BlueCloud_{}", i));
    }
    for (int i = 0; i < GRASS_IMAGE_WIDTH / GRASS_WIDTH; i++) {
        ImageManager::AddTexture(fmt::format("BlueGrass_{}", i), "data/resources/Scenery/BlueGrass.png", sf::IntRect({i * GRASS_WIDTH, 0}, {GRASS_WIDTH, GRASS_HEIGHT}));
        BlueGrassAnimName.push_back(fmt::format("BlueGrass_{}", i));
    }
}
void AddScenery(const int id, const sf::Vector2f& pos) {
    switch (id) {
        case 0:
            GameScene::sceneryManager.addScenery<BlueCloud>(pos);
            break;
        case 1:
            GameScene::sceneryManager.addScenery<BlueGrass>(pos);
            break;
        default: ;
    }
}