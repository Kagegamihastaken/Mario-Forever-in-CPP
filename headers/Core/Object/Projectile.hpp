#ifndef MFCPP_PROJECTILE_HPP
#define MFCPP_PROJECTILE_HPP
#include <boost/intrusive/list_hook.hpp>
#include "Core/Class/ActiveObjectClass.hpp"

using ProjectileIntrusiveHook = boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;
class ProjectileManager;

namespace MFCPP {
    class Projectile : public ProjectileIntrusiveHook, public ActiveObject<float> {
    public:
        explicit Projectile(ProjectileManager& manager) : m_manager(manager) {};

        virtual void setPreviousData() = 0;
        virtual void interpolateData(float alpha) = 0;
        virtual void statusUpdate(float deltaTime) = 0;
        virtual void CollisionUpdate() = 0;
        virtual void LevelEndCleanup() = 0;
        virtual void draw() = 0;
    protected:
        ProjectileManager& m_manager;
    };
}

#endif //MFCPP_PROJECTILE_HPP