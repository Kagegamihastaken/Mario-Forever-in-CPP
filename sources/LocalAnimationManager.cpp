#include <SFML/Graphics.hpp>
#include <cstdint>

#include "Core/WindowFrame.hpp"

#include "Core/Animate/LocalAnimationManager.hpp"

void LocalAnimationManager::setAnimation(const int startingIndexAnimation, const int endingIndexAnimation, const int sizex, const int sizey, const int y, const int frequency) {
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
void LocalAnimationManager::setTexture(sf::Sprite& sprite, const sf::Texture* texture) {
	sprite.setTexture(*texture);
}
void LocalAnimationManager::SetRangeIndexAnimation(const int startingIndexAnimation, const int endingIndexAnimation, const int frequency) {
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
void LocalAnimationManager::setIndexAnimation(const int indexAnimation) {
	this->indexAnimation = indexAnimation;
}
void LocalAnimationManager::setStartingIndexAnimation(const int startingIndexAnimation) {
	this->startingIndexAnimation = startingIndexAnimation;
}
void LocalAnimationManager::setEndingIndexAnimation(const int endingIndexAnimation) {
	this->endingIndexAnimation = endingIndexAnimation;
}
void LocalAnimationManager::setFrequency(const int frequency) {
	this->frequency = frequency;
}
void LocalAnimationManager::setYPos(const int y) {
	this->y = y;
}
void LocalAnimationManager::update(sf::Sprite& sprite) {
	//sprite.setTexture(*texture[this->indexAnimation], true);
	sprite.setTextureRect(sf::IntRect({ this->indexAnimation * this->sizex, y * this->sizey }, { this->sizex, this->sizey }));
	this->TimeRan = this->TimeRemainSave + this->TimeRun.getElapsedTime().asMicroseconds() / 1000.0f;
	if (this->frequency != 0 && this->TimeRan >= (2000.0f / this->frequency / (timestep.getTimeSpeed()))) {
		const long long loop = static_cast<long long>(
			this->TimeRan / (2000.0f / this->frequency / (timestep.getTimeSpeed())));
		for (int i = 0; i < loop; i++) {
			if (this->indexAnimation < this->endingIndexAnimation) this->indexAnimation++;
			else this->indexAnimation = this->startingIndexAnimation;
		}
		this->TimeRemainSave = TimeRan - loop * (2000.0f / this->frequency / (timestep.getTimeSpeed()));
		this->TimeRun.restart();
	}
}
sf::IntRect LocalAnimationManager::getAnimationTextureRect() const {
	return sf::IntRect({ this->indexAnimation * this->sizex, y * this->sizey }, { this->sizex, this->sizey });
}
void LocalAnimationManager::silentupdate() {
	this->TimeRan = this->TimeRemainSave + this->TimeRun.getElapsedTime().asMilliseconds();
	if (this->frequency != 0 && this->TimeRan >= (2000.0f / this->frequency / (timestep.getTimeSpeed()))) {
		const long long loop = static_cast<long long>(this->TimeRan / (2000.0f / this->frequency / (timestep.getTimeSpeed())));
		for (int i = 0; i < loop; i++) {
			if (this->indexAnimation < this->endingIndexAnimation) this->indexAnimation++;
			else this->indexAnimation = this->startingIndexAnimation;
		}
		this->TimeRemainSave = TimeRan - loop * (2000.0f / this->frequency / (timestep.getTimeSpeed()));
		this->TimeRun.restart();
	}
}
bool LocalAnimationManager::isAtTheEnd() const {
	int temp = this->endingIndexAnimation;
	temp = std::max(0, temp - 1);
	return (this->indexAnimation == temp);
}
