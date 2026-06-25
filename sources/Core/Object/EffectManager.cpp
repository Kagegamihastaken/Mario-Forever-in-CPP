#include "Core/Object/EffectManager.hpp"


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

void EffectManager::updatePreviousData() const {
    for (const auto &i : m_effects) {
        if (i) i->updatePreviousData();
    }
}

void EffectManager::statusUpdate(float deltaTime) const {
    for (const auto &i : m_effects) {
        if (i) i->statusUpdate(deltaTime);
    }
}

void EffectManager::Draw(float alpha) const {
    for (const auto &i : m_effects) {
        if (i) i->draw(alpha);
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

void EffectManager::animationUpdate(float deltaTime) const {
    for (const auto &i : m_effects) {
        if (i) i->animationUpdate(deltaTime);
    }
}

void EffectManager::teleportWithScroll(sf::Vector2f pos) {
    for (auto &i : getScrollBasedList()) {
        i.teleport(pos);
    }
}
