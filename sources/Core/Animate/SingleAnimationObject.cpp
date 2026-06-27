#include "Core/WindowFrame.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"

#include "Core/AnimationSequenceManager.hpp"
#include "Core/Exception.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Logging.hpp"
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
		m_precompute_update = true;
	}
	void SingleAnimationObject::setAnimationSequence(std::string_view val) {
		m_sequence_cache = &AnimationSequenceManager::getData(val);
		m_precompute_update = true;
	}
	void SingleAnimationObject::setRangeIndexAnimation(const uint32_t startingIndexAnimation, const uint32_t endingIndexAnimation, const uint8_t frequency) {
		m_precompute_update = true;
		m_TimeRan = 0.0f;
		m_frequency = frequency;
		if (m_startingIndexAnimation != startingIndexAnimation || m_endingIndexAnimation != endingIndexAnimation) {
			m_startingIndexAnimation = startingIndexAnimation;
			m_endingIndexAnimation = endingIndexAnimation;
			m_indexAnimation = startingIndexAnimation;
			m_color = sf::Color(255, 255, 255);
		}
	}

	void SingleAnimationObject::setColor(sf::Color color) {
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
				m_precompute_update = true;
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
	void SingleAnimationObject::animationUpdate(sf::Vector2f pos, sf::Vector2f origin) {
		frameUpdate();
		if (m_precompute_update) {
			m_precompute_cache = &ImageManager::getSpritePreCompute(m_sequence_cache->operator[](m_indexAnimation));
			m_precompute_update = false;
		}
		if (!m_precompute_cache) throw std::bad_alloc();
		const sf::IntRect rect = m_precompute_cache->getTextureRect();
		m_precompute_cache->setRotation(m_angle);
		m_precompute_cache->setPosition(pos);
		m_precompute_cache->setOrigin(origin);
		m_precompute_cache->setColor(m_color);
		if (m_direction == AnimationDirection::ANIM_RIGHT)
			m_precompute_cache->setTextureRect(sf::IntRect({0, rect.position.y}, {std::abs(rect.size.x), rect.size.y}));
		else
			m_precompute_cache->setTextureRect(sf::IntRect({std::abs(rect.size.x), rect.position.y}, {-std::abs(rect.size.x), rect.size.y}));
	}
	void SingleAnimationObject::animationDraw() const {
		//ImageManager::AddToVertex(m_AnimName[m_indexAnimation], m_Index[m_indexAnimation].getTextureRect(), m_Index[m_indexAnimation].getTransform(), m_Index[m_indexAnimation].getColor());
		WindowFrame::getWindow().draw(*m_precompute_cache);
	}
	void SingleAnimationObject::setAnimationDirection(AnimationDirection dir) {
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

	int32_t SingleAnimationObject::getFrequency() const {
		return m_frequency;
	}

	void SingleAnimationObject::setLoop(bool val) {
		m_loop = val;
	}
}
