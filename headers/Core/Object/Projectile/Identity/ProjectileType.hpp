#ifndef MFCPP_PROJECTILETYPE_HPP
#define MFCPP_PROJECTILETYPE_HPP

namespace MFCPP {
    enum ProjectileTypeEnum {ENEMY_PROJECTILE, MARIO_PROJECTILE};

    class ProjectileType {
    public:
        ProjectileType() = default;
        virtual ProjectileTypeEnum getProjectileType() = 0;
        virtual ~ProjectileType() = default;
    };
}

#endif //MFCPP_PROJECTILETYPE_HPP