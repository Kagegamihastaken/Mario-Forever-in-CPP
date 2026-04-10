#ifndef MFCPP_PLATFORMTYPE_HPP
#define MFCPP_PLATFORMTYPE_HPP

#include "MovingBlockType.hpp"

namespace MFCPP {
    class PlatformType : public MovingBlockType {
        MovingBlockEnum getMovingBlockType() override {
            return PLATFORM;
        }
    };
}

#endif //MFCPP_PLATFORMTYPE_HPP
