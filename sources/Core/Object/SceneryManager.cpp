#include "Core/Object/SceneryManager.hpp"

SceneryManager::SceneryManager() {}

bool SceneryManager::getDeletionFlag() const {
    return m_SceneryDeletionFlag;
}

SceneryManager::SceneryIntrusiveList &SceneryManager::getGenericScenery() {
    return m_generic_scenery;
}

void SceneryManager::SceneryCleanup() {
    if (!getDeletionFlag()) return;
    auto it = m_scenery.begin();
    while (it != m_scenery.end()) {
        if (!it->get()->isDestroyed()) ++it;
        else it = m_scenery.erase(it);
    }
    setDeletionFlag(false);
}

void SceneryManager::setPreviousData() const {
    for (const auto &i : m_scenery) {
        if (i) i->setPreviousData();
    }
}

void SceneryManager::interpolateData(float alpha) const {
    for (const auto &i : m_scenery) {
        if (i) i->interpolateData(alpha);
    }
}

void SceneryManager::DeleteAll() {
    m_scenery.clear();
}

void SceneryManager::setDeletionFlag(bool val) {
    m_SceneryDeletionFlag = val;
}

void SceneryManager::draw() const {
    for (const auto &i : m_scenery) {
        if (i) i->draw();
    }
}
