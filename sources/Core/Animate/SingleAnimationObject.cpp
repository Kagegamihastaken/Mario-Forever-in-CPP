#include "Core/WindowFrame.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Loading/enum.hpp"

void SingleAnimationObject::setAnimation(const int startingIndexAnimation, const int endingIndexAnimation, const int frequency, const bool changeAnimDirection) {
	m_startingIndexAnimation = startingIndexAnimation;
	m_indexAnimation = startingIndexAnimation;
	m_endingIndexAnimation = endingIndexAnimation;
	m_frequency = frequency;
	m_TimeRun.restart();
	m_TimeRan = 0.0f;
	m_TimeRemainSave = 0.0f;
	m_direction = AnimationDirection::ANIM_RIGHT;
	m_change_direction = changeAnimDirection;
}
void SingleAnimationObject::AddAnimationSequence(const std::string& aName) {
	m_Index.emplace_back(ImageManager::GetReturnTexture(aName));
}
void SingleAnimationObject::setAnimationSequence(const std::vector<std::string>& aName) {
	m_Index.clear();
	for (const auto& str : aName)
		m_Index.emplace_back(ImageManager::GetReturnTexture(str));
}
void SingleAnimationObject::SetRangeIndexAnimation(const int startingIndexAnimation, const int endingIndexAnimation, const int frequency) {
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
void SingleAnimationObject::setIndexAnimation(const int indexAnimation) {
	m_indexAnimation = indexAnimation;
}
void SingleAnimationObject::setStartingIndexAnimation(const int startingIndexAnimation) {
	m_startingIndexAnimation = startingIndexAnimation;
}
void SingleAnimationObject::setEndingIndexAnimation(const int endingIndexAnimation) {
	m_endingIndexAnimation = endingIndexAnimation;
}
void SingleAnimationObject::setFrequencyAnimation(const int frequency) {
	m_frequency = frequency;
}
void SingleAnimationObject::AnimationUpdate(const sf::Vector2f& pos, const sf::Vector2f& origin) {
	//sprite.setTexture(*texture[this->indexAnimation], true);
	//sprite.setTextureRect(sf::IntRect({ this->indexAnimation * this->sizex, y * this->sizey }, { this->sizex, this->sizey }));
	m_TimeRan = m_TimeRemainSave + static_cast<float>(m_TimeRun.getElapsedTime().asMicroseconds()) / 1000.0f;
	if (m_frequency != 0) {
		if (const float FrameTime = 2000.0f / static_cast<float>(m_frequency) / static_cast<float>(timestep.getTimeSpeed());
			m_TimeRan >= FrameTime) {
			const auto FrameCount = static_cast<int>(m_TimeRan / FrameTime);
			//m_indexAnimation += loop;
			m_indexAnimation = m_startingIndexAnimation + (m_indexAnimation - m_startingIndexAnimation + FrameCount) % (m_endingIndexAnimation - m_startingIndexAnimation + 1);
			//if (m_indexAnimation > m_endingIndexAnimation) m_indexAnimation = m_startingIndexAnimation + (m_indexAnimation + loop) % (m_endingIndexAnimation - m_startingIndexAnimation + 1);
			//for (int i = 0; i < loop; i++) {
			//	if (m_indexAnimation < m_endingIndexAnimation) m_indexAnimation++;
			//	else m_indexAnimation = m_startingIndexAnimation;
			//}
			m_TimeRemainSave = m_TimeRan - static_cast<float>(FrameCount) * FrameTime;
			m_TimeRun.restart();
			}
	}
	const sf::IntRect rect = m_Index[m_indexAnimation].getTextureRect();
	m_Index[m_indexAnimation].setRotation(m_angle);
	m_Index[m_indexAnimation].setPosition(pos);
	m_Index[m_indexAnimation].setOrigin(origin);
	if (m_direction == AnimationDirection::ANIM_RIGHT)
		m_Index[m_indexAnimation].setTextureRect(sf::IntRect({0, rect.position.y}, {std::abs(rect.size.x), rect.size.y}));
	else
		if (m_change_direction)
			m_Index[m_indexAnimation].setTextureRect(sf::IntRect({1 * std::abs(rect.size.x), rect.position.y}, {-std::abs(rect.size.x), rect.size.y}));
}
void SingleAnimationObject::AnimationDraw(sf::RenderWindow& window, const sf::RenderStates &states) const {
	window.draw(m_Index[m_indexAnimation], states);
}
void SingleAnimationObject::setAnimationDirection(const AnimationDirection& dir) {
	m_direction = dir;
}
//sf::IntRect LocalAnimationManager::getAnimationTextureRect() const {
	//return sf::IntRect({ this->indexAnimation * this->sizex, y * this->sizey }, { this->sizex, this->sizey });
//}
bool SingleAnimationObject::isAnimationAtTheEnd() const {
	int temp = this->m_endingIndexAnimation;
	return (this->m_indexAnimation == temp);
}
void SingleAnimationObject::setRotation(const sf::Angle angle) {
	m_angle = angle;
}
AnimationDirection SingleAnimationObject::getAnimationDirection() const {
	return m_direction;
}
