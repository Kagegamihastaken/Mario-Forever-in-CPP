#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Editor/Editor.hpp"
#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Collision/Collide.hpp"

sf::View View;
sf::Vector2f ViewPosition;
sf::Vector2f ViewPositionOff;

sf::View &Scroll::getView() {
	return View;
}

const sf::Vector2f &Scroll::getViewPosition() {
	return ViewPosition;
}

const sf::Vector2f &Scroll::getViewPositionOff() {
	return ViewPositionOff;
}

void setLetterboxView(sf::View& viewVal, const sf::Vector2u& windowSize) {
	const float windowRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
	const float viewRatio = viewVal.getSize().x / viewVal.getSize().y;
	sf::Vector2f size(1.f, 1.f);
	sf::Vector2f pos(0.f, 0.f);

	if (windowRatio >= viewRatio) {
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
	const auto winSize = WindowFrame::getWindow().getSize();
	const auto gameSize = WindowFrame::getGameSize();
	const float min = std::min(winSize.x / gameSize.x, winSize.y / gameSize.y);
	ViewPositionOff = {winSize.x - (gameSize.x * min), winSize.y - (gameSize.y * min)};
	ViewPosition = {View.getCenter().x - gameSize.x / 2.0f, View.getCenter().y - gameSize.y / 2.0f};
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