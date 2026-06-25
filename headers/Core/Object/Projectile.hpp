#ifndef MFCPP_PROJECTILE_HPP
#define MFCPP_PROJECTILE_HPP
#include <boost/intrusive/list_hook.hpp>
#include "ActiveObject.hpp"
#include <bitset>

using ProjectileIntrusiveHook = boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;
class ProjectileManager;

namespace MFCPP {
    class Projectile : public ProjectileIntrusiveHook {
        enum Flag : uint8_t {
            DrawingPriorityLow = 0,
            DrawingPriorityHigh = 1
        };
    public:
        explicit Projectile(ProjectileManager& manager) : m_manager(manager) {};

        virtual void updatePreviousData() = 0;
        virtual void statusUpdate(float deltaTime) = 0;
        virtual void CollisionUpdate() = 0;
        virtual void LevelEndCleanup() = 0;
        virtual void draw(float alpha) = 0;
        virtual void animationUpdate(float deltaTime) = 0;
        [[nodiscard]] virtual bool isDestroyed() = 0;

        [[nodiscard]] virtual sf::Vector2f getPosition() = 0;
        [[nodiscard]] virtual sf::Vector2f getOrigin() = 0;
        [[nodiscard]] virtual sf::FloatRect getHitbox() = 0;

        [[nodiscard]] int getDrawingPriority() const {
            return m_option[DrawingPriorityLow] + (m_option[DrawingPriorityHigh] << 1);
        }
        virtual ~Projectile() = default;
    protected:
        void setDrawingPriority(const int val) {
            m_option[DrawingPriorityLow] = val & 1;
            m_option[DrawingPriorityHigh] = (val >> 1) & 1;
        }
        ProjectileManager& m_manager;
        std::bitset<2> m_option = 0;
    };
}

#endif //MFCPP_PROJECTILE_HPP