#pragma once
#ifndef LOCALANIMATIONMANAGER_HPP
#define LOCALANIMATIONMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class LocalAnimationManager {
private:
	int indexAnimation;
	int startingIndexAnimation;
	int endingIndexAnimation;
	int frequency;
	sf::Clock TimeRun;
public:
	void setAnimation(int startingIndexAnimation = 0, int endingIndexAnimation = 0, int frequency = 50) {
		this->startingIndexAnimation = startingIndexAnimation;
		this->indexAnimation = startingIndexAnimation;
		this->endingIndexAnimation = endingIndexAnimation;
		this->frequency = frequency;
	}
	void setIndexAnimation(int indexAnimation) {
		this->indexAnimation = indexAnimation;
	}
	void setStartingIndexAnimation(int startingIndexAnimation) {
		this->startingIndexAnimation = startingIndexAnimation;
	}
	void setEndingIndexAnimation(int endingIndexAnimation) {
		this->endingIndexAnimation = endingIndexAnimation;
	}
	void setFrequency(int frequency) {
		this->frequency = frequency;
	}
	void update(sf::Sprite& sprite, std::vector<sf::Texture*> texture) {
		sprite.setTexture(*texture[this->indexAnimation]);
		if (this->frequency != 0 && this->TimeRun.getElapsedTime().asMilliseconds() >= 3000.0f / this->frequency) {
			this->TimeRun.restart().asMilliseconds();
			if (this->indexAnimation < this->endingIndexAnimation) this->indexAnimation++;
			else this->indexAnimation = this->startingIndexAnimation;
		}
	}
	void silentupdate(sf::Sprite& sprite) {
		if (this->frequency != 0 && this->TimeRun.getElapsedTime().asMilliseconds() >= 3000.0f / this->frequency) {
			this->TimeRun.restart().asMilliseconds();
			this->indexAnimation++;
			if (this->indexAnimation > this->endingIndexAnimation)  this->indexAnimation = this->startingIndexAnimation;
		}
	}
	bool isAtTheEnd() {
		int temp = this->endingIndexAnimation;
		temp = std::max(0, temp - 1);
		return (this->indexAnimation == temp);
	}
};
#endif // LOCALANIMATIONMANAGER_HPP