#ifndef MFCPP_PROJECTILEMANAGER_HPP
#define MFCPP_PROJECTILEMANAGER_HPP
#include <memory>
#include <vector>
#include <boost/intrusive/list.hpp>
#include "Projectile.hpp"
#include "Core/Exception.hpp"
#include "fmt/format.h"
#include "Projectile/Identity/EnemyProjectileType.hpp"
#include "Projectile/Identity/MarioProjectileType.hpp"
#include "Core/ExternalHeaders/plf_colony.h"

class ProjectileManager {
public:
    using ProjectileIntrusiveList = boost::intrusive::list<MFCPP::Projectile, boost::intrusive::constant_time_size<false>>;
    ProjectileManager();
    template <typename T, typename... Args>
    void addProjectile(Args&&... args) {
        auto newProjectile = std::make_unique<T>(*this, std::forward<Args>(args)...);

        if constexpr (std::is_base_of_v<MFCPP::MarioProjectileType, T>)
            m_Mario.push_back(*newProjectile);
        else if constexpr (std::is_base_of_v<MFCPP::EnemyProjectileType, T>)
            m_Enemy.push_back(*newProjectile);
        else throw MFCPP::Exception::WhichIdentity(fmt::format("Class {} doesn't have any identity.", typeid(*newProjectile).name()));
        m_projectile.emplace_back(std::move(newProjectile));
    }

    [[nodiscard]] ProjectileIntrusiveList& getMarioProjectileList();
    [[nodiscard]] ProjectileIntrusiveList& getEnemyProjectileList();

    void setDeletionFlag(bool val);
    bool getDeletionFlag() const;
    void setPreviousData() const;
    void interpolateData(float alpha) const;
    void statusUpdate(float deltaTime) const;
    void CollisionUpdate() const;
    void ProjectileCleanup();
    void DeleteAll();
    void DrawPriority(int index) const;
private:
    ProjectileIntrusiveList m_Mario;
    ProjectileIntrusiveList m_Enemy;
    std::vector<std::unique_ptr<MFCPP::Projectile>> m_projectile;
    bool m_ProjectileDeletionFlag = false;
};

#endif //MFCPP_PROJECTILEMANAGER_HPP