#ifndef BROAI_HPP
#define BROAI_HPP

#include "Core/ExternalHeaders/plf_colony.h"
#include "Core/Loading/enum.hpp"

extern void BroAILoadRes();
extern void AddBroAI(BroAIType type, BroAIMovementType movementType, float x, float y);

extern std::vector<std::string> HammerBroAnimName;
extern std::vector<std::string> HammerBroLaunchAnimName;
extern std::vector<std::string> FireBroAnimName;
extern std::vector<std::string> FireBroLaunchAnimName;

#endif //BROAI_HPP
