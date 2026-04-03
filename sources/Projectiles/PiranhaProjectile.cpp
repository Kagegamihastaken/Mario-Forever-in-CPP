#include "Core/Loading/enum.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Projectile/PiranhaFireball.hpp"

void AddPiranhaAIProjectile(const bool direction, const PiranhaDirection piranha_direction, const PiranhaProjectileType type, const float x, const float y) {
    switch (type) {
        case PiranhaProjectileType::PIRANHA_FIREBALL:
            GameScene::projectileManager.addProjectile<PiranhaFireball>(piranha_direction, sf::Vector2f(x, y));
            // it = PiranhaAIProjectileList.emplace(direction, type, piranha_direction, sf::FloatRect({0.f, 0.f}, {15.f, 16.f}), sf::Vector2f(x, y), sf::Vector2f(7, 8));
            // it->setTexture("Fireball", direction);
            break;
        default: ;
    }
}