#include <SFML/Graphics.hpp>
#include "../headers/Scroll.hpp"
#include "../headers/WindowFrame.hpp"
#include "../headers/Mario.hpp"
#include "../headers/Level.hpp"

#include <algorithm>

bool MarioLockedView = false;
sf::View view;
float ViewX, ViewY;
void ViewInit() {
	view.reset(sf::FloatRect(0, 0, Width, Height));
}
void setView() {
	//view.setCenter(player.property.getPosition().x + (Xvelo > 0.0f ? CompareX * deltaTime : 0.0f), Height / 2.0f);
	view.setCenter(std::min(std::max(Width / 2.0f, player.property.getPosition().x), LevelWidth / 2.0f), std::min(std::max(Height / 2.0f, player.property.getPosition().y), LevelHeight / 2.0f));
}
void moveView(float x, float y) {
	view.setCenter(view.getCenter().x + x, view.getCenter().y + y);
}
void setRotate(int degree) {
	view.setRotation(degree);
}
void updateView() {
	float vx = window.getSize().x / Width;
	float vy = window.getSize().y / Height;
	float min = std::min(vx, vy);
	view.setViewport(sf::FloatRect(((window.getSize().x - (Width * min)) / window.getSize().x) * 0.5f, ((window.getSize().y - (Height * min)) / window.getSize().y) * 0.5f, (Width * min) / window.getSize().x, (Height * min) / window.getSize().y));
	ViewX = view.getCenter().x - Width / 2.0f;
	ViewY = view.getCenter().y - Height / 2.0f;
	window.setView(view);
}
bool isOutScreen(float xPos, float yPos, float OffsetX, float OffsetY) {
	if (xPos - ViewX < 0.0f - OffsetX || xPos - ViewX > Width + OffsetX - 32.0f || yPos - ViewY < 0.0f - OffsetY || yPos - ViewY > Height + OffsetY - 32.0f) {
		return true;
	}
	return false;
}
bool isOutScreenX(float xPos, float OffsetX) {
	if (xPos - ViewX < 0.0f - OffsetX || xPos - ViewX > Width + OffsetX - 32.0f) {
		return true;
	}
	return false;
}
bool isOutScreenY(float yPos, float OffsetY) {
	if (yPos - ViewY < 0.0f - OffsetY || yPos - ViewY > Height + OffsetY - 32.0f) {
		return true;
	}
	return false;
}