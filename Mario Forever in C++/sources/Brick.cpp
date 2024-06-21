#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../headers/Brick.hpp"
#include "../headers/Obstacles.hpp"
#include "../headers/WindowFrame.hpp"

#include <vector>
#include <iostream>

std::vector<Obstacles> Bricks;

sf::SoundBuffer BrickSoundBuffer;
sf::Sound BrickSound;
sf::Texture BrickTexture;
int LoadBricks() {
	if (!BrickTexture.loadFromFile("data/resources/brick.png")) {
		std::cout << "Failed to load brick.png" << std::endl;
	}
	if (!BrickSoundBuffer.loadFromFile("data/sounds/coin.wav")) {
		std::cout << "Cannot load data/sounds/coin.wav" << "\n";
	}
	BrickSound.setBuffer(BrickSoundBuffer);
	return 6;
}
int BrickInit = LoadBricks();
void AddBrick(float x, float y) {
	Bricks.push_back(Obstacles{ 0, sf::Sprite(BrickTexture, sf::IntRect(0, 0, 32, 32)) });
	Bricks[Bricks.size() - 1].property.setPosition({ x, y });
	Bricks[Bricks.size() - 1].setHitbox({ 0.f, 0.f, 32.f, 32.f });
}
void BrickUpdate() {
	for (auto& i : Bricks) {
		window.draw(i.property);
	}
}
void HitEvent(float x, float y) {
	for (const auto& i : Bricks) {
		if (i.property.getPosition().x == x && i.property.getPosition().y == y) {
			BrickSound.play();
			break;
		}
	}
}