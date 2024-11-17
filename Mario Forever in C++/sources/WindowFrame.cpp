#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Core/Animate/AnimationManager.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Object/Mario.hpp"
#include "../headers/Core/Loading/Loading.hpp"

#include "../resource.h"

#include "../headers/Core/MultiThreading/ThreadPool.hpp"
#include "../headers/Core/TextureManager.hpp"

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

sf::Sprite MarioHUD;

ThreadPool Thread_Pool;
TextureManager Maintexture;

AnimationManager CoinHUDAnim;
float f_min(float a, float b) { return a < b ? a : b; }
float f_max(float a, float b) { return a > b ? a : b; }
float f_abs(float a) { return a < 0 ? -a : a; }
void windowInit() {
	sf::Texture* Temp = new sf::Texture();
	LoadTexture(*Temp, MARIOHUD_TEXTURE);
	Maintexture.AddTexture("MarioHUD", Temp);
	LoadTexture(CoinHUDTexture, COINHUD_TEXTURE);
	CoinHUDAnim.addAnimation("IdleCoinHUD", &CoinHUDTexture, { 3,0 }, { 28,16 }, { 0,0 }, 16, { 0,0 }, { 3,0 });
	Temp = new sf::Texture();
	delete Temp;
}
void updateFrame() {
	MouseX = (mouse.getPosition(window).x - ViewXOff / 2.0f) * (Width / (window.getSize().x - ViewXOff));
	MouseY = (mouse.getPosition(window).y - ViewYOff / 2.0f) * (Height / (window.getSize().y - ViewYOff));
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
	//MarioHUD
	MarioHUD.setPosition(35.0f + ViewX, 15.0f + ViewY);
	MarioHUD.setTexture(*Maintexture.GetTexture("MarioHUD"));
	window.draw(CoinHUD);
	window.draw(MarioHUD);
}