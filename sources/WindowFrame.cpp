#include <SFML/Graphics.hpp>
#include "Core/WindowFrame.hpp"
#include "Core/Scroll.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/ImageManager.hpp"
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

LocalAnimationManager CoinHUDAnim;
float f_min(const float a, const float b) { return a < b ? a : b; }
float f_max(const float a, const float b) { return a > b ? a : b; }
float f_abs(const float a) { return a < 0 ? -a : a; }
float f_round(const float val) {
	if (const float fl = std::trunc(val); val - fl < 0.5f) return fl;
	else return fl+1.0f;
}
int hex_to_int(const std::string &hex) { return std::stoi(hex, nullptr, 16); }

static std::vector<std::string> CoinHUDAnimName;
static constexpr int COINHUD_IMAGE_WIDTH = 86;
static constexpr int COINHUD_WIDTH = 28;
static constexpr int COINHUD_HEIGHT = 16;

void windowInit() {
	// Create Window
	window.create(videoMode, "Mario Forever");

	sf::Image icon;
	LoadImageFile(icon, "data/resources/Icon/GameICON.png");

	ImageManager::AddImage("MarioHUDImage", "data/resources/MarioHUD.png");
	ImageManager::AddTexture("MarioHUDImage", "MarioHUD");
	ImageManager::AddImage("CoinHUDImage", "data/resources/CoinHUD.png");
	for (int i = 0; i < COINHUD_IMAGE_WIDTH / COINHUD_WIDTH; i++) {
		ImageManager::AddTexture("CoinHUDImage", sf::IntRect({ i * COINHUD_WIDTH, 0 }, { COINHUD_WIDTH, COINHUD_HEIGHT }), "CoinHUD_" + std::to_string(i));
		CoinHUDAnimName.emplace_back("CoinHUD_" + std::to_string(i));
	}
	CoinHUD.setTexture(ImageManager::GetTexture("CoinHUD_0"), true);
	CoinHUDAnim.setAnimation(0, 2, 16);
	CoinHUDAnim.SetSequence(CoinHUDAnimName, CoinHUDAnimName);
	//Maintexture.AddTexture("MarioHUD", Temp);
	MarioHUD.setTexture(ImageManager::GetTexture("MarioHUD"), true);
	window.setIcon(icon);
	//rObject.setRepeated(true);
	//window.setVerticalSyncEnabled(true);
	timestep.setStep(1.0f / 50.0f);
	timestep.setMaxAccumulation(1.0f / 30.0f);
}
void HUDUpdate() {
	CoinHUD.setPosition(sf::Vector2f(236.0f + ViewX, 15.0f + ViewY));
	CoinHUDAnim.AnimationUpdate(CoinHUD.getPosition(), CoinHUD.getOrigin());
	MarioHUD.setPosition(sf::Vector2f(35.0f + ViewX, 15.0f + ViewY));
}
void FrameDraw() {
	CoinHUDAnim.AnimationDraw(window);
	window.draw(MarioHUD);
}
void updateFrame() {
	const sf::Vector2i mouse = sf::Mouse::getPosition(window);
	MouseX = (static_cast<float>(mouse.x) - ViewXOff / 2.0f) * (Width / (static_cast<float>(window.getSize().x) - ViewXOff));
	MouseY = (static_cast<float>(mouse.y) - ViewYOff / 2.0f) * (Height / (static_cast<float>(window.getSize().y) - ViewYOff));
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