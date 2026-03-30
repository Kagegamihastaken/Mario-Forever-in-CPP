#ifndef MFCPP_ENEMYPROJECTILETYPE_HPP
#define MFCPP_ENEMYPROJECTILETYPE_HPP

#include "ProjectileType.hpp"

namespace MFCPP {
    class EnemyProjectileType : public ProjectileType {
        ProjectileTypeEnum getProjectileType() override {
            return ENEMY_PROJECTILE;
        }
    };
}

#endif //MFCPP_ENEMYPROJECTILETYPE_HPP