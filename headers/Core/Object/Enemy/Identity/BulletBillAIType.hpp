#ifndef MFCPP_BULLETBILLAITYPE_HPP
#define MFCPP_BULLETBILLAITYPE_HPP

#include "EnemyType.hpp"

namespace MFCPP {
    class BulletBillAIType : public EnemyType {
        EnemyTypeEnum getEnemyType() override {
            return BULLET_BILL_AI;
        }
    };
}

#endif //MFCPP_BULLETBILLAITYPE_HPP