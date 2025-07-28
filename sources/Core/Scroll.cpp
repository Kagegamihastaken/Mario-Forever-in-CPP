#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Object/Mario.hpp"
#include "Core/Level.hpp"
#include "Editor/Editor.hpp"
#include "Core/Scene.hpp"
#include "Core/Loading/enum.hpp"

bool MarioLockedView = false;
sf::View view;
sf::View WindowView;
float ViewX, ViewY;
float ViewXOff, ViewYOff;
static float OFFSET = 0.0f;
float lastX = std::round(std::min(std::max(Width / 2.0f, player.property.getPosition().x), LevelWidth - 320.0f));
sf::Vector2f ScrollPos(0.f, 0.f);
sf::View getLetterboxView(sf::View view, const int windowWidth, const int windowHeight) {
	const float windowRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	const float viewRatio = view.getSize().x / (float)view.getSize().y;
	float sizeX = 1;
	float sizeY = 1;
	float posX = 0;
	float posY = 0;

	bool horizontalSpacing = true;
	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	// If horizontalSpacing is true, the black bars will appear on the left and right side.
	// Otherwise, the black bars will appear on the top and bottom.

	if (horizontalSpacing) {
		sizeX = viewRatio / windowRatio;
		posX = (1 - sizeX) / 2.f;
	}

	else {
		sizeY = windowRatio / viewRatio;
	}
		posY = (1 - sizeY) / 2.f;
	view.setViewport(sf::FloatRect({ posX, posY }, { sizeX, sizeY }));

	return view;
}
void ViewInit() {
	view = sf::View(sf::FloatRect({ 0, 0 }, { Width, Height}));
}
void WindowSetView() {
	view = getLetterboxView(view, window.getSize().x, window.getSize().y);
}
void moveView(const float x, const float y) {
	view.setCenter({ view.getCenter().x + x, view.getCenter().y + y });
}
void setRotate(int degree) {
	//view.setRotation(sf::Angle(degree));
}
void updateView() {
	const float vx = window.getSize().x / Width;
	const float vy = window.getSize().y / Height;
	const float min = std::min(vx, vy);
	ViewXOff = window.getSize().x - (Width * min);
	ViewYOff = window.getSize().y - (Height * min);
	//view.setViewport(sf::FloatRect({ ((window.getSize().x - (view.getSize().x * min)) / window.getSize().x) * 0.5f, ((window.getSize().y - (view.getSize().y * min)) / window.getSize().y) * 0.5f }, { (view.getSize().x * min) / window.getSize().x, (view.getSize().y * min) / window.getSize().y }));
	ViewX = view.getCenter().x - Width / 2.0f;
	ViewY = view.getCenter().y - Height / 2.0f;
	//rObject.setView(view);
	window.setView(view);
	//window.setView(WindowView);
	//window.setView(viewwin);
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
bool isOutScreenYBottom(float yPos, float OffsetY) {
	if (yPos - ViewY > Height + OffsetY - 32.0f) {
		return true;
	}
	return false;
}