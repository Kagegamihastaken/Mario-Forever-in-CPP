#include "Block/Brick.hpp"
#include "Block/LuckyBlock.hpp"
#include "Core/ImageManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Sound.hpp"
#include "Block/Obstacles.hpp"
#include "Block/Slopes.hpp"
#include "Core/Level.hpp"
#include "Core/Music.hpp"
#include "Core/Scroll.hpp"
#include "Core/Background/Bg.hpp"
#include "Core/Background/BgGradient.hpp"
#include "Effect/BrickParticle.hpp"
#include "Effect/CoinEffect.hpp"
#include "Effect/GoombaAIEffect.hpp"
#include "Effect/MarioEffect.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/Coin.hpp"
#include "Object/ExitGate.hpp"
#include "Object/GoombaAI.hpp"
#include "Object/Mario.hpp"
#include "Object/PiranhaAI.hpp"
#include "Object/Spike.hpp"
#include "Text/Text.hpp"
#include "Core/Scene.hpp"
#include "Editor/Editor.hpp"
#include "Editor/SelectTile.hpp"
#include "Object/BroAI.hpp"

#include "Core/Game.hpp"

#include <iostream>

void GameObjectInit() {
    InitTempTex();
    loadSlopeRes();
    windowInit();
    SoundInit();
    loadObstacleRes();
    loadMarioRes();
    BrickParticleInit();
    loadFontRes();
    LoadBricks();
    LoadLuckyBlock();
    GoombaAILoadRes();
    BroAILoadRes();
    CoinEffectInit();
    CoinInit();
    ScoreEffectInit();
    GoombaAIEffectInit();
    PiranhaAIInit();
    SpikeInit();
    MusicInit();
    MarioEffectInit();
    BgInit();
    ExitGateInit();
    ViewInit();

    EditorInit();
    SelectTileInit();
}
void GameTextInit() {
    if (CurrentScene == SceneID::SCENE_GAMEPLAY) {
        //AddText("_DEBUG", (isDebug ? "DEBUG" : "RELEASE"), LEFT_MARGIN, 0.0f, 464.0f);
        AddText("_COIN", "", RIGHT_MARGIN, 287.0f, 15.0f);
        AddText("_LIVE", "", LEFT_MARGIN, 138.0f, 15.0f);
        AddText("_SCORE", "", RIGHT_MARGIN, 138.0f, 34.0f);
        //AddText("_FPS", "", LEFT_MARGIN, 0.0f, 448.0f);
        AddText("_FPS", "", LEFT_MARGIN, 0.0f, 464.0f);
        //AddText("_DELTA", "", LEFT_MARGIN, 0, 432.0f);
        //AddText("_HOLDING", "", LEFT_MARGIN, 0.0f, 432.0f);
        //AddText("_CODX", "", RIGHT_MARGIN, 624.0f, 416.0f);
        //AddText("_CODY", "", RIGHT_MARGIN, 624.0f, 400.0f);
        //AddText("_FALL", "", LEFT_MARGIN, 0.0f, 48.0f);
        //AddText("_MARIOXY", "", RIGHT_MARGIN, 624.0f, 448.0f);
        //AddText("_FALLING", "", LEFT_MARGIN, 0.0f, 80.0f);
        //AddText("_PREJUMP", "", LEFT_MARGIN, 0.0f, 96.0f);
        if (isDebug) {
            AddText("_MOUSEXY", "", RIGHT_MARGIN, 624.0f, 464.0f);
            AddText("_VIEWXY", "", RIGHT_MARGIN, 624.0f, 432.0f);
            AddText("_APPE", "", LEFT_MARGIN, 0.0f, 64.0f);
        }
    }
    else if (CurrentScene == SceneID::SCENE_LEVEL_EDITOR) {
        AddText("_FPS", "", LEFT_MARGIN, 0.0f, 464.0f);
        AddText("_MOUSEXY", "", RIGHT_MARGIN, 624.0f, 464.0f);
    }
}
void GameLoadLevel() {
    if (CurrentScene == SceneID::SCENE_GAMEPLAY) {
        ReadData("data/levels/lvl1.txt");
        Bgbuilding();
        Obstaclebuilding();
        Slopebuilding();
        Objectbuilding();
        BgGradientInitPos();
        ExitGateBuilding();
    }
}
void GameObjectEditText() {
    if (CurrentScene == SceneID::SCENE_GAMEPLAY) {
        //EditText("DeltaTime: " + std::to_string(deltaTime), "_DELTA");
        EditText(std::to_string(Lives), "_LIVE");
        EditText("FPS: " + std::to_string(static_cast<int>(fpsLite.getFps())), "_FPS");
        //EditText(std::to_string(Xvelo) + " VX", "_CODX");
        //EditText(std::to_string(Yvelo) + " VY", "_CODY");
        //EditText(std::to_string((int)player.property.getPosition().x) + "/" + std::to_string((int)player.property.getPosition().y) + "  M", "_MARIOXY");
        //EditText("Crouch Down: " + fall, "_FALL");
        //EditText(std::to_string(ObstaclesList.size() + Bricks.size() + LuckyBlock.size()), "_FALL");
        //EditText(std::to_string(GoombaAIList.size()), "_FALL");
        //EditText("Holding: " + std::string((Holding ? "TRUE" : "FALSE")), "_HOLDING");
        //EditText("Falling: " + (MarioCurrentFalling ? std::string("TRUE") : std::string("FALSE")), "_FALLING");
        //EditText("PreJump: " + (PreJump ? std::string("TRUE") : std::string("FALSE")), "_PREJUMP");
        if (isDebug) {
            std::string appe;
            EditText(std::to_string(static_cast<int>(MouseX)) + "/" + std::to_string(static_cast<int>(MouseY)) + "  R", "_MOUSEXY");
            EditText(std::to_string(static_cast<int>(ViewX)) + "/" + std::to_string(static_cast<int>(ViewY)) + "  V", "_VIEWXY");
            EditText("Appear: " + appe, "_APPE");
        }
        EditText(std::to_string(CoinCount), "_COIN");
        EditText(std::to_string(Score), "_SCORE");
    }
    else if (CurrentScene == SceneID::SCENE_LEVEL_EDITOR) {
        EditText("FPS: " + std::to_string(static_cast<int>(fpsLite.getFps())), "_FPS");
        EditText(std::to_string(static_cast<int>(MouseX)) + "/" + std::to_string(static_cast<int>(MouseY)) + "  R", "_MOUSEXY");
    }
}
void GameObjectSetPrev() {
    if (CurrentScene == SceneID::SCENE_GAMEPLAY) {
        SetPrevMarioPos();
        SetPrevGoombaAIPos();
        SetPrevGoombaAIEffectPos();
        SetPrevPiranhaAIPos();
        SetPrevCoinEffectPos();
        SetPrevScoreEffectPos();
        SetPrevBrickParticlePos();
        SetPrevMarioEffectPos();
        SetPrevExitGatePos();
        SetPrevBricksPos();
        SetPrevLuckyBlockPos();
        SetPrevBroAIPos();
    }
    else if (CurrentScene == SceneID::SCENE_LEVEL_EDITOR) {
        SetPrevEditor();
    }
}
void GameObjectDeltaMovement(const float dt) {
    if (CurrentScene == SceneID::SCENE_GAMEPLAY) {
        KeyboardMovement(dt);
        MarioPosXUpdate(dt);
        MarioVertXUpdate();
        MarioPosYUpdate(dt);
        MarioVertYUpdate();
        GoombaAIVertXUpdate(dt);
        GoombaAIVertYUpdate(dt);
        BroAIVertXUpdate(dt);
        BroAIVertYUpdate(dt);
        GoombaAIEffectVertYUpdate(dt);
        PiranhaAIMovementUpdate(dt);

        GoombaStatusUpdate(dt);
        GoombaAIEffectStatusUpdate(dt);
        CoinEffectStatusUpdate(dt);
        ScoreEffectStatusUpdate(dt);
        BrickParticleStatusUpdate(dt);
        MarioEffectStatusUpdate(dt);
        ExitGateStatusUpdate(dt);
        BrickUpdate(dt);
        LuckyBlockUpdate(dt);
    }
    else if (CurrentScene == SceneID::SCENE_LEVEL_EDITOR) {
        EditorScreenMove(dt);
        TilePosUpdate(dt);
        SelectTileAlphaUpdate(dt);
    }
}
void GameObjectRetrieveEvent(const std::optional<sf::Event>& event) {
    Window::WindowEventUpdate(event);
    if (CurrentScene == SceneID::SCENE_LEVEL_EDITOR) {
        EditorEvent(event);
    }
    else if (CurrentScene == SceneID::SCENE_GAMEPLAY) {
        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                AddBroAI(MouseX + view.getCenter().x - 320.0f, MouseY + view.getCenter().y - 240.0f);
                //AddGoombaAI(GoombaAIType::GOOMBA, 0, MouseX + view.getCenter().x - 320.0f, MouseY + view.getCenter().y - 240.0f, GoombaAIDirection::LEFT);
            }
        }
    }
}
void GameObjectInterpolateMovement(const float alpha) {
    if (CurrentScene == SceneID::SCENE_GAMEPLAY) {
        InterpolateMarioPos(alpha);
        InterpolateGoombaAIPos(alpha);
        InterpolateGoombaAIEffectPos(alpha);
        InterpolatePiranhaAIPos(alpha);
        InterpolateCoinEffectPos(alpha);
        InterpolateScoreEffectPos(alpha);
        InterpolateBrickParticlePos(alpha);
        InterpolateMarioEffectPos(alpha);
        InterpolateExitGatePos(alpha);
        InterpolateBricksPos(alpha);
        InterpolateLuckyBlockPos(alpha);
        InterpolateBroAIPos(alpha);
    }
    else if (CurrentScene == SceneID::SCENE_LEVEL_EDITOR) {
        InterpolateEditorPos(alpha);
    }
}
void GameObjectAnimation() {
    if (CurrentScene == SceneID::SCENE_GAMEPLAY) {
        MarioUpdateAnimation();
    }
}
void GameObjectCollision() {
    if (CurrentScene == SceneID::SCENE_GAMEPLAY) {
        GoombaAICollisionUpdate();
        GoombaAICheckCollide();
        CoinOnTouch();
        PiranhaAIStatusUpdate();
        MarioUpdateHitbox();
        SpikeStatusUpdate();
        CheckForDeath();
    }
}
void GameObjectMiscUpdate() {
    setView();
    updateFrame();
    updateView();
    UpdatePositionCharacter();
    if (CurrentScene == SceneID::SCENE_GAMEPLAY) {
        BgUpdatePos();
        HUDUpdate();
        //Update Position that stuck on scree
    }
    else if (CurrentScene == SceneID::SCENE_LEVEL_EDITOR) {
        BgGradientPosUpdate();
        SelectedTilePosUpdate();
        SelectTilePosUpdate();
    }
}
void GameObjectEditorUpdate() {
    if (CurrentScene == SceneID::SCENE_LEVEL_EDITOR) {
        PlaceTile();
    }
}
void GameObjectDraw() {
    BgGradientDraw();
    if (CurrentScene == SceneID::SCENE_GAMEPLAY) {
        BgDraw();
        ExitGateDraw();
        MarioDraw();
        GoombaAIUpdate();
        BroAIDraw();
        PiranhaAIUpdate();
        ObstaclesUpdate();
        SpikeUpdate();
        SlopeUpdate();
        CoinUpdate();
        CoinEffectUpdate();
        BrickDraw();
        LuckyBlockDraw();
        ScoreEffectUpdate();
        BrickParticleUpdate();
        GoombaAIEffectUpdate();
        MarioEffectDraw();
        ExitGateEffectDraw();
        FrameDraw();
    }
    else if (CurrentScene == SceneID::SCENE_LEVEL_EDITOR) {
        DrawTile();
        SelectTileDraw();
    }
    UpdateText();
}
