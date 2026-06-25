#ifndef MFCPP_SCENERY_HPP
#define MFCPP_SCENERY_HPP

#include <boost/intrusive/list_hook.hpp>
#include "ActiveObject.hpp"

using SceneryIntrusiveHook = boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;
class SceneryManager;

namespace MFCPP {
    class Scenery : public SceneryIntrusiveHook {
    public:
        explicit Scenery(SceneryManager& manager) : m_manager(manager) {};
        virtual void updatePreviousData() = 0;
        virtual void draw(float alpha) = 0;
        virtual void animationUpdate(float deltaTime) = 0;
        [[nodiscard]] virtual bool isDestroyed() = 0;
        [[nodiscard]] virtual sf::Vector2f getPosition() = 0;
        [[nodiscard]] virtual sf::Vector2f getOrigin() = 0;
        [[nodiscard]] virtual sf::FloatRect getHitbox() = 0;
        virtual ~Scenery() = default;
    protected:
        SceneryManager& m_manager;
    };
}

#endif //MFCPP_SCENERY_HPP
