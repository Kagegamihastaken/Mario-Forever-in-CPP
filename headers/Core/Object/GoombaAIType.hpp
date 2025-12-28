#ifndef MFCPP_GOOMBAAITYPE_HPP
#define MFCPP_GOOMBAAITYPE_HPP

#include "Core/Object/EnemyType.hpp"

namespace MFCPP {
    class GoombaAIType : public EnemyType {
        EnemyTypeEnum getEnemyType() override {
            return GOOMBA_AI;
        }
    };
}

#endif //MFCPP_GOOMBAAITYPE_HPP