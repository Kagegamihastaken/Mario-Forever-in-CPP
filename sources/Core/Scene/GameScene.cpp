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
#include "Projectiles/BroAIProjectile.hpp"
#include "Projectiles/MarioProjectile.hpp"
#include "Projectiles/PiranhaProjectile.hpp"
#include "Text/Text.hpp"

#include <fmt/format.h>

#include "Core/Level.hpp"
#include "Core/Time.hpp"
#include "Projectiles/ProjectileHelper.hpp"
#include "Core/Checkpoint.hpp"
#include "Core/Object/EnemyManager.hpp"
#include "Core/Object/Enemy/RotodiscAIBehavior.hpp"
#include "Object/RotodiscAI.hpp"
#include "Object/Enemy/BulletBill.hpp"
#include "Object/Enemy/FireBro.hpp"
#include "Object/Enemy/HammerBro.hpp"

EnemyManager GameScene::enemyManager;

GameScene::GameScene(SceneManager &manager)
    : Scene(manager) {}

void GameScene::handleInput(const std::optional<sf::Event>& event) {
    if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            enemyManager.addEnemy<HammerBro>(sf::Vector2f(MouseX + view.getCenter().x - Width / 2.f, MouseY + view.getCenter().y - Height / 2.f));
            //AddBroAI(BroAIType::FIRE_BRO, BroAIMovementType::CAN_JUMP, MouseX + view.getCenter().x - Width / 2.f, MouseY + view.getCenter().y - Height / 2.f);
        }
        else if (mousePressed->button == sf::Mouse::Button::Middle)
            SetPowerState(2);
        else if (mousePressed->button == sf::Mouse::Button::Right) {
            AddBroAI(BroAIType::HAMMER_BRO, BroAIMovementType::CAN_JUMP, MouseX + view.getCenter().x - Width / 2.f, MouseY + view.getCenter().y - Height / 2.f);
            //AddGoombaAI(GoombaAIType::SPINY, 1, MouseX + view.getCenter().x - Width / 2.f, MouseY + view.getCenter().y - Height / 2.f, GoombaAIDirection::LEFT);
        }
    }
}
void GameScene::update(const float deltaTime) {
    // UI Update
    TimeUpdate(deltaTime);
    TimeRingBehavior();

    //Checkpoint
    CheckpointAnimationUpdate();
    CheckpointCollision();

    ExitGateClockUpdate(deltaTime);
    PlatformPositionUpdate(deltaTime);

    KeyboardMovement(deltaTime);
    MarioUpdateHitbox();
    MarioPosXUpdate(deltaTime);
    MarioVertXUpdate();
    MarioPosYUpdate(deltaTime);
    MarioVertYUpdate();

    enemyManager.XUpdate(deltaTime);
    enemyManager.YUpdate(deltaTime);
    enemyManager.statusUpdate(deltaTime);

    BroAIProjectileMovementUpdate(deltaTime);
    BroAIProjectileSpin(deltaTime);

    MarioProjectileMovementUpdate(deltaTime);
    MarioProjectileSpin(deltaTime);

    PiranhaAIMovementUpdate(deltaTime);
    PiranhaAIShoot(deltaTime);
    PiranhaAIProjectileMovementUpdate(deltaTime);
    PiranhaAIProjectileSpin(deltaTime);

    CoinEffectStatusUpdate(deltaTime);
    ScoreEffectStatusUpdate(deltaTime);
    BrickParticleStatusUpdate(deltaTime);
    MarioEffectStatusUpdate(deltaTime);
    ExitGateStatusUpdate(deltaTime);
    BrickUpdate(deltaTime);
    LuckyBlockUpdate(deltaTime);
    BulletLauncherStatusUpdate(deltaTime);

}
void GameScene::setPreviousPosition() {
    SetPrevMarioPos();
    SetPrevPiranhaAIPos();
    SetPrevCoinEffectPos();
    SetPrevScoreEffectPos();
    SetPrevBrickParticlePos();
    SetPrevMarioEffectPos();
    SetPrevExitGatePos();
    SetPrevBricksPos();
    SetPrevLuckyBlockPos();
    SetPrevBroAIProjectilePos();
    SetPrevMarioProjectilePos();
    SetPrevPlatformPos();
    SetPrevPiranhaAIProjectilePos();
    enemyManager.setPreviousData();
}
void GameScene::interpolatePosition(const float alpha) {
    InterpolateMarioPos(alpha);
    InterpolatePiranhaAIPos(alpha);
    InterpolateCoinEffectPos(alpha);
    InterpolateScoreEffectPos(alpha);
    InterpolateBrickParticlePos(alpha);
    InterpolateMarioEffectPos(alpha);
    InterpolateExitGatePos(alpha);
    InterpolateBricksPos(alpha);
    InterpolateLuckyBlockPos(alpha);
    InterpolateBroAIProjectilePos(alpha);
    InterpolateMarioProjectilePos(alpha);
    InterpolatePlatformPos(alpha);
    InterpolatePiranhaAIProjectilePos(alpha);
    enemyManager.interpolateData(alpha);
}
void GameScene::draw(sf::RenderWindow &window) {
    BgGradientDraw();

    BgDraw();
    ExitGateDraw();
    CheckpointDraw();
    PiranhaAIDraw();
    ImageManager::DrawAllVertex();
    ObstaclesDraw();
    DrawPlatform();
    MarioDraw();
    enemyManager.DrawLowPriority();
    BrickDraw();
    LuckyBlockDraw();
    SpikeDraw();
    CoinDraw();
    BulletLauncherDraw();
    enemyManager.DrawHighPriority();
    BrickParticleDraw();
    CoinEffectDraw();
    ScoreEffectDraw();
    BroAIProjectileDraw();
    MarioProjectileDraw();
    PiranhaAIProjectileDraw();
    FireballExplosionDraw();
    MarioEffectDraw();
    ExitGateEffectDraw();
    FrameDraw();

    ImageManager::DrawAllVertex();
    TextDraw();
}
void GameScene::objectCleanup() {
    MarioProjectileCleanup();
    PiranhaAICleanup();
    BrickCleanup();
    ScoreEffectCleanup();
    BrickParticleCleanup();
    CoinCleanup();
    SpikeCleanup();
    LuckyBlockCleanup();
    PlatformCleanup();
    PiranhaAIProjectileCleanup();
    BroAIProjectileCleanup();
    CoinEffectCleanup();
    enemyManager.EnemyCleanup();
}
void GameScene::postUpdate() {
    PlatformStatusUpdate();
    BroAIProjectileCollision();
    MarioProjectileCollision();
    PiranhaAIProjectileCollision();

    enemyManager.MarioCollision();
    enemyManager.EnemyCollision();

    CoinOnTouch();
    PiranhaAIStatusUpdate();
    SpikeStatusUpdate();
    CheckForDeath();
    BroAIProjectileStatusUpdate();
    MarioProjectileStatusUpdate();
    PiranhaAIProjectileStatusUpdate();
    FireballExplosionStatusUpdate();

    MarioUpdateAnimation();
}
void GameScene::HUDPositionUpdate() {
    BgUpdatePos();
}
void GameScene::textUpdate() {
    EditText(fmt::format("{}", Lives), "_LIVE");
    EditText(fmt::format("FPS: {}", static_cast<int>(std::round(fpsLite.getFps()))), "_FPS");
    if (isDebug) {
        EditText(std::to_string(static_cast<int>(MouseX)) + "/" + std::to_string(static_cast<int>(MouseY)) + "  R", "_MOUSEXY");
        EditText(std::to_string(static_cast<int>(ViewX)) + "/" + std::to_string(static_cast<int>(ViewY)) + "  V", "_VIEWXY");
    }
    EditText(fmt::format("{}", CoinCount), "_COIN");
    EditText(fmt::format("{}", Score), "_SCORE");
    EditText(fmt::format("{}", getTime()), "_TIME");
}
void GameScene::setView() {
    const sf::Vector2f& ScrollPos = player.property.getPosition();
    view.setCenter({ std::min(std::max(Width / 2.0f, ScrollPos.x), LevelWidth - Width / 2.f), std::min(std::max(Height / 2.0f, ScrollPos.y), LevelHeight - Height / 2.f) });
}
void GameScene::loadResources() {
    //Load Resources
    GameSceneInit();
    //Preload
    loadObstacleRes();
    //Init Projectile Texture First then anything else
    ProjectileInit();
    //Force Load
    loadMarioRes();
    BrickParticleInit();
    BricksInit();
    LoadLuckyBlock();
    GoombaAIInit();
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
    ReadData("data/levels/onedashtwo.json");
    Bgbuilding();
    CheckpointBuilding();
    Obstaclebuilding();
    Objectbuilding();
    ExitGateBuilding();

    //enemyManager.addEnemy<Goomba>(sf::Vector2f(128.f, 128.f));
}
void GameScene::unloadResources() {
    //implement later
}








