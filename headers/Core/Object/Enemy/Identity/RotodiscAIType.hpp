#ifndef MFCPP_ROTODISCAITYPE_HPP
#define MFCPP_ROTODISCAITYPE_HPP

#include "EnemyType.hpp"

namespace MFCPP {
    class RotodiscAIType : public EnemyType {
        EnemyTypeEnum getEnemyType() override {
            return ROTODISC_AI;
        }
    };
}

#endif //MFCPP_ROTODISCAITYPE_HPP