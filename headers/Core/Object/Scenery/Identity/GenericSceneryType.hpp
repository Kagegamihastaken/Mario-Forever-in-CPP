#ifndef MFCPP_GENERICSCENERYTYPE_HPP
#define MFCPP_GENERICSCENERYTYPE_HPP

#include "SceneryType.hpp"

namespace MFCPP {
    class GenericSceneryType : public SceneryType {
        SceneryTypeEnum getSceneryType() override {
            return GENERIC_SCENERY;
        }
    };
}

#endif //MFCPP_GENERICSCENERYTYPE_HPP
