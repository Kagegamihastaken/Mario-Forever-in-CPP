#include <SFML/Graphics.hpp>
#include "headers/WindowFrame.hpp"
#include "headers/Mario.hpp"
#include "headers/Obstacles.hpp"
#include "headers/Level.hpp"
#include "headers/Text.hpp"
#include "headers/Scroll.hpp"
#include "headers/Coin.hpp"

#include <iostream>
#include <string>
#include <fstream>

int main() {
	//Init things
	for (int i = 0; i <= 20; ++i) {
		for (int j = 0; j <= 12; ++j) {
			addCoin(320.0f - 5 * 32.0f + 32.0f * i, 0.0f + 32.0f * j);
		}
	}
	ViewInit();
	std::ifstream Level("levels/lvl1.txt");
	ReadData(Level);
	//For program
	AddText((isDebug ? "DEBUG" : "RELEASE"), 0.0f, 0.0f, "_DEBUG");
	AddText(0, 448.0f + 16.0f, "_COIN");
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
		Test = CheckTest();
		fir = (Test.first ? "TRUE" : "FALSE");
		se = (Test.second ? "TRUE" : "FALSE");
		fall = (MarioCurrentFalling ? "TRUE" : "FALSE");
		if (isDebug) {
			EditText("DeltaTime: " + std::to_string(deltaTime), "_DELTA");
			EditText("mouse: " + std::to_string((int)MouseX) + "/" + std::to_string((int)MouseY), "_MOUSEXY");
			EditText("mario: " + std::to_string((int)player.property.getPosition().x) + "/" + std::to_string((int)player.property.getPosition().y), "_MARIOXY");
			EditText("view: " + std::to_string((int)ViewX) + "/" + std::to_string((int)ViewY), "_VIEWXY");
			EditText("Xvelocity: " + std::to_string(Xvelo), "_CODX");
			EditText("CurrentFall: " + fall, "_FALL");
		}
		EditText("FPS: " + std::to_string((int)fps), "_FPS");
		EditText("Coin x " + std::to_string(CoinCount), "_COIN");
		KeyboardMovement();
		MarioVertXUpdate();
		MarioVertYUpdate();
		//Check event coin
		CoinOnTouch();
		// set current view of mario
		setView();
		//Window close
		//update text position
		UpdatePositionCharacter();
		UpdateAnimation();
		//core code
		window.clear(sf::Color::Black);
		updateFrame();
		//draw
		updateView();
		MarioDraw();
		CoinUpdate();
		ObstaclesUpdate();
		DrawText();
		//display
		CheckForDeath();
		window.display();
	}
}