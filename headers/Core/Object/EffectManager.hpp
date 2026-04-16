#ifndef MFCPP_EFFECTMANAGER_HPP
#define MFCPP_EFFECTMANAGER_HPP

#include <memory>
#include <vector>
#include <boost/intrusive/list.hpp>
#include "Effect.hpp"
#include "Core/Exception.hpp"
#include "Effect/Identity/GenericEffectType.hpp"

class EffectManager {
public:
    using EffectIntrusiveList = boost::intrusive::list<MFCPP::Effect, boost::intrusive::constant_time_size<false>>;
    EffectManager();
    template <typename T, typename... Args>
    void addEffect(Args&&... args) {
        auto newEffect = std::make_unique<T>(*this, std::forward<Args>(args)...);

        if constexpr (std::is_base_of_v<MFCPP::GenericEffectType, T>)
            m_GenericEffect.push_back(*newEffect);
        else throw MFCPP::Exception::WhichIdentity(fmt::format("Class {} doesn't have any identity.", typeid(*newEffect).name()));
        m_effects.push_back(std::move(newEffect));
    }

    [[nodiscard]] EffectIntrusiveList& getGenericList();
    void setDeletionFlag(bool val);
    [[nodiscard]] bool getDeletionFlag() const;
    void setPreviousData() const;
    void interpolateData(float alpha) const;
    void statusUpdate(float deltaTime) const;
    void DeleteAll();
    void Draw() const;
    void EffectCleanup();

private:
    EffectIntrusiveList m_GenericEffect;
    std::vector<std::unique_ptr<MFCPP::Effect>> m_effects;
    bool m_EffectDeletionFlag = false;
};

#endif //MFCPP_EFFECTMANAGER_HPP
