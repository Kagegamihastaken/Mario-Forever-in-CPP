#include "Object/BulletBillAI.hpp"
#include <vector>
#include "Core/ImageManager.hpp"
#include "Core/WindowFrame.hpp"

static std::vector<std::string> BulletBillNormalAnimName;
void BulletBillInit() {
    ImageManager::AddTexture("BulletBillNormal_0", "data/resources/BulletBill/BulletBillNormal.png");
    BulletBillNormalAnimName.emplace_back("BulletBillNormal_0");
}