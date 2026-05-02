#ifndef MFCPP_MOVINGBLOCKTYPE_HPP
#define MFCPP_MOVINGBLOCKTYPE_HPP

namespace MFCPP {
    enum MovingBlockEnum {PLATFORM, MOVING_SOLID, TANK_GROUND};

    class MovingBlockType {
    public:
        MovingBlockType() = default;
        virtual MovingBlockEnum getMovingBlockType() = 0;
        virtual ~MovingBlockType() = default;
    };
}

#endif //MFCPP_MOVINGBLOCKTYPE_HPP