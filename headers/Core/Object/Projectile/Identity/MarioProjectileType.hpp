#ifndef MFCPP_MARIOPROJECTILETYPE_HPP
#define MFCPP_MARIOPROJECTILETYPE_HPP

#include "ProjectileType.hpp"

namespace MFCPP {
    class MarioProjectileType : public ProjectileType {
        ProjectileTypeEnum getProjectileType() override {
            return MARIO_PROJECTILE;
        }
    };
}

#endif //MFCPP_MARIOPROJECTILETYPE_HPP