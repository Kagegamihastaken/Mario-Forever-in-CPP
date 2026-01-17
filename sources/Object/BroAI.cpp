#include "Object/BroAI.hpp"
#include "Core/ImageManager.hpp"
#include "Core/ExternalHeaders/plf_colony.h"
#include "Core/Scene/GameScene.hpp"
#include "Object/Enemy/FireBro.hpp"
#include "Object/Enemy/HammerBro.hpp"

std::vector<std::string> HammerBroAnimName;
std::vector<std::string> HammerBroLaunchAnimName;
std::vector<std::string> FireBroAnimName;
std::vector<std::string> FireBroLaunchAnimName;
static constexpr int HAMMER_BRO_IMAGE_WIDTH = 96;
static constexpr int HAMMER_BRO_WIDTH = 48;
static constexpr int HAMMER_BRO_HEIGHT = 64;

void BroAILoadRes() {
    for (int i = 0; i < HAMMER_BRO_IMAGE_WIDTH / HAMMER_BRO_WIDTH; i++) {
        ImageManager::AddTexture(fmt::format("HammerBro_{}", i), "data/resources/Bro/HammerBro.png", sf::IntRect({i * HAMMER_BRO_WIDTH, 0}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}));
        HammerBroAnimName.push_back(fmt::format("HammerBro_{}", i));
        ImageManager::AddTexture(fmt::format("FireBro_{}", i), "data/resources/Bro/FireBro.png", sf::IntRect({i * HAMMER_BRO_WIDTH, 0}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}));
        FireBroAnimName.push_back(fmt::format("FireBro_{}", i));

        ImageManager::AddTexture(fmt::format("HammerBroLaunch_{}", i), "data/resources/Bro/HammerBro.png", sf::IntRect({i * HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}));
        HammerBroLaunchAnimName.push_back(fmt::format("HammerBroLaunch_{}", i));
        ImageManager::AddTexture(fmt::format("FireBroLaunch_{}", i), "data/resources/Bro/FireBro.png", sf::IntRect({i * HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}));
        FireBroLaunchAnimName.push_back(fmt::format("FireBroLaunch_{}", i));
    }
}
void AddBroAI(const BroAIType type, const BroAIMovementType movementType, const float x, const float y) {
    switch (type) {
        case BroAIType::HAMMER_BRO:
            GameScene::enemyManager.addEnemy<HammerBro>(sf::Vector2f(x, y));
            break;
        case BroAIType::FIRE_BRO:
            GameScene::enemyManager.addEnemy<FireBro>(sf::Vector2f(x, y));
            break;
        default: ;
    }
}