#include "Object/BroAI.hpp"

#include "Core/AnimationSequenceManager.hpp"
#include "Core/ImageManager.hpp"
#include "Core/ExternalHeaders/plf_colony.h"
#include "Core/Scene/GameScene.hpp"
#include "Object/Enemy/FireBro.hpp"
#include "Object/Enemy/HammerBro.hpp"

static constexpr int HAMMER_BRO_IMAGE_WIDTH = 96;
static constexpr int HAMMER_BRO_WIDTH = 48;
static constexpr int HAMMER_BRO_HEIGHT = 64;

void BroAILoadRes() {
    MFCPP::AnimationSequenceManager::newData("HammerBroAnimName");
    MFCPP::AnimationSequenceManager::newData("FireBroAnimName");
    MFCPP::AnimationSequenceManager::newData("HammerBroLaunchAnimName");
    MFCPP::AnimationSequenceManager::newData("FireBroLaunchAnimName");
    for (int i = 0; i < HAMMER_BRO_IMAGE_WIDTH / HAMMER_BRO_WIDTH; i++) {
        ImageManager::AddTexture(fmt::format("HammerBro_{}", i), "data/resources/Bro/HammerBro.png", sf::IntRect({i * HAMMER_BRO_WIDTH, 0}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}));
        MFCPP::AnimationSequenceManager::addSingleFrame("HammerBroAnimName", fmt::format("HammerBro_{}", i));
        ImageManager::AddTexture(fmt::format("FireBro_{}", i), "data/resources/Bro/FireBro.png", sf::IntRect({i * HAMMER_BRO_WIDTH, 0}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}));
        MFCPP::AnimationSequenceManager::addSingleFrame("FireBroAnimName", fmt::format("FireBro_{}", i));

        ImageManager::AddTexture(fmt::format("HammerBroLaunch_{}", i), "data/resources/Bro/HammerBro.png", sf::IntRect({i * HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}));
        MFCPP::AnimationSequenceManager::addSingleFrame("HammerBroLaunchAnimName", fmt::format("HammerBroLaunch_{}", i));
        ImageManager::AddTexture(fmt::format("FireBroLaunch_{}", i), "data/resources/Bro/FireBro.png", sf::IntRect({i * HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}, {HAMMER_BRO_WIDTH, HAMMER_BRO_HEIGHT}));
        MFCPP::AnimationSequenceManager::addSingleFrame("FireBroLaunchAnimName", fmt::format("FireBroLaunch_{}", i));
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