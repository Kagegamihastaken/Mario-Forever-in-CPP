#include "Core/Loading/SceneryLoading.hpp"

#include "Core/Scene/GameScene.hpp"
#include "Object/Scenery/BlueCloud.hpp"
#include "Object/Scenery/BlueGrass.hpp"
#include "Object/Scenery/TankTileBack.hpp"

namespace MFCPP {
    void AddScenery(const int id, const sf::Vector2f& pos) {
        switch (id) {
            case 0:
                GameScene::sceneryManager.addScenery<BlueCloud>(pos);
                break;
            case 1:
                GameScene::sceneryManager.addScenery<BlueGrass>(pos);
                break;
            case 2:
                GameScene::sceneryManager.addScenery<TankTileBack>(pos);
                break;
            default: ;
        }
    }
}