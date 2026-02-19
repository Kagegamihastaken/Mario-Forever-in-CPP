#ifndef MFCPP_SPIKEAITYPE_HPP
#define MFCPP_SPIKEAITYPE_HPP

#include "EnemyType.hpp"

namespace MFCPP {
    class SpikeAIType : public EnemyType {
        EnemyTypeEnum getEnemyType() override {
            return SPIKE_AI;
        }
    };
}

#endif //MFCPP_BULLETBILLAITYPE_HPP