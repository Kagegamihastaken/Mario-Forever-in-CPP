#include <SFML/Graphics.hpp>
#include <cstdint>
#include <iostream>

#include "Core/WindowFrame.hpp"

#include "Core/Animate/LocalAnimationManager.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Loading/enum.hpp"

void LocalAnimationManager::setAnimation(const int startingIndexAnimation, const int endingIndexAnimation, const int frequency) {
	m_startingIndexAnimation = startingIndexAnimation;
	m_indexAnimation = startingIndexAnimation;
	m_endingIndexAnimation = endingIndexAnimation;
	m_frequency = frequency;
	m_TimeRun.restart();
	m_TimeRan = 0.0f;
	m_TimeRemainSave = 0.0f;
	m_direction = AnimationDirection::ANIM_LEFT;
}
void LocalAnimationManager::AddSequence(const std::string& a_left, const std::string& a_right) {
	m_LeftIndex.emplace_back(ImageManager::GetTexture(a_left));
	m_RightIndex.emplace_back(ImageManager::GetTexture(a_right));
}
void LocalAnimationManager::SetSequence(const std::vector<std::string>& s_left, const std::vector<std::string>& s_right) {
	if (s_left.size() != s_right.size()) {
		std::cout << "Size of 2 directions must be equal\n";
		return;
	}
	m_LeftIndex.clear();
	m_RightIndex.clear();
	for (const auto& str : s_left)
		m_LeftIndex.emplace_back(ImageManager::GetTexture(str));
	for (const auto& str : s_right)
		m_RightIndex.emplace_back(ImageManager::GetTexture(str));
}
void LocalAnimationManager::SetRangeIndexAnimation(const int startingIndexAnimation, const int endingIndexAnimation, const int frequency) {
	if (m_startingIndexAnimation != startingIndexAnimation || m_endingIndexAnimation != endingIndexAnimation) {
		m_startingIndexAnimation = startingIndexAnimation;
		m_endingIndexAnimation = endingIndexAnimation;
		m_frequency = frequency;
		m_indexAnimation = startingIndexAnimation;
		m_TimeRun.restart();
		m_TimeRan = 0.0f;
		m_TimeRemainSave = 0.0f;
	}
	else {
		m_frequency = frequency;
		m_TimeRun.restart();
		m_TimeRan = 0.0f;
		m_TimeRemainSave = 0.0f;
	}
}
void LocalAnimationManager::setIndexAnimation(const int indexAnimation) {
	m_indexAnimation = indexAnimation;
}
void LocalAnimationManager::setStartingIndexAnimation(const int startingIndexAnimation) {
	m_startingIndexAnimation = startingIndexAnimation;
}
void LocalAnimationManager::setEndingIndexAnimation(const int endingIndexAnimation) {
	m_endingIndexAnimation = endingIndexAnimation;
}
void LocalAnimationManager::setFrequency(const int frequency) {
	m_frequency = frequency;
}
void LocalAnimationManager::AnimationUpdate(const sf::Vector2f& pos, const sf::Vector2f& origin) {
	//sprite.setTexture(*texture[this->indexAnimation], true);
	//sprite.setTextureRect(sf::IntRect({ this->indexAnimation * this->sizex, y * this->sizey }, { this->sizex, this->sizey }));
	m_TimeRan = m_TimeRemainSave + m_TimeRun.getElapsedTime().asMicroseconds() / 1000.0f;
	if (m_frequency != 0 && m_TimeRan >= (2000.0f / m_frequency / (timestep.getTimeSpeed()))) {
		const long long loop = static_cast<long long>(
		m_TimeRan / (2000.0f / m_frequency / (timestep.getTimeSpeed())));
		for (int i = 0; i < loop; i++) {
			if (m_indexAnimation < m_endingIndexAnimation) m_indexAnimation++;
			else m_indexAnimation = m_startingIndexAnimation;
		}
		m_TimeRemainSave = m_TimeRan - loop * (2000.0f / m_frequency / (timestep.getTimeSpeed()));
		m_TimeRun.restart();
	}

	if (m_direction == AnimationDirection::ANIM_RIGHT) {
		m_RightIndex[m_indexAnimation].setPosition(pos);
		m_RightIndex[m_indexAnimation].setOrigin(origin);
	}
	else {
		m_LeftIndex[m_indexAnimation].setPosition(pos);
		m_LeftIndex[m_indexAnimation].setOrigin(origin);
	}
}
void LocalAnimationManager::AnimationDraw(sf::RenderWindow& window) {
	if (m_direction == AnimationDirection::ANIM_RIGHT) window.draw(m_RightIndex[m_indexAnimation]);
	else window.draw(m_LeftIndex[m_indexAnimation]);
}
void LocalAnimationManager::setDirection(const AnimationDirection& dir) {
	m_direction = dir;
}
//sf::IntRect LocalAnimationManager::getAnimationTextureRect() const {
	//return sf::IntRect({ this->indexAnimation * this->sizex, y * this->sizey }, { this->sizex, this->sizey });
//}
bool LocalAnimationManager::isAtTheEnd() const {
	int temp = this->m_endingIndexAnimation;
	temp = std::max(0, temp - 1);
	return (this->m_indexAnimation == temp);
}
