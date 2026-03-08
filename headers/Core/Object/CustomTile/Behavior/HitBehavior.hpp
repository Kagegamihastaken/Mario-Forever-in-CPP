#ifndef MFCPP_HITBEHAVIOR_HPP
#define MFCPP_HITBEHAVIOR_HPP
#include "Core/Class/CollisionObjectClass.hpp"

class HitBehavior {
public:
    static void HitDetection(const MFCPP::CollisionObject& obj);
};

#endif //MFCPP_HITBEHAVIOR_HPP