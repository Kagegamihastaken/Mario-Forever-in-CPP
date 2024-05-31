#include <SFML/Graphics.hpp>
#include "../headers/WindowFrame.hpp"
#include "../headers/Mario.hpp"
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
void updateFrame() {
	MouseX = mouse.getPosition(window).x * (Width / window.getSize().x);
	MouseY = mouse.getPosition(window).y * (Height / window.getSize().y);
	float fpsUpdate = 1.0f / fpsClock.restart().asSeconds();
	if (GameClock.getElapsedTime().asSeconds() >= 0.5f) {
		fps = fpsUpdate;
		GameClock.restart().asSeconds();
	}
	if (!optionSmoothness) window.setFramerateLimit(50);
	else window.setFramerateLimit(1000);
	deltaTime = delta.restart().asSeconds() * 50.0f;
	if (deltaTime >= 1.0f) {
		deltaTime = 1.0f;
		optionSmoothness = false;
	}
	else optionSmoothness = true;
}