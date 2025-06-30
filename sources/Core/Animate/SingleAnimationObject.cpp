#include "Core/WindowFrame.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Loading/enum.hpp"

void SingleAnimationObject::setAnimation(const int startingIndexAnimation, const int endingIndexAnimation, const int frequency) {
	m_startingIndexAnimation = startingIndexAnimation;
	m_indexAnimation = startingIndexAnimation;
	m_endingIndexAnimation = endingIndexAnimation;
	m_frequency = frequency;
	m_TimeRun.restart();
	m_TimeRan = 0.0f;
	m_TimeRemainSave = 0.0f;
	m_direction = AnimationDirection::ANIM_LEFT;
}
void SingleAnimationObject::AddAnimationSequence(const std::string& a_left, const std::string& a_right) {
	m_LeftIndex.emplace_back(*ImageManager::GetReturnTexture(a_left));
	m_RightIndex.emplace_back(*ImageManager::GetReturnTexture(a_right));
}
void SingleAnimationObject::setAnimationSequence(const std::vector<std::string>& s_left, const std::vector<std::string>& s_right) {
	if (s_left.size() != s_right.size()) {
		std::cout << "Size of 2 directions must be equal\n";
		return;
	}
	m_LeftIndex.clear();
	m_RightIndex.clear();
	for (const auto& str : s_left)
		m_LeftIndex.emplace_back(*ImageManager::GetReturnTexture(str));
	for (const auto& str : s_right)
		m_RightIndex.emplace_back(*ImageManager::GetReturnTexture(str));
}
void SingleAnimationObject::setAnimationSequence(const std::vector<std::string>& s_left) {
	m_LeftIndex.clear();
	m_RightIndex.clear();
	for (const auto& str : s_left) {
		m_LeftIndex.emplace_back(*ImageManager::GetReturnTexture(str));
		m_RightIndex.emplace_back(*ImageManager::GetReturnTexture(str));
	}
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

	if (m_direction == AnimationDirection::ANIM_RIGHT) {
		m_RightIndex[m_indexAnimation].setRotation(m_angle);
		m_RightIndex[m_indexAnimation].setPosition(pos);
		m_RightIndex[m_indexAnimation].setOrigin(origin);
	}
	else {
		m_LeftIndex[m_indexAnimation].setRotation(m_angle);
		m_LeftIndex[m_indexAnimation].setPosition(pos);
		m_LeftIndex[m_indexAnimation].setOrigin(origin);
	}
}
void SingleAnimationObject::AnimationDraw(sf::RenderWindow& window, sf::RenderStates states) const {
	if (m_direction == AnimationDirection::ANIM_RIGHT) {
		//states.transform *= m_RightIndex[m_indexAnimation].getTransform();
		//states.texture = &m_RightIndex[m_indexAnimation].getTexture();
		window.draw(m_RightIndex[m_indexAnimation], states);
	}
	else {
		//states.transform *= m_LeftIndex[m_indexAnimation].getTransform();
		//states.texture = &m_LeftIndex[m_indexAnimation].getTexture();
		window.draw(m_LeftIndex[m_indexAnimation], states);
	}
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
