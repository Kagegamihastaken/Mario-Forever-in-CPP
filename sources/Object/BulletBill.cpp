#include "Object/BulletBill.hpp"

#include <vector>

#include "Core/ImageManager.hpp"
#include "Core/Class/BulletBillClass.hpp"
#include "Core/ExternalHeaders/plf_colony.h"
#include "Core/Interpolation.hpp"
static std::vector<std::string> BulletBillNormalAnimName;
plf::colony<MFCPP::BulletBill> BulletBillList;
void BulletBillInit() {
    ImageManager::AddTexture("BulletBillNormal_0", "data/resources/BulletBill/BulletBillNormal.png");
    BulletBillNormalAnimName.push_back("BulletBillNormal_0");
}
void SetPrevBulletBillPos() {
    for (auto & i : BulletBillList) {
        if (i.willBeDestroyed()) continue;

        i.setPreviousPosition(i.getCurrentPosition());
    }
}
void InterpolateBulletBillPos(const float alpha) {
    for (auto & i : BulletBillList) {
        if (i.willBeDestroyed()) continue;

        i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
    }
}
void AddBulletBill(const BulletType type, const float speed, const bool direction, float x, float y) {
    plf::colony<MFCPP::BulletBill>::colony_iterator<false> it;
    switch (type) {
        case BULLET_NORMAL:
            it = BulletBillList.emplace(speed, direction, sf::FloatRect({0.f, 0.f}, {34.f, 28.f}), sf::Vector2f(x, y), sf::Vector2f(16.f, 27.f));
            //!direction
    }
}

