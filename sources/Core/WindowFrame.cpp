#include "Core/WindowFrame.hpp"

#include "Core/AnimationSequenceManager.hpp"
#include "Core/Scroll.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Logging.hpp"
#include "Core/ExternalHeaders/Kairos.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Profiler.hpp"

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
void WindowFrame::Window::ChangeScreenMode(uint8_t mode) {
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
		window.setFramerateLimit(60);
		//window.setVerticalSyncEnabled(false);
	}
	else {
		window.setFramerateLimit(0);
		//window.setVerticalSyncEnabled(false);
	}
}
void WindowFrame::Window::WindowEventUpdate(const std::optional<sf::Event>& event) {
	ZoneScopedNC("WindowFrame::Window::WindowEventUpdate", 0x97DBB4);
	if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
		switch (keyPressed->code) {
			case sf::Keyboard::Key::F11:
				WindowToggleFullscreen();
				ChangeScreenMode();
				break;
			case sf::Keyboard::Key::F12:
				WindowToggleSmooth();
				ChangeScreenMode();
				break;
			default: ;
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

void WindowFrame::Init() {
	MFCPP::IO::LoadImageFile(icon, "data/resources/Icon/GameICON.png");
	Window::ChangeScreenMode(1);
	running = true;
	//timestep.setTimeSpeed(0.5);
	timestep.setStep(1.0f / 500.0f);
	//timestep.setMaxAccumulation(1.0f / 30.0f);
}
void WindowFrame::GameSceneInit() {
	ImageManager::AddTexture("MarioHUD", "data/resources/MarioHUD.png");
	ImageManager::AddTexture("TimeHUD", "data/resources/TimeHUD.png");
	MFCPP::AnimationSequenceManager::newData("CoinHUDAnimName");
	for (int i = 0; i < COINHUD_IMAGE_WIDTH / COINHUD_WIDTH; i++) {
		ImageManager::AddTexture(fmt::format("CoinHUD_{}", i), "data/resources/CoinHUD.png", sf::IntRect({ i * COINHUD_WIDTH, 0 }, { COINHUD_WIDTH, COINHUD_HEIGHT }));
		MFCPP::AnimationSequenceManager::addSingleFrame("CoinHUDAnimName", fmt::format("CoinHUD_{}", i));
	}
	CoinHUD.setAnimation(0, 2, 16, true);
	CoinHUD.setAnimationSequence("CoinHUDAnimName");
	MarioHUD.setTexture("MarioHUD");
	TimeHUD.setTexture("TimeHUD");
}
void WindowFrame::FrameDraw() {
	CoinHUD.animationUpdate(sf::Vector2f(236.0f + Scroll::getViewPosition().x, 15.0f + Scroll::getViewPosition().y), sf::Vector2f(0.f, 0.f));
	CoinHUD.animationDraw();
	MarioHUD.animationUpdate(sf::Vector2f(35.0f + Scroll::getViewPosition().x, 15.0f + Scroll::getViewPosition().y), sf::Vector2f(0.f, 0.f));
	MarioHUD.animationDraw();
	TimeHUD.animationUpdate(sf::Vector2f(513.f + Scroll::getViewPosition().x, 15.f + Scroll::getViewPosition().y), sf::Vector2f(0.f, 0.f));
	TimeHUD.animationDraw();
}
void WindowFrame::updateFrame() {
	const sf::Vector2i mouse = sf::Mouse::getPosition(window);
	MousePosition.x = (static_cast<float>(mouse.x) - Scroll::getViewPositionOff().x / 2.0f) * (GameSize.x / (static_cast<float>(window.getSize().x) - Scroll::getViewPositionOff().x));
	MousePosition.y = (static_cast<float>(mouse.y) - Scroll::getViewPositionOff().y / 2.0f) * (GameSize.y / (static_cast<float>(window.getSize().y) - Scroll::getViewPositionOff().y));
}