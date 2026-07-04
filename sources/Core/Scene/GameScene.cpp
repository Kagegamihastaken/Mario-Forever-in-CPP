#include <fmt/format.h>

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

EnemyManager GameScene::enemyManager;
CustomTileManager GameScene::customTileManager;
ProjectileManager GameScene::projectileManager;
MovingBlockManager GameScene::movingBlockManager;
SceneryManager GameScene::sceneryManager;
EffectManager GameScene::effectManager;

GameScene::GameScene(SceneManager &manager)
    : Scene(manager) {
}

void GameScene::handleInput(const std::optional<sf::Event> &event) {
    if (const auto *mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
        switch (mousePressed->button) {
            case sf::Mouse::Button::Middle:
                Mario::SetPowerState(3);
                break;
            default: ;
        }
    }
}

void GameScene::update(const float deltaTime) {
    // UI Update
    TimeUpdate(deltaTime);
    TimeRingBehavior();
    //Animation Update
    enemyManager.animationUpdate(deltaTime);
    customTileManager.animationUpdate(deltaTime);
    projectileManager.animationUpdate(deltaTime);
    movingBlockManager.animationUpdate(deltaTime);
    sceneryManager.animationUpdate(deltaTime);
    effectManager.animationUpdate(deltaTime);

    Mario::MarioAnimationUpdate(deltaTime);
    CoinAnimationUpdate(deltaTime);
    ExitGateAnimationUpdate(deltaTime);

    MFCPP::AutoScroll::UpdateSpeed(deltaTime);
    Mario::MarioOutSideScreen();

    //Checkpoint
    CheckpointAnimationUpdate(deltaTime);
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
    enemyManager.updatePreviousData();
    customTileManager.updatePreviousData();
    projectileManager.updatePreviousData();
    movingBlockManager.updatePreviousData();
    sceneryManager.updatePreviousData();
    effectManager.updatePreviousData();
}

void GameScene::interpolatePosition(const float alpha) {
    Mario::storeAlpha(alpha);
}

void GameScene::draw(sf::RenderWindow &window, float alpha) {
    MFCPP::Tilemap::drawHitboxMap();

    BgGradientDraw();
    BgDraw();
    sceneryManager.draw(alpha);
    ExitGateDraw(alpha);
    CheckpointDraw(alpha);
    enemyManager.DrawPriority(0, alpha);
    Mario::MarioDraw(alpha);
    ObstaclesDraw();
    enemyManager.DrawPriority(1, alpha);
    movingBlockManager.draw(alpha);
    customTileManager.DrawPriority(0, alpha);
    CoinDraw(alpha);
    enemyManager.DrawPriority(2, alpha);
    projectileManager.DrawPriority(0, alpha);
    customTileManager.DrawPriority(1, alpha);
    enemyManager.DrawPriority(3, alpha);
    projectileManager.DrawPriority(1, alpha);
    projectileManager.DrawPriority(2, alpha);
    MarioEffectDraw(alpha);
    ExitGateEffectDraw(alpha);
    effectManager.Draw(alpha);
    WindowFrame::FrameDraw();

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
    EditText(fmt::format("FPS: {}", static_cast<int32_t>(std::round(WindowFrame::getFpsLite().getFps()))), "_FPS");
    if (WindowFrame::isDebug()) {
        EditText(std::to_string(static_cast<int32_t>(WindowFrame::getMousePosition().x)) + "/" + std::to_string(
                     static_cast<int32_t>(WindowFrame::getMousePosition().y)) + "  R", "_MOUSEXY");
        EditText(std::to_string(static_cast<int32_t>(Scroll::getViewPosition().x)) + "/" + std::to_string(
                     static_cast<int32_t>(Scroll::getViewPosition().y)) + "  V", "_VIEWXY");
    }
    EditText(fmt::format("{}", CoinCount), "_COIN");
    EditText(fmt::format("{}", Mario::getScore()), "_SCORE");
    EditText(fmt::format("{}", getTime()), "_TIME");
}

void GameScene::setView() {
    const sf::Vector2f ScrollPos = (MFCPP::AutoScroll::getAutoScrollMode()
                                        ? MFCPP::AutoScroll::getPosition()
                                        : Mario::getInterpolatedPosition());
    Scroll::getView().setCenter({
        std::min(std::max(WindowFrame::getGameSize().x / 2.0f, ScrollPos.x),
                 LevelWidth - WindowFrame::getGameSize().x / 2.f),
        std::min(std::max(WindowFrame::getGameSize().y / 2.0f, ScrollPos.y),
                 LevelHeight - WindowFrame::getGameSize().y / 2.f)
    });
}

void GameScene::loadResources() {
    //Load Resources
    WindowFrame::GameSceneInit();
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
    BulletBillInit();
    PlatformInit();
    CheckpointInit();
    RotodiscInit();
    CustomBlockInit();

    AddText("_COIN", "", TextMarginID::RIGHT_MARGIN, 287.0f, 15.0f);
    AddText("_LIVE", "", TextMarginID::LEFT_MARGIN, 138.0f, 15.0f);
    AddText("_SCORE", "", TextMarginID::RIGHT_MARGIN, 138.0f, 34.0f);
    AddText("_TIME", "", TextMarginID::RIGHT_MARGIN, 562.0f, 35.0f);
    AddText("_FPS", "", TextMarginID::LEFT_MARGIN, 0.0f, 464.0f);
    if (WindowFrame::isDebug()) {
        AddText("_MOUSEXY", "", TextMarginID::RIGHT_MARGIN, 624.0f, 464.0f);
        AddText("_VIEWXY", "", TextMarginID::RIGHT_MARGIN, 624.0f, 432.0f);
        AddText("_APPE", "", TextMarginID::LEFT_MARGIN, 0.0f, 64.0f);
    }
    //Load Level
    //ReadData("data/levels/onedashthree.json");
    ReadData("data/levels/twodashone.json");
    //ReadData("data/levels/untitled.json");
    //ReadData("data/levels/gearuptest.json");
    //ReadData("data/levels/sevendashone.json");
    Bgbuilding();
    CheckpointBuilding();
    Obstaclebuilding();
    Objectbuilding();
    ExitGateBuilding();
}

void GameScene::unloadResources() {
    //implement later
}
