#ifndef MFCPP_ENEMYTYPE_HPP
#define MFCPP_ENEMYTYPE_HPP

namespace MFCPP {
    enum EnemyTypeEnum {GOOMBA_AI, PIRANHA_AI, BRO_AI, BULLET_BILL_AI, ROTODISC_AI, SPIKE_AI};

    class EnemyType {
    public:
        EnemyType() = default;
        virtual EnemyTypeEnum getEnemyType() = 0;
        virtual ~EnemyType() = default;
    };
}
#endif //MFCPP_ENEMYTYPE_HPP