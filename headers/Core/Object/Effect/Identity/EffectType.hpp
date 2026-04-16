#ifndef MFCPP_EFFECTTYPE_HPP
#define MFCPP_EFFECTTYPE_HPP

namespace MFCPP {
    enum EffectTypeEnum {GENERIC_EFFECT, SCROLL_BASED_EFFECT};

    class EffectType {
    public:
        EffectType() = default;
        virtual EffectTypeEnum getEffectType() = 0;
        virtual ~EffectType() = default;
    };
}

#endif //MFCPP_EFFECTTYPE_HPP
