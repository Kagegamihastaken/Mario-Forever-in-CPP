#ifndef MFCPP_BONUSTYPE_HPP
#define MFCPP_BONUSTYPE_HPP

#include "CustomTileType.hpp"

namespace MFCPP {
    class BonusType : public CustomTileType {
        CustomTileTypeEnum getCustomTileType() override {
            return BONUS;
        }
    };
}

#endif //MFCPP_BONUSTYPE_HPP
