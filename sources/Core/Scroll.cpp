#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Editor/Editor.hpp"
#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Collision/Collide.hpp"

sf::View Scroll::View;
sf::Vector2f Scroll::ViewPosition;
sf::Vector2f Scroll::ViewPositionOff;

sf::View &Scroll::getView() {
	return View;
}

const sf::Vector2f &Scroll::getViewPosition() {
	return ViewPosition;
}

const sf::Vector2f &Scroll::getViewPositionOff() {
	return ViewPositionOff;
}

void Scroll::setLetterboxView(sf::View& viewVal, const sf::Vector2u& windowSize) {
	const float windowRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
	const float viewRatio = viewVal.getSize().x / viewVal.getSize().y;
	sf::Vector2f size(1.f, 1.f);
	sf::Vector2f pos(0.f, 0.f);

	bool horizontalSpacing = true;
	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	// If horizontalSpacing is true, the black bars will appear on the left and right side.
	// Otherwise, the black bars will appear on the top and bottom.

	if (horizontalSpacing) {
		size.x = viewRatio / windowRatio;
		pos.x = (1 - size.x) / 2.f;
	}

	else {
		size.y = windowRatio / viewRatio;
	}
		pos.y = (1 - size.y) / 2.f;
	viewVal.setViewport(sf::FloatRect(pos, size));
}
void Scroll::Init() {
	View = sf::View(sf::FloatRect({ 0, 0 }, { WindowFrame::getGameSize().x, WindowFrame::getGameSize().y}));
}
void Scroll::setWindowView() {
	setLetterboxView(View, WindowFrame::getWindow().getSize());
}
void Scroll::moveView(const sf::Vector2f& pos) {
	View.setCenter(View.getCenter() + pos);
}
void Scroll::updateView() {
	const float vx = WindowFrame::getWindow().getSize().x / WindowFrame::getGameSize().x;
	const float vy = WindowFrame::getWindow().getSize().y / WindowFrame::getGameSize().y;
	const float min = std::min(vx, vy);
	ViewPositionOff.x = WindowFrame::getWindow().getSize().x - (WindowFrame::getGameSize().x * min);
	ViewPositionOff.y = WindowFrame::getWindow().getSize().y - (WindowFrame::getGameSize().y * min);
	ViewPosition.x = View.getCenter().x - WindowFrame::getGameSize().x / 2.0f;
	ViewPosition.y = View.getCenter().y - WindowFrame::getGameSize().y / 2.0f;
	WindowFrame::getWindow().setView(View);
}
bool Scroll::isOutOfScreen(const MFCPP::CollisionObject& obj, const float offset) {
	return !isCollide(
		getGlobalHitbox(obj.GetLeftHitbox(), obj.GetPosition(), obj.GetOrigin()),
		HitboxExtend(getGlobalHitbox(sf::FloatRect({0.f, 0.f}, {WindowFrame::getGameSize().x, WindowFrame::getGameSize().y}), ViewPosition, sf::Vector2f(0.f, 0.f)), offset)
		);
	// return (obj.GetPosition().x < ViewX + Width + offset &&
	// 	obj.GetPosition().x + obj.GetLeftHitbox().size.x > ViewX - offset &&
	// 	obj.GetPosition().y < ViewY + Height + offset &&
	// 	obj.GetPosition().y + obj.GetLeftHitbox().size.y > ViewY - offset);
}
bool Scroll::isOutOfScreenYBottom(const MFCPP::CollisionObject& obj, const float offset) {
	return obj.GetPosition().y - obj.GetOrigin().y > ViewPosition.y + WindowFrame::getGameSize().y + offset;
}
bool Scroll::isOutOfScreenXLeft(const MFCPP::CollisionObject& obj, const float offset) {
	return obj.GetPosition().x - obj.GetOrigin().x + obj.GetLeftHitbox().size.x < ViewPosition.x - offset;
}
bool Scroll::isOutOfScreenXRight(const MFCPP::CollisionObject& obj, const float offset) {
	return obj.GetPosition().x - obj.GetOrigin().x > ViewPosition.x + WindowFrame::getGameSize().x + offset;
}