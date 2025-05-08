#include <SFML/Graphics.hpp>
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Object/Mario.hpp"
#include "../headers/Core/Level.hpp"

#include <algorithm>

bool MarioLockedView = false;
sf::View view;
sf::View viewwin;
float ViewX, ViewY;
float ViewXOff, ViewYOff;
sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight) {
	// Compares the aspect ratio of the window to the aspect ratio of the view,
	// and sets the view's viewport accordingly in order to achieve a letterbox effect.
	// A new view (with a new viewport set) is returned.

	float windowRatio = (float)windowWidth / (float)windowHeight;
	float viewRatio = view.getSize().x / (float)view.getSize().y;
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
		posY = (1 - sizeY) / 2.f;
	}

	view.setViewport(sf::FloatRect({ posX, posY }, { sizeX, sizeY }));

	return view;
}
void ViewInit() {
	view = sf::View(sf::FloatRect({ 0, 0 }, { Width, Height }));
}
void setView() {
	//  MouseX - (Width / 2.0f)

	//view.setCenter(player.property.getPosition().x + (Xvelo > 0.0f ? CompareX * deltaTime : 0.0f), Height / 2.0f);
	//view.setCenter({ player.property.getPosition().x, player.property.getPosition().y });

	viewwin = getLetterboxView(view, window.getSize().x, window.getSize().y);
	view.setCenter({ std::min(std::max(Width / 2.0f, player.property.getPosition().x), LevelWidth - 320.0f), std::min(std::max(Height / 2.0f, player.property.getPosition().y), LevelHeight - 240.0f) });
	viewwin.setCenter(sf::Vector2f({ Width / 2, Height / 2 }));

	//view.setCenter(player.property.getPosition());

	//view.setCenter({ std::min(std::max(Width / 2.0f, player.property.getPosition().x + (MouseX - Width / 2.0f) * 0.1f), LevelWidth / 2.0f), std::min(std::max(Height / 2.0f, player.property.getPosition().y), LevelHeight / 2.0f) });
}
void moveView(float x, float y) {
	view.setCenter({ view.getCenter().x + x, view.getCenter().y + y });
}
void setRotate(int degree) {
	//view.setRotation(sf::Angle(degree));
}
void updateView() {
	float vx = window.getSize().x / Width;
	float vy = window.getSize().y / Height;
	float min = std::min(vx, vy);
	ViewXOff = window.getSize().x - (Width * min);
	ViewYOff = window.getSize().y - (Height * min);
	//view.setViewport(sf::FloatRect({ ((window.getSize().x - (view.getSize().x * min)) / window.getSize().x) * 0.5f, ((window.getSize().y - (view.getSize().y * min)) / window.getSize().y) * 0.5f }, { (view.getSize().x * min) / window.getSize().x, (view.getSize().y * min) / window.getSize().y }));
	ViewX = view.getCenter().x - Width / 2.0f;
	ViewY = view.getCenter().y - Height / 2.0f;
	rTexture.setView(view);
	window.setView(viewwin);
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