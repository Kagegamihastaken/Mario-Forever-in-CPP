#pragma once
#ifndef LOCALANIMATIONMANAGER_HPP
#define LOCALANIMATIONMANAGER_HPP
#include <SFML/Graphics.hpp>
#include "Core/Loading/enum.hpp"

class LocalAnimationManager {
private:
	std::string m_lastAnim = "";
	std::vector<sf::Sprite> m_LeftIndex;
	std::vector<sf::Sprite> m_RightIndex;
protected:
	int m_indexAnimation = 0;
	int m_startingIndexAnimation = 0;
	int m_endingIndexAnimation = 0;
	int m_frequency = 50;
	sf::Clock m_TimeRun;
	float m_TimeRan = 0.0f;
	float m_TimeRemainSave = 0.0f;
	AnimationDirection m_direction;
public:
	void setAnimation(int startingIndexAnimation, int endingIndexAnimation, int frequency = 50);
	void SetRangeIndexAnimation(int startingIndexAnimation, int endingIndexAnimation, int frequency = 50);
	void setIndexAnimation(int indexAnimation);
	void setStartingIndexAnimation(int startingIndexAnimation);
	void setEndingIndexAnimation(int endingIndexAnimation);
	void setFrequency(int frequency);
	void AnimationUpdate(const sf::Vector2f& pos, const sf::Vector2f& origin);
	void AnimationDraw(sf::RenderWindow& window);
	void setDirection(const AnimationDirection& dir);
	//sf::IntRect getAnimationTextureRect() const;
	[[nodiscard]] bool isAtTheEnd() const;
	void AddSequence(const std::string& a_left, const std::string& a_right);
	void SetSequence(const std::vector<std::string>& s_left, const std::vector<std::string>& s_right);
};
#endif // LOCALANIMATIONMANAGER_HPP