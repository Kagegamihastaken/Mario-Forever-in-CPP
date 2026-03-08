#ifndef MFCPP_SOLIDTYPE_HPP
#define MFCPP_SOLIDTYPE_HPP

#include "CustomTileType.hpp"

namespace MFCPP {
    class SolidType : public CustomTileType {
        CustomTileTypeEnum getCustomTileType() override {
            return SOLID;
        }
    };
}

#endif //MFCPP_SOLIDTYPE_HPP
