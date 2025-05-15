#include <SFML/Graphics.hpp>
#include "Core/WindowFrame.hpp"
#include "Core/Scroll.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/TextureManager.hpp"
#include "Core/ExternalHeaders/Kairos.hpp"
#include "Core/Animate/LocalAnimationManager.hpp"

#if defined _DEBUG
bool isDebug = true;
#else
bool isDebug = false;
#endif
float Width = 640.0f, Height = 480.0f;
sf::VideoMode videoMode({ static_cast<unsigned int>(Width), static_cast<unsigned int>(Height) });
sf::RenderWindow window;
sf::RenderTexture rObject({ static_cast<unsigned int>(Width), static_cast<unsigned int>(Height) });
int optionSmoothness = 1;
int previousUpdate = 2;

kairos::Stopwatch Gclock;
kairos::Timestep timestep;

kairos::FpsLite fpsLite;

float MouseX, MouseY;

sf::Sprite CoinHUD(tempTex);
sf::Texture CoinHUDTexture;

sf::Sprite MarioHUD(tempTex);
TextureManager Maintexture;

LocalAnimationManager CoinHUDAnim;
float f_min(const float a, const float b) { return a < b ? a : b; }
float f_max(const float a, const float b) { return a > b ? a : b; }
float f_abs(const float a) { return a < 0 ? -a : a; }
int hex_to_int(const std::string &hex) { return std::stoi(hex, nullptr, 16); }
void windowInit() {
	// Create Window
	window.create(videoMode, "Mario Forever");

	sf::Image icon;
	LoadImageFile(icon, "data/resources/Icon/GameICON.png");
	Maintexture.Loadingtexture("data/resources/MarioHUD.png", "MarioHUD", 0, 0, 97, 16);
	Maintexture.Loadingtexture("data/resources/CoinHUD.png", "CoinHUDTexture", 0, 0, 86, 16);
	CoinHUD.setTexture(*Maintexture.GetTexture("CoinHUDTexture"), true);
	CoinHUDAnim.setAnimation(0, 2, 28, 16, 0, 16 );
	//Maintexture.AddTexture("MarioHUD", Temp);
	MarioHUD.setTexture(*Maintexture.GetTexture("MarioHUD"), true);
	window.setIcon(icon);
	//window.setVerticalSyncEnabled(true);

	timestep.setStep(1.0f / 2000.0f);
	timestep.setMaxAccumulation(1.0f / 40.0f);
}
void FrameDraw() {
	//CoinHUD
	CoinHUDAnim.update(CoinHUD);
	CoinHUD.setPosition(sf::Vector2f(236.0f + ViewX, 15.0f + ViewY));
	//MarioHUD
	MarioHUD.setPosition(sf::Vector2f(35.0f + ViewX, 15.0f + ViewY));
	rObject.draw(CoinHUD);
	rObject.draw(MarioHUD);
}
void updateFrame() {
	const sf::Vector2i mouse = sf::Mouse::getPosition(window);
	MouseX = (mouse.x - ViewXOff / 2.0f) * (Width / (window.getSize().x - ViewXOff));
	MouseY = (mouse.y - ViewYOff / 2.0f) * (Height / (window.getSize().y - ViewYOff));
	if (previousUpdate == 2) {
		if (!optionSmoothness) window.setFramerateLimit(50);
		else window.setFramerateLimit(0); //300
		previousUpdate = optionSmoothness;
	}
	else if (previousUpdate != optionSmoothness) {
		if (!optionSmoothness) window.setFramerateLimit(50);
		else window.setFramerateLimit(0); //300
	}
	//deltaTime = delta.restart().asSeconds() * 50.0f;
	//deltaTime = (delta.restart().asMicroseconds() * 50) / 1000000.0f;
	//float fpsUpdate = 1.0f / fpsClock.restart().asSeconds();
	//if (GameClock.getElapsedTime().asSeconds() >= 0.5f) {
	//	if (deltaTime >= 1.0f) {
	//		fps = fpsUpdate;
	//		GameClock.restart();
	//	}
	//	else {
	//		fps = (1.0f / deltaTime) * 50.0f;
	//		GameClock.restart();
	//	}
	//}
	//if (deltaTime >= 1.0f) {
	//	deltaTime = 1.0f;
	//	optionSmoothness = false;
	//}
	//else optionSmoothness = true;
}