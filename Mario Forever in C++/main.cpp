#include <SFML/Graphics.hpp>
#include "headers/WindowFrame.hpp"
#include "headers/Mario.hpp"
#include "headers/Obstacles.hpp"
#include "headers/Level.hpp"
#include "headers/Text.hpp"
#include "headers/Scroll.hpp"
#include "headers/Coin.hpp"
#include "headers/CoinEffect.hpp"
#include "headers/Brick.hpp"
#include "headers/LuckyBlock.hpp"
#include "headers/ScoreEffect.hpp"

#include <iostream>
#include <string>
#include <fstream>

int main() {
	//Init window
	ViewInit();
	//set level data
	std::ifstream Level("levels/lvl1.txt");
	ReadData(Level);
	//For program
	AddText((isDebug ? "DEBUG" : "RELEASE"), 0.0f, 464.0f, "_DEBUG");
	AddText(287.0f, 15.0f, "_COIN");
	AddText(0, 448.0f, "_SCORE");
	AddText(0, 16.0f, "_FPS");
	if (isDebug) {
		AddText(0, 32.0f, "_DELTA");
		AddText(0, 48.0f, "_MOUSEXY");
		AddText(0, 64.0f, "_MARIOXY");
		AddText(0, 80.0f, "_VIEWXY");
		AddText(0, 96.0f, "_CODX");
		AddText(0, 112.0f, "_FALL");
	}
	//build a level
	building();
	// set position of mario
	player.property.setPosition({ 100.0f, 200.0f });
	std::pair<bool, bool> Test;
	std::string fir, se, fall;
	//looping frame
	while (window.isOpen()) {
		// process events
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//update: Mario
		fall = (MarioCurrentFalling ? "TRUE" : "FALSE");
		if (isDebug) {
			EditText("DeltaTime: " + std::to_string(deltaTime), "_DELTA");
			EditText("mouse: " + std::to_string((int)MouseX) + "/" + std::to_string((int)MouseY), "_MOUSEXY");
			EditText("mario: " + std::to_string((int)player.property.getPosition().x) + "/" + std::to_string((int)player.property.getPosition().y), "_MARIOXY");
			EditText("view: " + std::to_string((int)ViewX) + "/" + std::to_string((int)ViewY), "_VIEWXY");
			EditText("Xvelocity: " + std::to_string(Xvelo), "_CODX");
			EditText("CurrFallBool: " + fall, "_FALL");
		}
		EditText("FPS: " + std::to_string((int)fps), "_FPS");
		EditText(std::to_string(CoinCount), "_COIN");
		if (CoinCount > 9) EditPosition(272.0f, 15.0f, "_COIN");
		else EditPosition(287.0f, 15.0f, "_COIN");
		EditText(std::to_string(Score), "_SCORE");
		KeyboardMovement();
		MarioVertXUpdate();
		MarioVertYUpdate();
		//Check bonus event
		CoinOnTouch();
		CoinEffectStatusUpdate();
		LuckyAnimationUpdate();
		BrickStatusUpdate();
		ScoreEffectStatusUpdate();
		// set current view of mario
		setView();
		//Window close
		//update text position
		UpdatePositionCharacter();
		//Update mario animation
		UpdateAnimation();
		//core code
		window.clear(sf::Color::White);
		updateFrame();
		//draw
		updateView();
		MarioDraw();
		ScoreEffectUpdate();
		CoinUpdate();
		CoinEffectUpdate();
		BrickUpdate();
		LuckyBlockUpdate();
		ObstaclesUpdate();
		DrawText();
		//display
		CheckForDeath();
		window.display();
	}
}