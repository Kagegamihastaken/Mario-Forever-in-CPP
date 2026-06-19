#include "Core/WindowFrame.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"

#include "Core/AnimationSequenceManager.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Loading/enum.hpp"

namespace MFCPP {
	void SingleAnimationObject::setAnimation(const uint32_t startingIndexAnimation, const uint32_t endingIndexAnimation, const uint8_t frequency, const bool loop) {
		m_startingIndexAnimation = startingIndexAnimation;
		m_indexAnimation = startingIndexAnimation;
		m_endingIndexAnimation = endingIndexAnimation;
		m_frequency = frequency;
		m_TimeRan = 0.0f;
		m_loop = loop;
		m_direction = AnimationDirection::ANIM_RIGHT;
		m_color = sf::Color(255, 255, 255);
	}
	void SingleAnimationObject::setAnimationSequence(std::string_view aName) {
		m_sequenceName = aName;
	}
	void SingleAnimationObject::setRangeIndexAnimation(const uint32_t startingIndexAnimation, const uint32_t endingIndexAnimation, const uint8_t frequency) {
		if (m_startingIndexAnimation != startingIndexAnimation || m_endingIndexAnimation != endingIndexAnimation) {
			m_startingIndexAnimation = startingIndexAnimation;
			m_endingIndexAnimation = endingIndexAnimation;
			m_frequency = frequency;
			m_indexAnimation = startingIndexAnimation;
			m_TimeRan = 0.0f;
			m_color = sf::Color(255, 255, 255);
		}
		else {
			m_frequency = frequency;
			m_TimeRan = 0.0f;
		}
	}

	void SingleAnimationObject::setColor(const sf::Color &color) {
		m_color = color;
	}
	void SingleAnimationObject::setIndexAnimation(const uint32_t indexAnimation) {
		m_indexAnimation = indexAnimation;
	}
	void SingleAnimationObject::setStartingIndexAnimation(const uint32_t startingIndexAnimation) {
		m_startingIndexAnimation = startingIndexAnimation;
	}
	void SingleAnimationObject::setEndingIndexAnimation(const uint32_t endingIndexAnimation) {
		m_endingIndexAnimation = endingIndexAnimation;
	}
	void SingleAnimationObject::setFrequencyAnimation(const uint32_t frequency) {
		m_frequency = frequency;
	}

	void SingleAnimationObject::frameTimeAccumulate(float deltaTime) {
		m_TimeRan += deltaTime;
	}
	void SingleAnimationObject::frameUpdate() {
		if (m_frequency != 0) {
			if (const float FrameTime = 100.0f / static_cast<float>(m_frequency); m_TimeRan >= FrameTime) {
				const auto FrameCount = static_cast<int>(m_TimeRan / FrameTime);
				if (m_indexAnimation - m_startingIndexAnimation + FrameCount >= m_endingIndexAnimation - m_startingIndexAnimation + 1 && !m_reached_the_end) m_reached_the_end = true;
				if (m_loop)
					m_indexAnimation = m_startingIndexAnimation + (m_indexAnimation - m_startingIndexAnimation + FrameCount) % (m_endingIndexAnimation - m_startingIndexAnimation + 1);
				else
					m_indexAnimation = m_startingIndexAnimation + std::min((m_indexAnimation - m_startingIndexAnimation + FrameCount),(m_endingIndexAnimation - m_startingIndexAnimation));

				m_TimeRan -= static_cast<float>(FrameCount) * FrameTime;
			}
		}
	}
	void SingleAnimationObject::animationUpdate(const sf::Vector2f& pos, const sf::Vector2f& origin) {
		frameUpdate();
		SimpleSprite& index = ImageManager::getSpritePreCompute(AnimationSequenceManager::getData(m_sequenceName)[m_indexAnimation]);
		const sf::IntRect rect = index.getTextureRect();
		index.setRotation(m_angle);
		index.setPosition(pos);
		index.setOrigin(origin);
		index.setColor(m_color);
		if (m_direction == AnimationDirection::ANIM_RIGHT)
			index.setTextureRect(sf::IntRect({0, rect.position.y}, {std::abs(rect.size.x), rect.size.y}));
		else
			index.setTextureRect(sf::IntRect({std::abs(rect.size.x), rect.position.y}, {-std::abs(rect.size.x), rect.size.y}));
	}
	void SingleAnimationObject::animationDraw() const {
		SimpleSprite& index = ImageManager::getSpritePreCompute(AnimationSequenceManager::getData(m_sequenceName)[m_indexAnimation]);
		//ImageManager::AddToVertex(m_AnimName[m_indexAnimation], m_Index[m_indexAnimation].getTextureRect(), m_Index[m_indexAnimation].getTransform(), m_Index[m_indexAnimation].getColor());
		WindowFrame::getWindow().draw(index);
	}
	void SingleAnimationObject::setAnimationDirection(const AnimationDirection& dir) {
		m_direction = dir;
	}
	bool SingleAnimationObject::isAnimationAtTheEnd() const {
		return m_reached_the_end;
	}
	void SingleAnimationObject::setRotation(const sf::Angle angle) {
		m_angle = angle;
	}
	AnimationDirection SingleAnimationObject::getAnimationDirection() const {
		return m_direction;
	}

	int SingleAnimationObject::getFrequency() const {
		return m_frequency;
	}

	void SingleAnimationObject::setLoop(bool val) {
		m_loop = val;
	}
}
