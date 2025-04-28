#pragma once
#ifndef LOCALANIMATIONMANAGER_HPP
#define LOCALANIMATIONMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdint>

#include "../WindowFrame.hpp"

class LocalAnimationManager {
private:
	int indexAnimation = 0;
	int startingIndexAnimation = 0;
	int endingIndexAnimation = 0;
	int frequency = 50;
	sf::Clock TimeRun;
	float TimeRan = 0.0f;
	float TimeRemainSave = 0.0f;
	uintptr_t TextureAddress = 0;
	uintptr_t SpriteAddress = 0;
public:
	void setAnimation(int startingIndexAnimation = 0, int endingIndexAnimation = 0, int frequency = 50) {
		this->startingIndexAnimation = startingIndexAnimation;
		this->indexAnimation = startingIndexAnimation;
		this->endingIndexAnimation = endingIndexAnimation;
		this->frequency = frequency;
		this->TimeRun.restart();
		this->TimeRan = 0.0f;
		this->TimeRemainSave = 0.0f;
		this->TextureAddress = 0;
		this->SpriteAddress = 0;
	}
	void SetRangeIndexAnimation(int startingIndexAnimation, int endingIndexAnimation, int frequency = 50) {
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
		//sprite.setTexture(*texture[this->indexAnimation], true);
		if (this->TextureAddress != reinterpret_cast<uintptr_t>(texture[this->indexAnimation]) || this->SpriteAddress != reinterpret_cast<uintptr_t>(&sprite)) {
			sprite.setTexture(*texture[this->indexAnimation], true);
			this->TextureAddress = reinterpret_cast<uintptr_t>(texture[this->indexAnimation]);
			this->SpriteAddress = reinterpret_cast<uintptr_t>(&sprite);
		}
		this->TimeRan = this->TimeRemainSave + this->TimeRun.getElapsedTime().asMilliseconds();
		if (this->frequency != 0 && this->TimeRan >= (2000.0f / this->frequency)) {
			long long loop = (long long)(this->TimeRan / (2000.0f / this->frequency));
			for (int i = 0; i < loop; i++) {
				if (this->indexAnimation < this->endingIndexAnimation) this->indexAnimation++;
				else this->indexAnimation = this->startingIndexAnimation;
			}
			this->TimeRemainSave = TimeRan - loop * (2000.0f / this->frequency);
			this->TimeRun.restart();
		}
	}
	void silentupdate(sf::Sprite& sprite) {
		this->TimeRan = this->TimeRemainSave + this->TimeRun.getElapsedTime().asMilliseconds();
		if (this->frequency != 0 && this->TimeRan >= (2000.0f / this->frequency)) {
			long long loop = (long long)(this->TimeRan / (2000.0f / this->frequency));
			for (int i = 0; i < loop; i++) {
				if (this->indexAnimation < this->endingIndexAnimation) this->indexAnimation++;
				else this->indexAnimation = this->startingIndexAnimation;
			}
			this->TimeRemainSave = TimeRan - loop * (2000.0f / this->frequency);
			this->TimeRun.restart();
		}
	}
	bool isAtTheEnd() {
		int temp = this->endingIndexAnimation;
		temp = std::max(0, temp - 1);
		return (this->indexAnimation == temp);
	}
};
#endif // LOCALANIMATIONMANAGER_HPP