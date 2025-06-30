#pragma once
#ifndef LOCALANIMATIONMANAGER_HPP
#define LOCALANIMATIONMANAGER_HPP
#include "Core/SimpleSprite.hpp"
#include "Core/Loading/enum.hpp"

class SingleAnimationObject {
	std::string m_lastAnim = "";
	std::vector<MFCPP::SimpleSprite> m_LeftIndex;
	std::vector<MFCPP::SimpleSprite> m_RightIndex;

	int m_indexAnimation = 0;
	int m_startingIndexAnimation = 0;
	int m_endingIndexAnimation = 0;
	int m_frequency = 50;
	sf::Clock m_TimeRun;
	float m_TimeRan = 0.0f;
	float m_TimeRemainSave = 0.0f;
	sf::Angle m_angle = sf::degrees(0.f);
	AnimationDirection m_direction = AnimationDirection::ANIM_LEFT;
public:
	void setAnimation(int startingIndexAnimation, int endingIndexAnimation, int frequency = 50);
	void SetRangeIndexAnimation(int startingIndexAnimation, int endingIndexAnimation, int frequency = 50);
	void setIndexAnimation(int indexAnimation);
	void setStartingIndexAnimation(int startingIndexAnimation);
	void setEndingIndexAnimation(int endingIndexAnimation);
	void setFrequencyAnimation(int frequency);

	void AnimationUpdate(const sf::Vector2f& pos, const sf::Vector2f& origin);
	void AnimationDraw(sf::RenderWindow& window, sf::RenderStates states = sf::RenderStates::Default) const;
	void setAnimationDirection(const AnimationDirection& dir);
	//sf::IntRect getAnimationTextureRect() const;
	[[nodiscard]] bool isAnimationAtTheEnd() const;
	[[nodiscard]] AnimationDirection getAnimationDirection() const;
	void AddAnimationSequence(const std::string& a_left, const std::string& a_right);
	void setAnimationSequence(const std::vector<std::string>& s_left, const std::vector<std::string>& s_right);
	void setAnimationSequence(const std::vector<std::string>& s_left);
	void setRotation(sf::Angle angle);
};
#endif // LOCALANIMATIONMANAGER_HPP