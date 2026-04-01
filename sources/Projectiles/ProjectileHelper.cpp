#include "Projectiles/ProjectileHelper.hpp"
#include "Core/ImageManager.hpp"

void ProjectileInit() {
    ImageManager::AddTexture("Hammer", "data/resources/Projectiles/Hammer.png");
    ImageManager::AddTexture("Fireball", "data/resources/Projectiles/Fireball.png");
    ImageManager::AddTexture("Beetroot_Projectile", "data/resources/Projectiles/Beetroot_Projectile.png");
}
