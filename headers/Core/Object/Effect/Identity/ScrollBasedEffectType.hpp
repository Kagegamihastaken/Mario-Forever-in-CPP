//
// Created by dovie on 4/18/2026.
//

#ifndef MFCPP_SCROLLBASEDEFFECTTYPE_HPP
#define MFCPP_SCROLLBASEDEFFECTTYPE_HPP
#include "EffectType.hpp"

namespace MFCPP {
    class ScrollBasedEffectType : public EffectType {
        EffectTypeEnum getEffectType() override {
            return SCROLL_BASED_EFFECT;
        }
    };
}

#endif //MFCPP_SCROLLBASEDEFFECTTYPE_HPP
