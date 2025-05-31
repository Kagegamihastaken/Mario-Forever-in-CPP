#pragma once
#ifndef LOCALANIMATIONMANAGER_HPP
#define LOCALANIMATIONMANAGER_HPP
#include "Core/Loading/enum.hpp"

class LocalAnimationManager {
private:
	std::string m_lastAnim = "";
protected:
	int m_indexAnimation = 0;
	int m_startingIndexAnimation = 0;
	int m_endingIndexAnimation = 0;
	int m_frequency = 50;
	sf::Clock m_TimeRun;
	float m_TimeRan = 0.0f;
	float m_TimeRemainSave = 0.0f;
	AnimationDirection m_direction;
	std::vector<std::string> m_LeftIndex;
	std::vector<std::string> m_RightIndex;
public:
	void setAnimation(int startingIndexAnimation = 0, int endingIndexAnimation = 0, int frequency = 50);
	void SetRangeIndexAnimation(int startingIndexAnimation, int endingIndexAnimation, int frequency = 50);
	void setIndexAnimation(int indexAnimation);
	void setStartingIndexAnimation(int startingIndexAnimation);
	void setEndingIndexAnimation(int endingIndexAnimation);
	void setFrequency(int frequency);
	void update(sf::Sprite& sprite);
	void silentupdate();
	std::string getCurrentAnimationName();
	void setDirection(const AnimationDirection& dir);
	//sf::IntRect getAnimationTextureRect() const;
	[[nodiscard]] bool isAtTheEnd() const;
	void AddSequence(const std::string& a_left, const std::string& a_right);
	void SetSequence(const std::vector<std::string>& s_left, const std::vector<std::string>& s_right);
};
#endif // LOCALANIMATIONMANAGER_HPP