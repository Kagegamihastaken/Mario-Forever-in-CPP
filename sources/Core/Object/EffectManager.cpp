#include "Core/Object/EffectManager.hpp"

EffectManager::EffectManager() {}

bool EffectManager::getDeletionFlag() const {
    return m_EffectDeletionFlag;
}

void EffectManager::setDeletionFlag(bool val) {
    m_EffectDeletionFlag = val;
}

EffectManager::EffectIntrusiveList &EffectManager::getGenericList() {
    return m_GenericEffect;
}

EffectManager::EffectIntrusiveList &EffectManager::getScrollBasedList() {
    return m_ScrollBasedEffect;
}

void EffectManager::setPreviousData() const {
    for (const auto &i : m_effects) {
        if (i) i->setPreviousData();
    }
}

void EffectManager::interpolateData(float alpha) const {
    for (const auto &i : m_effects) {
        if (i) i->interpolateData(alpha);
    }
}

void EffectManager::statusUpdate(float deltaTime) const {
    for (const auto &i : m_effects) {
        if (i) i->statusUpdate(deltaTime);
    }
}

void EffectManager::Draw() const {
    for (const auto &i : m_effects) {
        if (i) i->draw();
    }
}

void EffectManager::DeleteAll() {
    m_effects.clear();
}

void EffectManager::EffectCleanup() {
    if (!getDeletionFlag()) return;
    auto it = m_effects.begin();
    while (it != m_effects.end()) {
        if (!it->get()->isDestroyed()) ++it;
        else it = m_effects.erase(it);
    }
    setDeletionFlag(false);
}
