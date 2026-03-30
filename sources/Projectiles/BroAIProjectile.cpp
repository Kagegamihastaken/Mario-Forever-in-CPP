#include "Core/Loading/enum.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Projectile/BroFireball.hpp"
#include "Object/Projectile/BroHammer.hpp"

void AddBroAIProjectile(const bool direction, const BroAIProjectileType type, const float x, const float y) {
    switch (type) {
        case BroAIProjectileType::BROAI_HAMMER:
            GameScene::projectileManager.addProjectile<BroHammer>(direction, sf::Vector2f(x, y));
            break;
        case BroAIProjectileType::BROAI_FIREBALL:
            GameScene::projectileManager.addProjectile<BroFireball>(direction, sf::Vector2f(x, y));
            break;
        default: ;
    }
}