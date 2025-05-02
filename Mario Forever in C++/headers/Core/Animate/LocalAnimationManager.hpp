#pragma once
#ifndef LOCALANIMATIONMANAGER_HPP
#define LOCALANIMATIONMANAGER_HPP

#include <SFML\Graphics.hpp>

class LocalAnimationManager {
private:
	int indexAnimation = 0;
	int startingIndexAnimation = 0;
	int endingIndexAnimation = 0;
	int frequency = 50;
	int y = 0;
	sf::Clock TimeRun;
	float TimeRan = 0.0f;
	float TimeRemainSave = 0.0f;
	int sizex = 0, sizey = 0;
public:
	void setAnimation(int startingIndexAnimation = 0, int endingIndexAnimation = 0, int sizex = 0, int sizey = 0, int y = 0, int frequency = 50);
	void SetRangeIndexAnimation(int startingIndexAnimation, int endingIndexAnimation, int frequency = 50);
	void setIndexAnimation(int indexAnimation);
	void setStartingIndexAnimation(int startingIndexAnimation);
	void setEndingIndexAnimation(int endingIndexAnimation);
	void setFrequency(int frequency);
	void update(sf::Sprite& sprite);
	void silentupdate();
	bool isAtTheEnd();
	void setTexture(sf::Sprite& sprite, sf::Texture* texture);
	void setYPos(int y);
};
#endif // LOCALANIMATIONMANAGER_HPP