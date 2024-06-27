#include <SFML/Graphics.hpp>
#include "../headers/WindowFrame.hpp"
#include "../headers/AnimationManager.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/Mario.hpp"
#include "../headers/Loading.hpp"

#include "../resource.h"

#if defined _DEBUG
bool isDebug = true;
#else
bool isDebug = false;
#endif
float Width = 640.0f, Height = 480.0f;
sf::RenderWindow window(sf::VideoMode(Width, Height), "Mario Forever");
sf::Clock delta;
float deltaTime;
sf::Clock fpsClock;
sf::Clock GameClock;
float fps;
float optionSmoothness = false;
float MouseX, MouseY;
sf::Mouse mouse;

sf::Sprite CoinHUD;
sf::Texture CoinHUDTexture;
AnimationManager CoinHUDAnim;
int windowInit() {
	LoadTexture(CoinHUDTexture, COINHUD_TEXTURE, IMAGEFILE);
	CoinHUDAnim.addAnimation("IdleCoinHUD", &CoinHUDTexture, { 3,0 }, { 28,16 }, { 0,0 }, 16, { 0,0 }, { 3,0 });
	return 6;
}
int initWin = windowInit();
void updateFrame() {
	MouseX = mouse.getPosition(window).x * (Width / window.getSize().x);
	MouseY = mouse.getPosition(window).y * (Height / window.getSize().y);
	if (!optionSmoothness) window.setFramerateLimit(50);
	else window.setFramerateLimit(10000); //300
	deltaTime = delta.restart().asSeconds() * 50.0f;
	float fpsUpdate = 1.0f / fpsClock.restart().asSeconds();
	if (GameClock.getElapsedTime().asSeconds() >= 0.5f) {
		fps = fpsUpdate;
		GameClock.restart().asSeconds();
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