#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../headers/WindowFrame.hpp"
#include "../headers/AnimationManager.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/Mario.hpp"
#include "../headers/Loading.hpp"

#include "../resource.h"

#include "../headers/ThreadPool.hpp"

#if defined _DEBUG
bool isDebug = true;
#else
bool isDebug = false;
#endif
float Width = 640.0f, Height = 480.0f;
sf::RenderWindow window(sf::VideoMode((unsigned int)(Width), (unsigned int)(Height)), "Mario Forever");
sf::Clock delta;
float deltaTime;
sf::Clock fpsClock;
sf::Clock GameClock;
float fps;
int optionSmoothness = 1;
int previousUpdate = 2;

float MouseX, MouseY;
sf::Mouse mouse;

sf::Sprite CoinHUD;
sf::Texture CoinHUDTexture;

ThreadPool Thread_Pool;

AnimationManager CoinHUDAnim;
float f_min(float a, float b) { return a < b ? a : b; }
float f_max(float a, float b) { return a > b ? a : b; }
int windowInit() {
	LoadTexture(CoinHUDTexture, COINHUD_TEXTURE);
	CoinHUDAnim.addAnimation("IdleCoinHUD", &CoinHUDTexture, { 3,0 }, { 28,16 }, { 0,0 }, 16, { 0,0 }, { 3,0 });
	return 6;
}
int initWin = windowInit();
void updateFrame() {
	MouseX = mouse.getPosition(window).x * (Width / window.getSize().x);
	MouseY = mouse.getPosition(window).y * (Height / window.getSize().y);
	if (previousUpdate == 2) {
		if (optionSmoothness) window.setFramerateLimit(50);
		else window.setFramerateLimit(10000); //300
		previousUpdate = optionSmoothness;
	}
	else if (previousUpdate != optionSmoothness) {
		if (optionSmoothness) window.setFramerateLimit(50);
		else window.setFramerateLimit(10000); //300
	}
	deltaTime = delta.restart().asSeconds() * 50.0f;
	float fpsUpdate = 1.0f / fpsClock.restart().asSeconds();
	if (GameClock.getElapsedTime().asSeconds() >= 0.5f) {
		if (deltaTime >= 1.0f) {
			fps = fpsUpdate;
			GameClock.restart().asSeconds();
		}
		else {
			fps = (1.0f / deltaTime) * 50.0f;
			GameClock.restart().asSeconds();
		}
	}
	if (deltaTime >= 1.0f) {
		deltaTime = 1.0f;
		optionSmoothness = false;
	}
	else optionSmoothness = true;
	//CoinHUD
	CoinHUDAnim.update("IdleCoinHUD", CoinHUD);
	CoinHUD.setPosition(236.0f + ViewX, 15.0f + ViewY);
	window.draw(CoinHUD);
}