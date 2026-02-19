#ifndef MFCPP_BROAITYPE_HPP
#define MFCPP_BROAITYPE_HPP

#include "EnemyType.hpp"

namespace MFCPP {
    class BroAIType : public EnemyType {
        EnemyTypeEnum getEnemyType() override {
            return BRO_AI;
        }
    };
}

#endif //MFCPP_BROAITYPE_HPP