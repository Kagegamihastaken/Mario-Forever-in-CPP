#include "Core/Object/EnemyManager.hpp"

#include <queue>

#include "Core/Logging.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/Mario.hpp"

EnemyManager::EnemyManager(){}
void EnemyManager::setPreviousData() const {
    for (const auto &i : m_enemies) {
        if (i) i->setPreviousData();
    }
}

void EnemyManager::interpolateData(const float alpha) const {
    for (const auto &i : m_enemies) {
        if (i) i->interpolateData(alpha);
    }
}

void EnemyManager::XUpdate(float deltaTime) const {
    for (const auto &i : m_enemies) {
        if (i) i->XUpdate(deltaTime);
    }
}

void EnemyManager::YUpdate(float deltaTime) const {
    for (const auto &i : m_enemies) {
        if (i) i->YUpdate(deltaTime);
    }
}

void EnemyManager::statusUpdate(const float deltaTime) const {
    for (const auto &i : m_enemies) {
        if (i) i->statusUpdate(deltaTime);
    }
}

void EnemyManager::EnemyCollision() {
    // GoombaAI-Only Collision
    for (auto it = m_GoombaAI.begin(); it != m_GoombaAI.end(); ++it) {
        MFCPP::Enemy& enemyFirst = *it;
        enemyFirst.EnemyCollision();
    }
}

void EnemyManager::MarioCollision() const {
    if (EffectActive) return;
    const float MarioYVelocityBefore = Yvelo;
    for (const auto &i : m_enemies) {
        if (i)
            i->MarioCollision(MarioYVelocityBefore);
    }
}
void EnemyManager::DrawHighPriority() const {
    for (auto &i : m_enemies) {
        if (i && !i->isDrawingLowerPriority()) i->draw();
    }
}
void EnemyManager::DrawLowPriority() const {
    for (auto &i : m_enemies) {
        if (i && i->isDrawingLowerPriority()) i->draw();
    }
}
EnemyManager::EnemyIntrusiveList &EnemyManager::getGoombaAIList() {
    return m_GoombaAI;
}

EnemyManager::EnemyIntrusiveList &EnemyManager::getBroAIList() {
    return m_BroAI;
}

void EnemyManager::DeleteAll() {
    m_enemies.clear();
}

bool EnemyManager::getDeletionFlag() const {
    return m_EnemyDeletionFlag;
}

void EnemyManager::setDeletionFlag(const bool val) {
    m_EnemyDeletionFlag = val;
}
void EnemyManager::EnemyCleanup() {
    if (!getDeletionFlag()) return;
    auto it = m_enemies.begin();
    while (it != m_enemies.end()) {
        if (!it->get()->isDestroyed()) ++it;
        else it = m_enemies.erase(it);
    }
    setDeletionFlag(false);
}
