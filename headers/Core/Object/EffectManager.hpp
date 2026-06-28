#ifndef MFCPP_EFFECTMANAGER_HPP
#define MFCPP_EFFECTMANAGER_HPP

#include <memory>
#include <boost/intrusive/list.hpp>
#include <boost/container/small_vector.hpp>
#include "Effect.hpp"
#include "Core/Exception.hpp"
#include "Effect/Identity/GenericEffectType.hpp"
#include "Effect/Identity/ScrollBasedEffectType.hpp"

class EffectManager {
public:
    using EffectIntrusiveList = boost::intrusive::list<MFCPP::Effect, boost::intrusive::constant_time_size<false>>;
    EffectManager() = default;
    template <typename T, typename... Args>
    void addEffect(Args&&... args) {
        auto newEffect = std::make_unique<T>(*this, std::forward<Args>(args)...);

        if constexpr (std::is_base_of_v<MFCPP::GenericEffectType, T>)
            m_GenericEffect.push_back(*newEffect);
        else if constexpr (std::is_base_of_v<MFCPP::ScrollBasedEffectType, T>)
            m_ScrollBasedEffect.push_back(*newEffect);
        else throw MFCPP::Exception::WhichIdentity(fmt::format("Class {} doesn't have any identity.", typeid(*newEffect).name()));
        m_effects.emplace_back(std::move(newEffect));
    }

    [[nodiscard]] EffectIntrusiveList& getGenericList();
    [[nodiscard]] EffectIntrusiveList& getScrollBasedList();
    void setDeletionFlag(bool val);
    [[nodiscard]] bool getDeletionFlag() const;
    void updatePreviousData() const;
    void statusUpdate(float deltaTime) const;
    void DeleteAll();
    void Draw(float alpha) const;
    void EffectCleanup();
    void animationUpdate(float deltaTime) const;

    //Tank Autoscroll
    void teleportWithScroll(sf::Vector2f pos);

private:
    EffectIntrusiveList m_GenericEffect;
    EffectIntrusiveList m_ScrollBasedEffect;
    boost::container::small_vector<std::unique_ptr<MFCPP::Effect>, 256> m_effects;
    bool m_EffectDeletionFlag = false;
};

#endif //MFCPP_EFFECTMANAGER_HPP
