#ifndef MFCPP_MOVINGBLOCK_HPP
#define MFCPP_MOVINGBLOCK_HPP

#include <bitset>
#include <boost/intrusive/list_hook.hpp>
#include "ActiveObject.hpp"

using MovingBlockIntrusiveHook = boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;
class MovingBlockManager;

namespace MFCPP {
    class MovingBlock : public MovingBlockIntrusiveHook {
        enum Flag : uint8_t {
            CanCollision = 0
        };
    public:
        explicit MovingBlock(MovingBlockManager& manager) : m_movingBlockManager(manager) {};

        virtual void updatePreviousData() = 0;
        virtual void statusUpdate(float deltaTime) = 0;
        virtual void activate() = 0;
        virtual void draw(float alpha) = 0;
        virtual void animationUpdate(float deltaTime) = 0;
        [[nodiscard]] virtual bool isDestroyed() = 0;

        [[nodiscard]] virtual sf::Vector2f getPosition() = 0;
        [[nodiscard]] virtual sf::Vector2f getPreviousPosition() = 0;
        [[nodiscard]] virtual sf::Vector2f getOrigin() = 0;
        [[nodiscard]] virtual sf::FloatRect getHitbox() = 0;

        [[nodiscard]] bool canCollision() const { return m_option[CanCollision]; }

        virtual ~MovingBlock() = default;
    protected:
        void setCanCollision(const bool val) { m_option[CanCollision] = val;}
        std::bitset<1> m_option;
        MovingBlockManager& m_movingBlockManager;
    };
}

#endif //MFCPP_MOVINGBLOCK_HPP