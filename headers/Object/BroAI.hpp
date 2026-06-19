#ifndef BROAI_HPP
#define BROAI_HPP

#include "Core/ExternalHeaders/plf_colony.h"
#include "Core/Loading/enum.hpp"

extern void BroAILoadRes();
extern void AddBroAI(BroAIType type, BroAIMovementType movementType, float x, float y);

#endif //BROAI_HPP
