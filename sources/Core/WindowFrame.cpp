#include "Core/WindowFrame.hpp"

#include <config.h>
#include "Core/Scroll.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/ImageManager.hpp"
#include "Core/ExternalHeaders/Kairos.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"

#if defined _DEBUG
bool WindowFrame::_Debug = true;
#else
bool WindowFrame::_Debug = false;
#endif

sf::Vector2f WindowFrame::GameSize(GAME_WIDTH, GAME_HEIGHT);
sf::VideoMode WindowFrame::Window::x1({ static_cast<uint32_t>(GameSize.x), static_cast<uint32_t>(GameSize.y) });
sf::VideoMode WindowFrame::Window::x15({ static_cast<uint32_t>(GameSize.x * 1.5f), static_cast<uint32_t>(GameSize.y * 1.5f) });
sf::VideoMode WindowFrame::Window::x2({ static_cast<uint32_t>(GameSize.x * 2.0f), static_cast<uint32_t>(GameSize.y * 2.0f) });
const std::vector<sf::VideoMode> WindowFrame::Window::Resolutions = sf::VideoMode::getFullscreenModes();

bool WindowFrame::Window::OPTION_SMOOTH = false;
bool WindowFrame::Window::OPTION_VSYNC = false;
bool WindowFrame::Window::OPTION_FULLSCREEN = false;
bool WindowFrame::Window::PREV_OPTION_FULLSCREEN = OPTION_FULLSCREEN;
const std::string WindowFrame::Window::PROGRAM_NAME = "Mario Forever - Test";
uint8_t WindowFrame::Window::OPTION_SCALE = 0;
sf::RenderWindow WindowFrame::window;
sf::Image WindowFrame::icon;

kairos::Timestep WindowFrame::timestep;
kairos::FpsLite WindowFrame::fpsLite;
sf::Vector2f WindowFrame::MousePosition;

static std::vector<std::string> CoinHUDAnimName;
static constexpr uint16_t COINHUD_IMAGE_WIDTH = 86;
static constexpr uint16_t COINHUD_WIDTH = 28;
static constexpr uint16_t COINHUD_HEIGHT = 16;

MFCPP::StaticAnimationObject WindowFrame::MarioHUD;
MFCPP::SingleAnimationObject WindowFrame::CoinHUD;
MFCPP::StaticAnimationObject WindowFrame::TimeHUD;

static sf::ContextSettings setting;
sf::VideoMode WindowFrame::Window::WindowGetVideoMode() {
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
uint8_t WindowFrame::Window::WindowGetScale() {
	return OPTION_SCALE;
}
void WindowFrame::Window::WindowToggleFullscreen() {
	OPTION_FULLSCREEN = !OPTION_FULLSCREEN;
}
void WindowFrame::Window::WindowToggleSmooth() {
	OPTION_SMOOTH = !OPTION_SMOOTH;
}
void WindowFrame::Window::WindowSetSmooth(const bool val) {
	OPTION_SMOOTH = val;
}
void WindowFrame::Window::WindowSetFullscreen(const bool val) {
	OPTION_FULLSCREEN = val;
}
void WindowFrame::Window::WindowSetScale(const uint8_t val) {
	OPTION_SCALE = val % 4;
}
void WindowFrame::Window::ChangeScreenMode(const uint8_t mode) {
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
		//window.setVerticalSyncEnabled(false);
	}
	else {
		window.setFramerateLimit(0);
		//window.setVerticalSyncEnabled(false);
	}
}
void WindowFrame::Window::WindowEventUpdate(const std::optional<sf::Event>& event) {
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

bool WindowFrame::isDebug() {
	return _Debug;
}

sf::RenderWindow &WindowFrame::getWindow() {
	return window;
}

kairos::Timestep &WindowFrame::getTimestep() {
	return timestep;
}

kairos::FpsLite &WindowFrame::getFpsLite() {
	return fpsLite;
}

sf::Vector2f WindowFrame::getMousePosition() {
	return MousePosition;
}

sf::Vector2f WindowFrame::getGameSize() {
	return GameSize;
}

void WindowFrame::WindowInit() {
	LoadImageFile(icon, "data/resources/Icon/GameICON.png");
	Window::ChangeScreenMode(1);
	timestep.setStep(1.0f / 500.0f);
	//timestep.setMaxAccumulation(1.0f / 30.0f);
}
void WindowFrame::GameSceneInit() {
	ImageManager::AddTexture("MarioHUD", "data/resources/MarioHUD.png");
	ImageManager::AddTexture("TimeHUD", "data/resources/TimeHUD.png");
	for (int i = 0; i < COINHUD_IMAGE_WIDTH / COINHUD_WIDTH; i++) {
		ImageManager::AddTexture(fmt::format("CoinHUD_{}", i), "data/resources/CoinHUD.png", sf::IntRect({ i * COINHUD_WIDTH, 0 }, { COINHUD_WIDTH, COINHUD_HEIGHT }));
		CoinHUDAnimName.emplace_back(fmt::format("CoinHUD_{}", i));
	}
	CoinHUD.setAnimation(0, 2, 16, true);
	CoinHUD.setAnimationSequence(CoinHUDAnimName);
	MarioHUD.setTexture("MarioHUD");
	TimeHUD.setTexture("TimeHUD");
}
void WindowFrame::FrameDraw() {
	CoinHUD.animationUpdate(sf::Vector2f(236.0f + ViewX, 15.0f + ViewY), sf::Vector2f(0.f, 0.f));
	CoinHUD.animationDraw();
	MarioHUD.animationUpdate(sf::Vector2f(35.0f + ViewX, 15.0f + ViewY), sf::Vector2f(0.f, 0.f));
	MarioHUD.animationDraw();
	TimeHUD.animationUpdate(sf::Vector2f(513.f + ViewX, 15.f + ViewY), sf::Vector2f(0.f, 0.f));
	TimeHUD.animationDraw();
}
void WindowFrame::updateFrame() {
	const sf::Vector2i mouse = sf::Mouse::getPosition(window);
	MousePosition.x = (static_cast<float>(mouse.x) - ViewXOff / 2.0f) * (GameSize.x / (static_cast<float>(window.getSize().x) - ViewXOff));
	MousePosition.y = (static_cast<float>(mouse.y) - ViewYOff / 2.0f) * (GameSize.y / (static_cast<float>(window.getSize().y) - ViewYOff));
}