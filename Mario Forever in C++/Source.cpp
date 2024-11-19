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

#include "headers/Core/sfMod/sfMod.hpp"
#include "resource.h"
#include "headers/Core/Loading/Loading.hpp"

#include <modplug.h>

#include <SFML/Audio/SoundStream.hpp>

#include <iostream>
#include <string>
#include <fstream>
int main() {
	//Init Games:
	windowInit();
	SoundInit();
	loadObstacleRes();
	BrickParticleInit();
	loadFontRes();
	LoadBricks();
	LoadLuckyBlock();
	GoombaAILoadRes();
	CoinEffectInit();
	GoombaAIEffectInit();
	PiranhaAIInit();
	SpikeInit();
	//Init music
	//sfmod::Mod test;
	//LoadMOD(test, TEST_MUSIC, 14, 44100);
	//test.setLoop(true);
	//test.play();
	//window.setTitle
	//Init window
	ViewInit();
	//set level data
	ReadData(LVL1);
	//For program
	AddText("_DEBUG", (isDebug ? "DEBUG" : "RELEASE"), LEFT_MARGIN, 0.0f, 464.0f);
	AddText("_COIN", "", RIGHT_MARGIN, 287.0f, 15.0f);
	AddText("_LIVE", "", LEFT_MARGIN, 138.0f, 15.0f);
	AddText("_SCORE", "", RIGHT_MARGIN, 138.0f, 34.0f);
	AddText("_FPS", "", LEFT_MARGIN, 0.0f, 448.0f);
	AddText("_DELTA", "", LEFT_MARGIN, 0, 432.0f);
	if (isDebug) {
		AddText("_MOUSEXY", "", RIGHT_MARGIN, 624.0f, 464.0f);
		AddText("_MARIOXY", "", RIGHT_MARGIN, 624.0f, 448.0f);
		AddText("_VIEWXY", "", RIGHT_MARGIN, 624.0f, 432.0f);
		AddText("_CODX", "", RIGHT_MARGIN, 624.0f, 416.0f);
		AddText("_CODY", "", RIGHT_MARGIN, 624.0f, 400.0f);
		AddText("_FALL", "", LEFT_MARGIN, 0.0f, 48.0f);
		AddText("_APPE", "", LEFT_MARGIN, 0.0f, 64.0f);
		AddText("_FALLING", "", LEFT_MARGIN, 0.0f, 80.0f);
	}
	//build a level
	Obstaclebuilding();
	Slopebuilding();
	Objectbuilding();
	std::pair<bool, bool> Test;
	std::string fall, appe;
	//looping frame
	while (window.isOpen()) {
		// process events
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				Sounds.ClearUp();
				Thread_Pool.Stop();
				window.close();
			}
		}
		//update: Mario
		fall = (MarioCrouchDown ? "TRUE" : "FALSE");
		appe = (MarioAppearing ? "TRUE" : "FALSE");
		EditText("DeltaTime: " + std::to_string(deltaTime), "_DELTA");
		EditText(std::to_string(Lives), "_LIVE");
		if (isDebug) {
			EditText(std::to_string((int)MouseX) + "/" + std::to_string((int)MouseY) + "  R", "_MOUSEXY");
			EditText(std::to_string((int)player.property.getPosition().x) + "/" + std::to_string((int)player.property.getPosition().y) + "  M", "_MARIOXY");
			EditText(std::to_string((int)ViewX) + "/" + std::to_string((int)ViewY) + "  V", "_VIEWXY");
			EditText(std::to_string(Xvelo) + " VX", "_CODX");
			EditText(std::to_string(Yvelo) + " VY", "_CODY");
			EditText("Crouch Down: " + fall, "_FALL");
			EditText("Appear: " + appe, "_APPE");
			EditText("Falling: " + (MarioCurrentFalling ? std::string("TRUE") : std::string("FALSE")), "_FALLING");
		}
		EditText("FPS: " + std::to_string((int)fps), "_FPS");
		EditText(std::to_string(CoinCount), "_COIN");
		EditText(std::to_string(Score), "_SCORE");
		KeyboardMovement();
		MarioVertXUpdate();
		MarioVertYUpdate();
		//Update movement other than mario
		GoombaAICollisionUpdate();
		GoombaAIVertXUpdate();
		GoombaAIVertYUpdate();
		GoombaAICheckCollide();
		GoombaAIEffectVertYUpdate();
		PiranhaAIMovementUpdate();
		//Check other event
		CoinOnTouch();
		CoinEffectStatusUpdate();
		LuckyAnimationUpdate();
		BrickStatusUpdate();
		ScoreEffectStatusUpdate();
		BrickParticleStatusUpdate();
		GoombaStatusUpdate();
		GoombaAIEffectStatusUpdate();
		PiranhaAIStatusUpdate();
		SpikeStatusUpdate();
		//set view
		setView();
		//update text position
		UpdatePositionCharacter();
		//Update mario animation
		UpdateAnimation();
		//core code
		window.clear(sf::Color::Black);
		updateFrame();
		//draw
		updateView();
		MarioDraw();
		GoombaAIUpdate();
		PiranhaAIUpdate();
		SpikeUpdate();
		ObstaclesUpdate();
		SlopeUpdate();
		CoinUpdate();
		BrickUpdate();
		LuckyBlockUpdate();
		CoinEffectUpdate();
		ScoreEffectUpdate();
		BrickParticleUpdate();
		GoombaAIEffectUpdate();
		UpdateText();
		//display
		CheckForDeath();
		window.display();
	}
}