#include "Core/Loading/enum.hpp"
#include "Projectiles/MarioProjectile.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Projectile/MarioFireball.hpp"

void AddMarioProjectile(const bool direction, const MarioProjectileType type, const float x, const float y) {
    GameScene::projectileManager.addProjectile<MarioFireball>(direction, sf::Vector2f(x, y));
}