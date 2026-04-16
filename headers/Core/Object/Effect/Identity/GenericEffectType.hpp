#ifndef MFCPP_GENERICEFFECTTYPE_HPP
#define MFCPP_GENERICEFFECTTYPE_HPP
#include "EffectType.hpp"

namespace MFCPP {
    class GenericEffectType : public EffectType {
        EffectTypeEnum getEffectType() override {
            return GENERIC_EFFECT;
        }
    };
}

#endif //MFCPP_GENERICEFFECTTYPE_HPP
