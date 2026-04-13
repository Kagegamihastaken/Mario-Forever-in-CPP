#ifndef MFCPP_MOVINGBLOCK_HPP
#define MFCPP_MOVINGBLOCK_HPP

#include <boost/intrusive/list_hook.hpp>
#include "Core/Class/ActiveObjectClass.hpp"

using MovingBlockIntrusiveHook = boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;
class MovingBlockManager;

namespace MFCPP {
    class MovingBlock : public MovingBlockIntrusiveHook, public ActiveObject<float> {
    public:
        explicit MovingBlock(MovingBlockManager& manager) : m_movingBlockManager(manager) {};

        virtual void setPreviousData() = 0;
        virtual void interpolateData(float alpha) = 0;
        virtual void statusUpdate(float deltaTime) = 0;
        virtual void activate() = 0;
        virtual void draw() = 0;
    protected:
        MovingBlockManager& m_movingBlockManager;
    };
}

#endif //MFCPP_MOVINGBLOCK_HPP