#include "Core/Object/CustomTile/Behavior/HitBehavior.hpp"

#include "Core/SoundManager.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/CoinEffect.hpp"
#include "Object/Coin.hpp"

void HitBehavior::HitDetection(const MFCPP::CollisionObject &obj) {
    sf::FloatRect hitbox = getGlobalHitbox(obj.GetLeftHitbox(), obj.GetPosition() - sf::Vector2f(0.f, 16.f), obj.GetOrigin());
    for (auto jt = CoinList.begin(); jt != CoinList.end(); ++jt) {
        if (jt->isDestroyed()) continue;
        if (sf::FloatRect CoinCollide = getGlobalHitbox(jt->getHitbox(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(CoinCollide, hitbox)) {
            AddCoinEffect(jt->getID(), jt->getAttribute(), jt->getCurrentPosition().x + 15.0f, jt->getCurrentPosition().y + 32.0f);
            DeleteIndexCoin(jt);
            SoundManager::PlaySound("Coin");
            ++CoinCount;
        }
    }
    auto& list = GameScene::enemyManager.getGoombaAIList();
    for (auto jt = list.begin(); jt != list.end(); ++jt) {
        if (sf::FloatRect EnemyGoombaAICollide = getGlobalHitbox(jt->getHitbox(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(EnemyGoombaAICollide, hitbox))
            if (!jt->isDeath()) jt->BlockHit();
    }
    auto& BroList = GameScene::enemyManager.getBroAIList();
    for (auto jt = BroList.begin(); jt != BroList.end(); ++jt) {
        if (sf::FloatRect EnemyBroAICollide = getGlobalHitbox(jt->getHitbox(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(EnemyBroAICollide, hitbox))
            if (!jt->isDeath()) jt->BlockHit();
    }
}
