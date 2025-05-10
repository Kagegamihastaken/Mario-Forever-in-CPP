#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdint>

#include "../headers/Core/WindowFrame.hpp"

#include "../headers/Core/Animate/LocalAnimationManager.hpp"

void LocalAnimationManager::setAnimation(int startingIndexAnimation, int endingIndexAnimation, int sizex, int sizey, int y, int frequency) {
	this->startingIndexAnimation = startingIndexAnimation;
	this->indexAnimation = startingIndexAnimation;
	this->endingIndexAnimation = endingIndexAnimation;
	this->frequency = frequency;
	this->TimeRun.restart();
	this->TimeRan = 0.0f;
	this->TimeRemainSave = 0.0f;
	this->sizex = sizex;
	this->sizey = sizey;
	this->y = y;
}
void LocalAnimationManager::setTexture(sf::Sprite& sprite, sf::Texture* texture) {
	sprite.setTexture(*texture);
}
void LocalAnimationManager::SetRangeIndexAnimation(int startingIndexAnimation, int endingIndexAnimation, int frequency) {
	if (this->startingIndexAnimation != startingIndexAnimation || this->endingIndexAnimation != endingIndexAnimation) {
		this->startingIndexAnimation = startingIndexAnimation;
		this->endingIndexAnimation = endingIndexAnimation;
		this->frequency = frequency;
		this->indexAnimation = startingIndexAnimation;
		this->TimeRun.restart();
		this->TimeRan = 0.0f;
		this->TimeRemainSave = 0.0f;
	}
	else {
		this->frequency = frequency;
		this->TimeRun.restart();
		this->TimeRan = 0.0f;
		this->TimeRemainSave = 0.0f;
	}
}
void LocalAnimationManager::setIndexAnimation(int indexAnimation) {
	this->indexAnimation = indexAnimation;
}
void LocalAnimationManager::setStartingIndexAnimation(int startingIndexAnimation) {
	this->startingIndexAnimation = startingIndexAnimation;
}
void LocalAnimationManager::setEndingIndexAnimation(int endingIndexAnimation) {
	this->endingIndexAnimation = endingIndexAnimation;
}
void LocalAnimationManager::setFrequency(int frequency) {
	this->frequency = frequency;
}
void LocalAnimationManager::setYPos(int y) {
	this->y = y;
}
void LocalAnimationManager::update(sf::Sprite& sprite) {
	//sprite.setTexture(*texture[this->indexAnimation], true);
	sprite.setTextureRect(sf::IntRect({ this->indexAnimation * this->sizex, y * this->sizey }, { this->sizex, this->sizey }));
	this->TimeRan = this->TimeRemainSave + this->TimeRun.getElapsedTime().asMicroseconds() / 1000.0f;
	if (this->frequency != 0 && this->TimeRan >= (2000.0f / this->frequency / (timestep.getTimeSpeed()))) {
		long long loop = (long long)(this->TimeRan / (2000.0f / this->frequency / (timestep.getTimeSpeed())));
		for (int i = 0; i < loop; i++) {
			if (this->indexAnimation < this->endingIndexAnimation) this->indexAnimation++;
			else this->indexAnimation = this->startingIndexAnimation;
		}
		this->TimeRemainSave = TimeRan - loop * (2000.0f / this->frequency / (timestep.getTimeSpeed()));
		this->TimeRun.restart();
	}
}
void LocalAnimationManager::silentupdate() {
	this->TimeRan = this->TimeRemainSave + this->TimeRun.getElapsedTime().asMilliseconds();
	if (this->frequency != 0 && this->TimeRan >= (2000.0f / this->frequency / (timestep.getTimeSpeed()))) {
		long long loop = (long long)(this->TimeRan / (2000.0f / this->frequency / (timestep.getTimeSpeed())));
		for (int i = 0; i < loop; i++) {
			if (this->indexAnimation < this->endingIndexAnimation) this->indexAnimation++;
			else this->indexAnimation = this->startingIndexAnimation;
		}
		this->TimeRemainSave = TimeRan - loop * (2000.0f / this->frequency / (timestep.getTimeSpeed()));
		this->TimeRun.restart();
	}
}
bool LocalAnimationManager::isAtTheEnd() {
	int temp = this->endingIndexAnimation;
	temp = std::max(0, temp - 1);
	return (this->indexAnimation == temp);
}