#pragma once
#ifndef LOCALANIMATIONMANAGER_HPP
#define LOCALANIMATIONMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class LocalAnimationManager {
private:
	sf::Vector2i spriteSize;
	sf::Vector2i indexAnimation;
	sf::Vector2i startingIndexAnimation;
	sf::Vector2i endingIndexAnimation;
	int frequency;
	sf::Clock TimeRun;
public:
	void setAnimation(sf::Vector2i spriteSize, sf::Vector2i startingIndexAnimation = sf::Vector2i(0, 0), sf::Vector2i endingIndexAnimation = sf::Vector2i(0, 0), int frequency = 50) {
		this->spriteSize = spriteSize;
		this->startingIndexAnimation = startingIndexAnimation;
		this->indexAnimation = startingIndexAnimation;
		if (endingIndexAnimation != sf::Vector2i(0, 0)) this->endingIndexAnimation = endingIndexAnimation;
		else this->endingIndexAnimation = spriteSize;
		this->frequency = frequency;
	}
	void setIndexAnimation(sf::Vector2i indexAnimation) {
		this->indexAnimation = indexAnimation;
	}
	void setStartingIndexAnimation(sf::Vector2i startingIndexAnimation) {
		this->startingIndexAnimation = startingIndexAnimation;
	}
	void setEndingIndexAnimation(sf::Vector2i endingIndexAnimation) {
		this->endingIndexAnimation = endingIndexAnimation;
	}
	void setFrequency(int frequency) {
		this->frequency = frequency;
	}
	void update(sf::Sprite& sprite, sf::Texture& texture) {
		sf::IntRect rect(this->indexAnimation.x * this->spriteSize.x + this->indexAnimation.x,
			this->indexAnimation.y * this->spriteSize.y + this->indexAnimation.y,
			this->spriteSize.x, this->spriteSize.y);
		sprite.setTexture(texture);
		sprite.setTextureRect(rect);
		if (this->frequency != 0 && this->TimeRun.getElapsedTime().asMilliseconds() >= 3000.0f / this->frequency) {
			this->TimeRun.restart().asMilliseconds();
			if (this->indexAnimation.y < this->endingIndexAnimation.y) {
				this->indexAnimation.y++;
			}
			else {
				this->indexAnimation.y = this->startingIndexAnimation.y;
				this->indexAnimation.x++;
				if (this->indexAnimation.x >= this->endingIndexAnimation.x)
					this->indexAnimation.x = this->startingIndexAnimation.x;
			}
		}
	}
	void silentupdate(sf::Sprite& sprite) {
		if (this->frequency != 0 && this->TimeRun.getElapsedTime().asMilliseconds() >= 3000.0f / this->frequency) {
			this->TimeRun.restart().asMilliseconds();
			if (this->indexAnimation.y < this->endingIndexAnimation.y) {
				this->indexAnimation.y++;
			}
			else {
				this->indexAnimation.y = this->startingIndexAnimation.y;
				this->indexAnimation.x++;
				if (this->indexAnimation.x >= this->endingIndexAnimation.x)
					this->indexAnimation.x = this->startingIndexAnimation.x;
			}
		}
	}
	bool isAtTheEnd() {
		sf::Vector2i temp = this->endingIndexAnimation;
		temp.x = std::max(0, temp.x - 1);
		temp.y = std::max(0, temp.y - 1);
		return (this->indexAnimation.x == temp.x && this->indexAnimation.y == temp.y);
	}
};
#endif // LOCALANIMATIONMANAGER_HPP