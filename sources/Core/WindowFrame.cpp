#include "Core/WindowFrame.hpp"

#include <config.h>
#include "Core/Scroll.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/ImageManager.hpp"
#include "Core/ExternalHeaders/Kairos.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Class/ActiveObjectClass.hpp"

#if defined _DEBUG
bool isDebug = true;
#else
bool isDebug = false;
#endif
float Width = GAME_WIDTH, Height = GAME_HEIGHT;
sf::RenderWindow window;
sf::RenderTexture rObject;
static bool OPTION_SMOOTH = false;
static bool OPTION_VSYNC = false;
static bool OPTION_FULLSCREEN = false;

static bool PREV_OPTION_FULLSCREEN = OPTION_FULLSCREEN;
static std::string PROGRAM_NAME = "Mario Forever";
static unsigned int OPTION_SCALE = 0;
sf::VideoMode x1({ static_cast<unsigned int>(Width), static_cast<unsigned int>(Height) });
sf::VideoMode x15({ static_cast<unsigned int>(Width * 1.5f), static_cast<unsigned int>(Height* 1.5f) });
sf::VideoMode x2({ static_cast<unsigned int>(Width * 2.0f), static_cast<unsigned int>(Height* 2.0f) });
const std::vector<sf::VideoMode> Resolutions = sf::VideoMode::getFullscreenModes();
std::random_device seed;

kairos::Stopwatch Gclock;
kairos::Timestep timestep;
kairos::FpsLite fpsLite;
float MouseX, MouseY;
static MFCPP::StaticAnimationObject MarioHUD;
static MFCPP::SingleAnimationObject CoinHUD;
static MFCPP::StaticAnimationObject TimeHUD;

int RandomIntNumberGenerator(const int a, const int b) {
	std::uniform_int_distribution<int> dis(a, b);
	return dis(seed);
}
float RandomFloatNumberGenerator(const float a, const float b) {
	std::uniform_real_distribution<float> dis(a, b);
	return dis(seed);
}
float f_mod(const float a, const float b) { return a - std::floor(a / b) * b;}
float f_min(const float a, const float b) { return a < b ? a : b; }
float f_max(const float a, const float b) { return a > b ? a : b; }
float f_abs(const float a) { return a < 0 ? -a : a; }
float f_round(const float val) {
	if (const float fl = std::trunc(val); val - fl < 0.5f) return fl;
	else return fl+1.0f;
}
float VectorGetLength(const sf::Vector2f& val) {
	return std::sqrt(val.x * val.x + val.y * val.y);
}
sf::Vector2f VectorNormalized(const sf::Vector2f& val) {
	if (const float len = VectorGetLength(val); len > 0)
		return sf::Vector2f(val.x / len, val.y / len);
	return sf::Vector2f(0.f, 0.f);
}
int hex_to_int(const std::string &hex) { return std::stoi(hex, nullptr, 16); }
static sf::Image icon;
static std::vector<std::string> CoinHUDAnimName;
static constexpr int COINHUD_IMAGE_WIDTH = 86;
static constexpr int COINHUD_WIDTH = 28;
static constexpr int COINHUD_HEIGHT = 16;

static sf::ContextSettings setting;
namespace Window {
	static sf::VideoMode WindowGetVideoMode() {
		if (!OPTION_FULLSCREEN) {
			switch (OPTION_SCALE) {
				case 0:
					return x1;
				case 1:
					return x1;
				case 2:
					return x15;
				case 3:
					return x2;
				default: ;
			}
		}
		else {
			return Resolutions[0];
		}
		return x1;
	}
	int WindowGetScale() { return OPTION_SCALE; }
	void WindowToggleFullscreen() {OPTION_FULLSCREEN = !OPTION_FULLSCREEN;}
	void WindowToggleSmooth() {OPTION_SMOOTH = !OPTION_SMOOTH;}
	void WindowSetSmooth(const bool val) {OPTION_SMOOTH = val;}
	void WindowSetFullscreen(const bool val) {OPTION_FULLSCREEN = val;}
	void WindowSetScale(const int val) {OPTION_SCALE = val % 4;}
	void ChangeScreenMode(const unsigned int mode) {
		// 0: native, 1: x1, 2: x1.5, 3: x2

		/*
		 * MODE
		 * 0: normal
		 * 1: force switch
		 * 2: force resize
		 *
		*/
		if (PREV_OPTION_FULLSCREEN != OPTION_FULLSCREEN) {
			window.create(WindowGetVideoMode(), PROGRAM_NAME, OPTION_FULLSCREEN ? sf::State::Fullscreen : sf::State::Windowed, setting );
		}
		else {
			if (mode == 1) window.create(WindowGetVideoMode(), PROGRAM_NAME, OPTION_FULLSCREEN ? sf::State::Fullscreen : sf::State::Windowed, setting);
			else if (mode == 2) {
				if (!OPTION_FULLSCREEN) window.setSize(WindowGetVideoMode().size);
				else window.create(WindowGetVideoMode(), PROGRAM_NAME, sf::State::Fullscreen, setting);
			}
		}
		PREV_OPTION_FULLSCREEN = OPTION_FULLSCREEN;
		window.setVerticalSyncEnabled(OPTION_VSYNC);
		window.setIcon(icon);
		if (!OPTION_SMOOTH) {
			window.setFramerateLimit(240);
			window.setVerticalSyncEnabled(false);
		}
		else {
			window.setFramerateLimit(0); //300
			window.setVerticalSyncEnabled(false);
		}
	}
	void WindowEventUpdate(const std::optional<sf::Event>& event) {
		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->code == sf::Keyboard::Key::F11) {
				WindowToggleFullscreen();
				ChangeScreenMode();
			}
			else if (keyPressed->code == sf::Keyboard::Key::F12) {
				WindowToggleSmooth();
				ChangeScreenMode();
			}
			else if (keyPressed->code == sf::Keyboard::Key::F10) {
				WindowSetScale(WindowGetScale() + 1);
				ChangeScreenMode(2);
			}
			else if (keyPressed->code == sf::Keyboard::Key::F9) {
				isInterpolation = !isInterpolation;
			}
		}
	}
}
void windowInit() {
	LoadImageFile(icon, "data/resources/Icon/GameICON.png");
	Window::ChangeScreenMode(1);
	timestep.setStep(1.0f / 500.0f);
	timestep.setMaxAccumulation(1.0f / 30.0f);
	//timestep.setTimeSpeed(0.5f);
}
void GameSceneInit() {
	ImageManager::AddTexture("MarioHUD", "data/resources/MarioHUD.png");
	ImageManager::AddTexture("TimeHUD", "data/resources/TimeHUD.png");
	for (int i = 0; i < COINHUD_IMAGE_WIDTH / COINHUD_WIDTH; i++) {
		ImageManager::AddTexture(fmt::format("CoinHUD_{}", i), "data/resources/CoinHUD.png", sf::IntRect({ i * COINHUD_WIDTH, 0 }, { COINHUD_WIDTH, COINHUD_HEIGHT }));
		CoinHUDAnimName.emplace_back(fmt::format("CoinHUD_{}", i));
	}
	CoinHUD.setAnimation(0, 2, 16);
	CoinHUD.setAnimationSequence(CoinHUDAnimName);
	MarioHUD.setTexture("MarioHUD");
	TimeHUD.setTexture("TimeHUD");
}
void FrameDraw() {
	CoinHUD.AnimationUpdate(sf::Vector2f(236.0f + ViewX, 15.0f + ViewY), sf::Vector2f(0.f, 0.f));
	CoinHUD.AnimationDraw();
	MarioHUD.AnimationUpdate(sf::Vector2f(35.0f + ViewX, 15.0f + ViewY), sf::Vector2f(0.f, 0.f));
	MarioHUD.AnimationDraw();
	TimeHUD.AnimationUpdate(sf::Vector2f(513.f + ViewX, 15.f + ViewY), sf::Vector2f(0.f, 0.f));
	TimeHUD.AnimationDraw();
}
void updateFrame() {
	const sf::Vector2i mouse = sf::Mouse::getPosition(window);
	MouseX = (static_cast<float>(mouse.x) - ViewXOff / 2.0f) * (Width / (static_cast<float>(window.getSize().x) - ViewXOff));
	MouseY = (static_cast<float>(mouse.y) - ViewYOff / 2.0f) * (Height / (static_cast<float>(window.getSize().y) - ViewYOff));
}