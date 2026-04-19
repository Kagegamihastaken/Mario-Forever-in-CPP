#include "Core/Object/CustomTileManager.hpp"

#include "Core/Tilemap.hpp"

CustomTileManager::CustomTileManager() {}
void CustomTileManager::setPreviousData() const {
    for (const auto &i : m_customTiles) {
        if (i) i->setPreviousData();
    }
}

void CustomTileManager::interpolateData(float alpha) const {
    for (const auto &i : m_customTiles) {
        if (i) i->interpolateData(alpha);
    }
}

void CustomTileManager::HitEvent() const {
    for (const auto &i : m_customTiles) {
        if (i) i->HitEvent();
    }
}

void CustomTileManager::statusUpdate(float deltaTime) const {
    for (const auto &i : m_customTiles) {
        if (i) i->statusUpdate(deltaTime);
    }
}

void CustomTileManager::setCollision(const sf::Vector2f &pos, bool val) {
    m_queue_data.push(QueueCustomTileData(pos, val));
}

void CustomTileManager::updateCollision() {
    while (!m_queue_data.empty()) {
        MFCPP::setIndexTilemapCollision(m_queue_data.front().pos.x, m_queue_data.front().pos.y, m_queue_data.front().val);
        m_queue_data.pop();
    }
}

CustomTileManager::CustomTileIntrusiveList &CustomTileManager::getBonusList() {
    return m_Bonus;
}
CustomTileManager::CustomTileIntrusiveList &CustomTileManager::getSolidList() {
    return m_Solid;
}

void CustomTileManager::DeleteAll() {
    m_customTiles.clear();
}

bool CustomTileManager::getDeletionFlag() const {
    return m_CustomTileDeletionFlag;
}

void CustomTileManager::setDeletionFlag(bool val) {
    m_CustomTileDeletionFlag = val;
}

void CustomTileManager::DrawPriority(const int index) const {
    for (const auto &i : m_customTiles) {
        if (i && i->getDrawingPriority() == index) i->draw();
    }
}

void CustomTileManager::CustomTileCleanup() {
    if (!getDeletionFlag()) return;
    auto it = m_customTiles.begin();
    while (it != m_customTiles.end()) {
        if (!it->get()->isDestroyed()) ++it;
        else it = m_customTiles.erase(it);
    }
    setDeletionFlag(false);
}

