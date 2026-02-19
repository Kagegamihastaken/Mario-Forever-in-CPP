#ifndef MFCPP_PIRANHAAITYPE_HPP
#define MFCPP_PIRANHAAITYPE_HPP

#include "EnemyType.hpp"

namespace MFCPP {
    class PiranhaAIType : public EnemyType {
        EnemyTypeEnum getEnemyType() override {
            return PIRANHA_AI;
        }
    };
}

#endif //MFCPP_BULLETBILLAITYPE_HPP