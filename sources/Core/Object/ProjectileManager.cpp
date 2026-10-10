#include "Core/Object/ProjectileManager.hpp"
#include "Core/Profiler.hpp"

void ProjectileManager::updatePreviousData() const {
    ZoneScopedNC("ProjectileManager::updatePreviousData", 0xC90387);
    for (const auto &i : m_projectile) {
        if (i) i->updatePreviousData();
    }
}

void ProjectileManager::CollisionUpdate() const {
    ZoneScopedNC("ProjectileManager::CollisionUpdate", 0xC90387);
    for (const auto &i : m_projectile) {
        if (i) i->CollisionUpdate();
    }
}

void ProjectileManager::statusUpdate(float deltaTime) const {
    ZoneScopedNC("ProjectileManager::statusUpdate", 0xC90387);
    for (const auto &i : m_projectile) {
        if (i) i->statusUpdate(deltaTime);
    }
}

void ProjectileManager::DrawPriority(int index, float alpha) const {
    ZoneScopedNC("ProjectileManager::DrawPriority", 0xC90387);
    for (const auto &i : m_projectile) {
        if (i && i->getDrawingPriority() == index) i->draw(alpha);
    }
}

void ProjectileManager::DeleteAll() {
    m_projectile.clear();
}

ProjectileManager::ProjectileIntrusiveList &ProjectileManager::getMarioProjectileList() {
    return m_Mario;
}
ProjectileManager::ProjectileIntrusiveList &ProjectileManager::getEnemyProjectileList() {
    return m_Enemy;
}

void ProjectileManager::setDeletionFlag(bool val) {
    m_ProjectileDeletionFlag = val;
}

bool ProjectileManager::getDeletionFlag() const {
    return m_ProjectileDeletionFlag;
}

void ProjectileManager::ProjectileCleanup() {
    if (!getDeletionFlag()) return;
    auto it = m_projectile.begin();
    while (it != m_projectile.end()) {
        if (!it->get()->isDestroyed()) ++it;
        else it = m_projectile.erase(it);
    }
    setDeletionFlag(false);
}

void ProjectileManager::animationUpdate(float deltaTime) const {
    ZoneScopedNC("ProjectileManager::animationUpdate", 0xC90387);
    for (const auto &i : m_projectile) {
        if (i) i->animationUpdate(deltaTime);
    }
}


