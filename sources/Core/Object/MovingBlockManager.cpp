#include "Core/Object/MovingBlockManager.hpp"
#include "Core/Profiler.hpp"

bool MovingBlockManager::getDeletionFlag() const {
    return m_MovingBLockDeletionFlag;
}

void MovingBlockManager::setDeletionFlag(bool val) {
    m_MovingBLockDeletionFlag = val;
}

void MovingBlockManager::updatePreviousData() const {
    for (const auto &i : m_movingblocks) {
        if (i) i->updatePreviousData();
    }
}

void MovingBlockManager::statusUpdate(float deltaTime) const {
    for (const auto &i : m_movingblocks) {
        if (i) i->statusUpdate(deltaTime);
    }
}

void MovingBlockManager::draw(float alpha) const {
    ZoneScopedNC("MovingBlockManager::draw", 0x73ded0);
    for (const auto &i : m_movingblocks) {
        if (i) i->draw(alpha);
    }
}

void MovingBlockManager::MovingBlockCleanup() {
    if (!getDeletionFlag()) return;
    auto it = m_movingblocks.begin();
    while (it != m_movingblocks.end()) {
        if (!it->get()->isDestroyed()) ++it;
        else it = m_movingblocks.erase(it);
    }
    setDeletionFlag(false);
}

void MovingBlockManager::DeleteAll() {
    m_movingblocks.clear();
}

MovingBlockManager::MovingBlockIntrusiveList &MovingBlockManager::getPlatformList() {
    return m_platforms;
}

void MovingBlockManager::activate() const {
    for (const auto &i : m_movingblocks) {
        if (i) i->activate();
    }
}

void MovingBlockManager::animationUpdate(float deltaTime) const {
    for (const auto &i : m_movingblocks) {
        if (i) i->animationUpdate(deltaTime);
    }
}
