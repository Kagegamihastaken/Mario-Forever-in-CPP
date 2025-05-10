#include <SFML/Graphics.hpp>
#include "headers/Core/WindowFrame.hpp"
#include "headers/Object/Mario.hpp"
#include "headers/Block/Obstacles.hpp"
#include "headers/Core/Level.hpp"
#include "headers/Text/Text.hpp"
#include "headers/Core/Scroll.hpp"
#include "headers/Object/Coin.hpp"
#include "headers/Effect/CoinEffect.hpp"
#include "headers/Block/Brick.hpp"
#include "headers/Block/LuckyBlock.hpp"
#include "headers/Effect/ScoreEffect.hpp"
#include "headers/Effect/BrickParticle.hpp"
#include "headers/Object/GoombaAI.hpp"
#include "headers/Effect/GoombaAIEffect.hpp"
#include "headers/Core/Loading/enum.hpp"
#include "headers/Block/Slopes.hpp"
#include "headers/Core/Sound.hpp"
#include "headers/Object/PiranhaAI.hpp"
#include "headers/Object/Spike.hpp"
#include "headers/Core/Music.hpp"
#include "headers/Effect/MarioEffect.hpp"
#include "headers/Core/Background/BgGradient.hpp"
#include "headers/Core/Background/Bg.hpp"
#include "headers/Object/ExitGate.hpp"
#include "headers/Core/Interpolation.hpp"
#include "headers/Core/Loading/Loading.hpp"

#include "headers/Core/ExternalHeaders/Kairos.hpp"

#include <iostream>
#include <string>
sf::Clock test;
sf::Sprite Renderer(tempTex);
float alphainter = 1.0f;

int main() {
	IOInit();

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
	//window.setTitle
	//Init window
	ViewInit();
	//set level data
	ReadData("data/levels/lvl1.txt");
	//For program
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
	//build a level
	Bgbuilding();
	Obstaclebuilding();
	Slopebuilding();
	Objectbuilding();
	BgGradientInitPos();
	ExitGateBuilding();
	std::string fall, appe;
	//looping frame
	while (window.isOpen()) {
		test.restart();
		// process events
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				Sounds.ClearUp();
				window.close();
				IODeinit();
			}
		}
		if (ExitGateClock.getElapsedTime().asSeconds() > 8.5f && !EffectActive) {
			ExitGateClock.reset();
			Sounds.ClearUp();
			window.close();
			IODeinit();
		}
		//update: Mario
		fall = (MarioCrouchDown ? "TRUE" : "FALSE");
		appe = (MarioAppearing ? "TRUE" : "FALSE");
		//EditText("DeltaTime: " + std::to_string(deltaTime), "_DELTA");
		EditText(std::to_string(Lives), "_LIVE");
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
			EditText(std::to_string((int)MouseX) + "/" + std::to_string((int)MouseY) + "  R", "_MOUSEXY");
			EditText(std::to_string((int)ViewX) + "/" + std::to_string((int)ViewY) + "  V", "_VIEWXY");
			EditText("Appear: " + appe, "_APPE");
		}
		EditText("FPS: " + std::to_string((int)fpsLite.getFps()), "_FPS");
		EditText(std::to_string(CoinCount), "_COIN");
		EditText(std::to_string(Score), "_SCORE");

		fpsLite.update();

		timestep.addFrame();
		//timestep.setTimeSpeed(0.5f);
		while (timestep.isUpdateRequired()) {
			float dt{ timestep.getStepAsFloat() * 50.0f };
			//Interpolate process
			//set previous position
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
			//deltaTime movement
			KeyboardMovement(dt);
			MarioPosXUpdate(dt);
			MarioVertXUpdate();
			MarioPosYUpdate(dt);
			MarioVertYUpdate();

			GoombaAIVertXUpdate(dt);
			GoombaAIVertYUpdate(dt);
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
		//interpolate
		if (isInterpolation) alphainter = timestep.getInterpolationAlphaAsFloat();
		else alphainter = 1.0f;

		InterpolateMarioPos(alphainter);
		InterpolateGoombaAIPos(alphainter);
		InterpolateGoombaAIEffectPos(alphainter);
		InterpolatePiranhaAIPos(alphainter);
		InterpolateCoinEffectPos(alphainter);
		InterpolateScoreEffectPos(alphainter);
		InterpolateBrickParticlePos(alphainter);
		InterpolateMarioEffectPos(alphainter);
		InterpolateExitGatePos(alphainter);
		InterpolateBricksPos(alphainter);
		InterpolateLuckyBlockPos(alphainter);
		//After interpolate
		UpdateAnimation();

		GoombaAICollisionUpdate();
		GoombaAICheckCollide();
		CoinOnTouch();
		LuckyAnimationUpdate();
		BrickStatusUpdate();
		PiranhaAIStatusUpdate();
		SpikeStatusUpdate();

		//set view
		setView();
		//update text position
		//Update mario animation
		//core code
		rTexture.clear();
		//resetDelta();
		updateFrame();
		updateView();
		//Update Position that stuck on screen
		UpdatePositionCharacter();
		BgUpdatePos();
		CheckForDeath();
		//draw
		BgGradientDraw();
		BgDraw();
		ExitGateDraw();
		MarioDraw();
		GoombaAIUpdate();
		PiranhaAIUpdate();
		ObstaclesUpdate();
		SpikeUpdate();
		SlopeUpdate();
		CoinUpdate();
		BrickDraw();
		LuckyBlockDraw();
		CoinEffectUpdate();
		ScoreEffectUpdate();
		BrickParticleUpdate();
		GoombaAIEffectUpdate();
		MarioEffectDraw();
		ExitGateEffectDraw();
		UpdateText();
		FrameDraw();

		rTexture.display();
		window.clear();
		Renderer.setTexture(rTexture.getTexture(), true);
		window.draw(Renderer);
		//display
		window.display();
	}
}