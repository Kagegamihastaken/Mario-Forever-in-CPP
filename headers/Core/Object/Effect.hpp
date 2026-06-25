#ifndef MFCPP_EFFECT_HPP
#define MFCPP_EFFECT_HPP

#include <boost/intrusive/list_hook.hpp>
#include "ActiveObject.hpp"

using EffectIntrusiveHook = boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;
class EffectManager;

namespace MFCPP {
    class Effect : public EffectIntrusiveHook {
    public:
        explicit Effect(EffectManager& manager) : m_effectManager(manager) {};

        virtual void updatePreviousData() = 0;
        virtual void statusUpdate(float deltaTime) = 0;
        virtual void draw(float alpha) = 0;
        virtual void animationUpdate(float deltaTime) = 0;
        [[nodiscard]] virtual bool isDestroyed() = 0;
        [[nodiscard]] virtual sf::Vector2f getPosition() = 0;
        [[nodiscard]] virtual sf::Vector2f getOrigin() = 0;
        [[nodiscard]] virtual sf::FloatRect getHitbox() = 0;
        virtual void teleport(sf::Vector2f val) = 0;
        virtual ~Effect() = default;
    protected:
        EffectManager& m_effectManager;
    };
}

#endif //MFCPP_EFFECT_HPP
