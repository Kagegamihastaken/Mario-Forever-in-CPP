#pragma once
#ifndef LOCALANIMATIONMANAGER_HPP
#define LOCALANIMATIONMANAGER_HPP
#include "Core/AnimationSequenceManager.hpp"
#include "Core/SimpleSprite.hpp"
#include "Core/Loading/enum.hpp"

namespace MFCPP {
	class SingleAnimationObject {
		uint32_t m_indexAnimation = 0;
		uint32_t m_startingIndexAnimation = 0;
		uint32_t m_endingIndexAnimation = 0;
		float m_TimeRan = 0.0f;
		sf::Angle m_angle = sf::degrees(0.f);
		uint8_t m_frequency = 50;
		bool m_reached_the_end = false;
		bool m_loop = true;
		sf::Color m_color;
		AnimationDirection m_direction = AnimationDirection::ANIM_LEFT;
		//cache
		bool m_precompute_update = false;
		AnimationSequence* m_sequence_cache = nullptr;
		SimpleSprite* m_precompute_cache = nullptr;
	public:
		void setAnimation(uint32_t startingIndexAnimation, uint32_t endingIndexAnimation, uint8_t frequency = 50, bool loop = false);
		void setRangeIndexAnimation(uint32_t startingIndexAnimation, uint32_t endingIndexAnimation, uint8_t frequency = 50);
		void setIndexAnimation(uint32_t indexAnimation);
		void setStartingIndexAnimation(uint32_t startingIndexAnimation);
		void setEndingIndexAnimation(uint32_t endingIndexAnimation);
		void setFrequencyAnimation(uint32_t frequency);

		void setColor(sf::Color color);
		void frameTimeAccumulate(float deltaTime);
		void frameUpdate();
		void animationUpdate(sf::Vector2f pos, sf::Vector2f origin);
		void animationDraw() const;
		void setAnimationDirection(AnimationDirection dir);
		[[nodiscard]] bool isAnimationAtTheEnd() const;
		[[nodiscard]] AnimationDirection getAnimationDirection() const;
		[[nodiscard]] int32_t getFrequency() const;
		void setAnimationSequence(std::string_view val);
		void setRotation(sf::Angle angle);
		void setLoop(bool val);
	};
}
#endif // LOCALANIMATIONMANAGER_HPP