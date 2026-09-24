#include "Core/Loading/BroAILoading.hpp"

#include "Core/Scene/GameScene.hpp"
#include "Object/Enemy/FireBro.hpp"
#include "Object/Enemy/HammerBro.hpp"

void MFCPP::AddBroAI(const BroAIID type, const BroAIMovementType movementType, const float x, const float y) {
    switch (type) {
        case BroAIID::HAMMER_BRO:
            GameScene::enemyManager.addEnemy<HammerBro>(sf::Vector2f(x, y));
            break;
        case BroAIID::FIRE_BRO:
            GameScene::enemyManager.addEnemy<FireBro>(sf::Vector2f(x, y));
            break;
        default: ;
    }
}