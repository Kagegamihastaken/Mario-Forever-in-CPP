#include "Core/Loading/GoombaAILoading.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Bonus/FireFlower.hpp"
#include "Object/Bonus/GreenMushroom.hpp"
#include "Object/Bonus/Mushroom.hpp"
#include "Object/Enemy/Goomba.hpp"
#include "Object/Enemy/GreenKoopa.hpp"
#include "Object/Enemy/GreenKoopaParatroopa.hpp"
#include "Object/Enemy/GreenSpiny.hpp"
#include "Object/Enemy/RedKoopa.hpp"
#include "Object/Enemy/RedSpiny.hpp"

namespace MFCPP {
    void AddGoombaAI(GoombaAIID type, int SkinID, const float x, const float y, const GoombaAIDirection Dir = GoombaAIDirection::LEFT) {
        switch (type) {
            case GoombaAIID::GOOMBA:
                GameScene::enemyManager.addEnemy<Goomba>(sf::Vector2f(x, y));
                break;
            case GoombaAIID::KOOPA:
                switch (SkinID) {
                    case 0:
                        GameScene::enemyManager.addEnemy<GreenKoopa>(sf::Vector2f(x, y), false);
                        break;
                    case 1:
                        GameScene::enemyManager.addEnemy<RedKoopa>(sf::Vector2f(x, y), false);
                        break;
                    default: ;
                }
                break;
            case GoombaAIID::MUSHROOM:
                switch (SkinID) {
                    case 0:
                        GameScene::enemyManager.addEnemy<Mushroom>(sf::Vector2f(x, y));
                        break;
                    case 1:
                        GameScene::enemyManager.addEnemy<GreenMushroom>(sf::Vector2f(x, y));
                        break;
                    default: ;
                }
                break;
            case GoombaAIID::SHELL:
                switch (SkinID) {
                    case 0:
                        GameScene::enemyManager.addEnemy<GreenKoopa>(sf::Vector2f(x, y), true);
                        break;
                    case 1:
                        GameScene::enemyManager.addEnemy<RedKoopa>(sf::Vector2f(x, y), true);
                        break;
                    default: ;
                }
                break;
            case GoombaAIID::SPINY:
                switch (SkinID) {
                    case 0:
                        GameScene::enemyManager.addEnemy<RedSpiny>(sf::Vector2f(x, y));
                        break;
                    case 1:
                        GameScene::enemyManager.addEnemy<GreenSpiny>(sf::Vector2f(x, y));
                        break;
                    default: ;
                }
                break;
            case GoombaAIID::FIRE_FLOWER:
                GameScene::enemyManager.addEnemy<FireFlower>(sf::Vector2f(x, y));
                break;
            case GoombaAIID::KOOPA_PARATROOPA:
                GameScene::enemyManager.addEnemy<GreenKoopaParatroopa>(sf::Vector2f(x, y), 0.f);
                break;
            default: ;
        }
    }
}
