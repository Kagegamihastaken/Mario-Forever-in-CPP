#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Object/Mario.hpp"
#include "Core/Level.hpp"
#include "Editor/Editor.hpp"
#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Collision/Collide.hpp"

bool MarioLockedView = false;
sf::View view;
float ViewX, ViewY;
float ViewXOff, ViewYOff;
float lastX = std::round(std::min(std::max(Width / 2.0f, player.property.getPosition().x), LevelWidth - Width / 2.f));
sf::Vector2f ScrollPos(0.f, 0.f);
sf::View getLetterboxView(sf::View view, const int windowWidth, const int windowHeight) {
	const float windowRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	const float viewRatio = view.getSize().x / view.getSize().y;
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
	ViewX = view.getCenter().x - Width / 2.0f;
	ViewY = view.getCenter().y - Height / 2.0f;
	window.setView(view);
}
static bool isPointOutOfScreen(const sf::Vector2f& pos, const float offset) {
	return (pos.x < ViewX - offset || pos.x > ViewX + Width + offset || pos.y < ViewY - offset || pos.y > ViewY + Height + offset);
}
bool isOutOfScreen(const MFCPP::CollisionObject& obj, const float offset) {
	return !isCollide(
		getGlobalHitbox(obj.GetLeftHitbox(), obj.GetPosition(), obj.GetOrigin()),
		HitboxExtend(getGlobalHitbox(sf::FloatRect({0.f, 0.f}, {Width, Height}), sf::Vector2f(ViewX, ViewY), sf::Vector2f(0.f, 0.f)), offset)
		);
	// return (obj.GetPosition().x < ViewX + Width + offset &&
	// 	obj.GetPosition().x + obj.GetLeftHitbox().size.x > ViewX - offset &&
	// 	obj.GetPosition().y < ViewY + Height + offset &&
	// 	obj.GetPosition().y + obj.GetLeftHitbox().size.y > ViewY - offset);
}
bool isOutOfScreenYBottom(const MFCPP::CollisionObject& obj, const float offset) {
	return obj.GetPosition().y > ViewY + Height + offset;
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