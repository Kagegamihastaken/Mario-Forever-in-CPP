#include "Core/Object/MovingBlockManager.hpp"

MovingBlockManager::MovingBlockManager() {}

bool MovingBlockManager::getDeletionFlag() const {
    return m_MovingBLockDeletionFlag;
}

void MovingBlockManager::setDeletionFlag(bool val) {
    m_MovingBLockDeletionFlag = val;
}

void MovingBlockManager::setPreviousData() const {
    for (const auto &i : m_movingblocks) {
        if (i) i->setPreviousData();
    }
}

void MovingBlockManager::interpolateData(float alpha) const {
    for (const auto &i : m_movingblocks) {
        if (i) i->interpolateData(alpha);
    }
}

void MovingBlockManager::statusUpdate(float deltaTime) const {
    for (const auto &i : m_movingblocks) {
        if (i) i->statusUpdate(deltaTime);
    }
}

void MovingBlockManager::draw() const {
    for (const auto &i : m_movingblocks) {
        if (i) i->draw();
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
