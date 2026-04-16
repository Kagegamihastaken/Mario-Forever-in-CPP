#ifndef MFCPP_EFFECT_HPP
#define MFCPP_EFFECT_HPP

#include <boost/intrusive/list_hook.hpp>
#include "Core/Class/ActiveObjectClass.hpp"

using EffectIntrusiveHook = boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;
class EffectManager;

namespace MFCPP {
    class Effect : public EffectIntrusiveHook, public ActiveObject<float> {
    public:
        explicit Effect(EffectManager& manager) : m_effectManager(manager) {};

        virtual void setPreviousData() = 0;
        virtual void interpolateData(float alpha) = 0;
        virtual void statusUpdate(float deltaTime) = 0;
        virtual void draw() = 0;
    protected:
        EffectManager& m_effectManager;
    };
}

#endif //MFCPP_EFFECT_HPP
