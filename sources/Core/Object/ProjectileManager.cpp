#include "Core/Object/ProjectileManager.hpp"

ProjectileManager::ProjectileManager() {}

void ProjectileManager::setPreviousData() const {
    for (const auto &i : m_projectile) {
        if (i) i->setPreviousData();
    }
}

void ProjectileManager::interpolateData(float alpha) const {
    for (const auto &i : m_projectile) {
        if (i) i->interpolateData(alpha);
    }
}

void ProjectileManager::CollisionUpdate() const {
    for (const auto &i : m_projectile) {
        if (i) i->CollisionUpdate();
    }
}

void ProjectileManager::statusUpdate(float deltaTime) const {
    for (const auto &i : m_projectile) {
        if (i) i->statusUpdate(deltaTime);
    }
}

void ProjectileManager::Draw() const {
    for (const auto &i : m_projectile) {
        if (i) i->draw();
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


