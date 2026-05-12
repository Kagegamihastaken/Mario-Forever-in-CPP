#include "Core/Scene/GameScene.hpp"
#include "Block/Brick.hpp"
#include "Block/BulletLauncher.hpp"
#include "Block/LuckyBlock.hpp"
#include "Core/SceneManager.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Background/Bg.hpp"
#include "Core/Background/BgGradient.hpp"
#include "Effect/BrickParticle.hpp"
#include "Effect/BroAIEffect.hpp"
#include "Effect/CoinEffect.hpp"
#include "Effect/FireballExplosion.hpp"
#include "Effect/GoombaAIEffect.hpp"
#include "Effect/MarioEffect.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/BroAI.hpp"
#include "Object/BulletBillAI.hpp"
#include "Object/Coin.hpp"
#include "Object/ExitGate.hpp"
#include "Object/GoombaAI.hpp"
#include "Object/Mario.hpp"
#include "Object/PiranhaAI.hpp"
#include "Object/Platform.hpp"
#include "Object/Spike.hpp"
#include "Text/Text.hpp"

#include <fmt/format.h>

#include "Block/CustomBlock.hpp"
#include "Core/Level.hpp"
#include "Core/Time.hpp"
#include "Projectiles/ProjectileHelper.hpp"
#include "Core/Checkpoint.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Core/Object/Enemy/Behavior/RotodiscAIBehavior.hpp"
#include "Block/Obstacles.hpp"
#include "Core/AutoScroll.hpp"
#include "Core/HitboxUtils.hpp"
#include "Core/Tilemap.hpp"
#include "Object/RotodiscAI.hpp"
#include "Object/SceneryHelper.hpp"
#include "Object/Projectile/FireLauncherProjectile.hpp"

EnemyManager GameScene::enemyManager;
CustomTileManager GameScene::customTileManager;
ProjectileManager GameScene::projectileManager;
MovingBlockManager GameScene::movingBlockManager;
SceneryManager GameScene::sceneryManager;
EffectManager GameScene::effectManager;

GameScene::GameScene(SceneManager &manager)
    : Scene(manager) {}

void GameScene::handleInput(const std::optional<sf::Event>& event) {
    if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            projectileManager.addProjectile<FireLauncherProjectile>(sf::Vector2f(MouseX + view.getCenter().x - Width / 2.f, MouseY + view.getCenter().y - Height / 2.f), sf::Vector2f(-8.5f, 0.f));
            //enemyManager.addEnemy<GreenKoopaParatroopa>(sf::Vector2f(MouseX + view.getCenter().x - Width / 2.f, MouseY + view.getCenter().y - Height / 2.f), 0.f);
            //AddBroAI(BroAIType::FIRE_BRO, BroAIMovementType::CAN_JUMP, MouseX + view.getCenter().x - Width / 2.f, MouseY + view.getCenter().y - Height / 2.f);
        }
        else if (mousePressed->button == sf::Mouse::Button::Middle)
            Mario::SetPowerState(3);
        else if (mousePressed->button == sf::Mouse::Button::Right) {
            AddBroAI(BroAIType::FIRE_BRO, BroAIMovementType::CAN_JUMP, MouseX + view.getCenter().x - Width / 2.f, MouseY + view.getCenter().y - Height / 2.f);
            //AddGoombaAI(GoombaAIType::SPINY, 1, MouseX + view.getCenter().x - Width / 2.f, MouseY + view.getCenter().y - Height / 2.f, GoombaAIDirection::LEFT);
        }
    }
}
void GameScene::update(const float deltaTime) {
    // UI Update
    TimeUpdate(deltaTime);
    TimeRingBehavior();

    MFCPP::AutoScroll::UpdateSpeed(deltaTime);
    Mario::MarioOutSideScreen();

    //Checkpoint
    CheckpointAnimationUpdate();
    CheckpointCollision();

    ExitGateClockUpdate(deltaTime);

    movingBlockManager.statusUpdate(deltaTime);

    Mario::KeyboardMovement(deltaTime);
    Mario::MarioUpdateHitbox();
    Mario::MarioPosXUpdate(deltaTime);
    Mario::MarioVertXUpdate();
    Mario::MarioPosYUpdate(deltaTime);
    Mario::MarioVertYBottomUpdate();
    customTileManager.HitEvent();
    Mario::MarioVertYTopUpdate();

    enemyManager.XUpdate(deltaTime);
    enemyManager.YUpdate(deltaTime);
    enemyManager.statusUpdate(deltaTime);

    customTileManager.updateCollision();
    customTileManager.statusUpdate(deltaTime);

    projectileManager.statusUpdate(deltaTime);
    projectileManager.CollisionUpdate();

    effectManager.statusUpdate(deltaTime);

    MarioEffectStatusUpdate(deltaTime);
    ExitGateStatusUpdate(deltaTime);

}
void GameScene::setPreviousPosition() {
    Mario::SetPrevMarioPos();
    SetPrevMarioEffectPos();
    SetPrevExitGatePos();
    enemyManager.setPreviousData();
    customTileManager.setPreviousData();
    projectileManager.setPreviousData();
    movingBlockManager.setPreviousData();
    sceneryManager.setPreviousData();
    effectManager.setPreviousData();
}
void GameScene::interpolatePosition(const float alpha) {
    Mario::InterpolateMarioPos(alpha);
    InterpolateMarioEffectPos(alpha);
    InterpolateExitGatePos(alpha);
    enemyManager.interpolateData(alpha);
    customTileManager.interpolateData(alpha);
    projectileManager.interpolateData(alpha);
    movingBlockManager.interpolateData(alpha);
    sceneryManager.interpolateData(alpha);
    effectManager.interpolateData(alpha);
}
void GameScene::draw(sf::RenderWindow &window) {
    MFCPP::drawHitboxMap();

    BgGradientDraw();

    BgDraw();
    sceneryManager.draw();
    ExitGateDraw();
    CheckpointDraw();
    enemyManager.DrawPriority(0);
    Mario::MarioDraw();
    ObstaclesDraw();
    enemyManager.DrawPriority(1);
    movingBlockManager.draw();
    customTileManager.DrawPriority(0);
    CoinDraw();
    enemyManager.DrawPriority(2);
    projectileManager.DrawPriority(0);
    customTileManager.DrawPriority(1);
    enemyManager.DrawPriority(3);
    projectileManager.DrawPriority(1);
    projectileManager.DrawPriority(2);
    MarioEffectDraw();
    ExitGateEffectDraw();
    effectManager.Draw();
    FrameDraw();

    HitboxUtils::drawHitbox();
    TextDraw();
}
void GameScene::objectCleanup() {
    CoinCleanup();
    movingBlockManager.MovingBlockCleanup();
    enemyManager.EnemyCleanup();
    customTileManager.CustomTileCleanup();
    projectileManager.ProjectileCleanup();
    sceneryManager.SceneryCleanup();
    effectManager.EffectCleanup();
}
void GameScene::postUpdate() {
    enemyManager.MarioCollision();
    enemyManager.EnemyCollision();

    CoinOnTouch();
    Mario::CheckForDeath();

    Mario::MarioUpdateAnimation();
}
void GameScene::HUDPositionUpdate() {
    BgUpdatePos();
}
void GameScene::textUpdate() {
    EditText(fmt::format("{}", Mario::getLives()), "_LIVE");
    EditText(fmt::format("FPS: {}", static_cast<int>(std::round(fpsLite.getFps()))), "_FPS");
    if (isDebug) {
        EditText(std::to_string(static_cast<int>(MouseX)) + "/" + std::to_string(static_cast<int>(MouseY)) + "  R", "_MOUSEXY");
        EditText(std::to_string(static_cast<int>(ViewX)) + "/" + std::to_string(static_cast<int>(ViewY)) + "  V", "_VIEWXY");
    }
    EditText(fmt::format("{}", CoinCount), "_COIN");
    EditText(fmt::format("{}", Mario::getScore()), "_SCORE");
    EditText(fmt::format("{}", getTime()), "_TIME");
}
void GameScene::setView() {
    sf::Vector2f ScrollPos;
    if (MFCPP::AutoScroll::getAutoScrollMode()) {
        ScrollPos = MFCPP::AutoScroll::getPosition();
    }
    else
        ScrollPos = Mario::getInterpolatedPosition();
    view.setCenter({ std::min(std::max(Width / 2.0f, ScrollPos.x), LevelWidth - Width / 2.f), std::min(std::max(Height / 2.0f, ScrollPos.y), LevelHeight - Height / 2.f) });
}
void GameScene::loadResources() {
    //Load Resources
    GameSceneInit();
    MFCPP::AutoScroll::AutoScrollInit();
    //Preload
    loadObstacleRes();
    //Init Projectile Texture First then anything else
    ProjectileInit();
    //Force Load
    SceneryInit();
    Mario::loadMarioRes();
    BrickParticleInit();
    BricksInit();
    GoombaAIInit();
    LoadLuckyBlock();
    BroAILoadRes();
    BroAIEffectInit();
    CoinEffectInit();
    CoinInit();
    ScoreEffectInit();
    GoombaAIEffectInit();
    PiranhaAIInit();
    SpikeInit();
    MarioEffectInit();
    BgInit();
    ExitGateInit();
    FireballExplosionInit();
    BulletLauncherInit();
    BulletBillInit();
    PlatformInit();
    CheckpointInit();
    RotodiscInit();
    CustomBlockInit();

    AddText("_COIN", "", RIGHT_MARGIN, 287.0f, 15.0f);
    AddText("_LIVE", "", LEFT_MARGIN, 138.0f, 15.0f);
    AddText("_SCORE", "", RIGHT_MARGIN, 138.0f, 34.0f);
    AddText("_TIME", "", RIGHT_MARGIN, 562.0f, 35.0f);
    AddText("_FPS", "", LEFT_MARGIN, 0.0f, 464.0f);
    if (isDebug) {
        AddText("_MOUSEXY", "", RIGHT_MARGIN, 624.0f, 464.0f);
        AddText("_VIEWXY", "", RIGHT_MARGIN, 624.0f, 432.0f);
        AddText("_APPE", "", LEFT_MARGIN, 0.0f, 64.0f);
    }
    //Load Level
    //ReadData("data/levels/onedashthree.json");
    //ReadData("data/levels/twodashone.json");
    //ReadData("data/levels/untitled.json");
    //ReadData("data/levels/gearuptest.json");
    ReadData("data/levels/sevendashone.json");
    Bgbuilding();
    CheckpointBuilding();
    Obstaclebuilding();
    Objectbuilding();
    ExitGateBuilding();
}
void GameScene::unloadResources() {
    //implement later
}
//TODO: Implement Scenery







