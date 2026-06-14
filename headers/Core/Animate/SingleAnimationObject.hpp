#pragma once
#ifndef LOCALANIMATIONMANAGER_HPP
#define LOCALANIMATIONMANAGER_HPP
#include "Core/SimpleSprite.hpp"
#include "Core/Loading/enum.hpp"

namespace MFCPP {
	class SingleAnimationObject {
		std::string m_lastAnim = "";
		std::vector<SimpleSprite> m_Index;
		std::vector<std::string> m_AnimName;
		sf::Clock m_TimeRun;
		uint32_t m_indexAnimation = 0;
		uint32_t m_startingIndexAnimation = 0;
		uint32_t m_endingIndexAnimation = 0;
		float m_TimeRan = 0.0f;
		float m_TimeRemainSave = 0.0f;
		sf::Angle m_angle = sf::degrees(0.f);
		uint8_t m_frequency = 50;
		bool m_reached_the_end = false;
		bool m_loop = true;
		sf::Color m_color;
		AnimationDirection m_direction = AnimationDirection::ANIM_LEFT;
	public:
		void setAnimation(uint32_t startingIndexAnimation, uint32_t endingIndexAnimation, uint8_t frequency = 50, bool loop = false);
		void setRangeIndexAnimation(uint32_t startingIndexAnimation, uint32_t endingIndexAnimation, uint8_t frequency = 50);
		void setIndexAnimation(uint32_t indexAnimation);
		void setStartingIndexAnimation(uint32_t startingIndexAnimation);
		void setEndingIndexAnimation(uint32_t endingIndexAnimation);
		void setFrequencyAnimation(uint32_t frequency);

		void setColor(const sf::Color& color);
		void frameUpdate();
		void animationUpdate(const sf::Vector2f& pos, const sf::Vector2f& origin);
		void animationDraw() const;
		void setAnimationDirection(const AnimationDirection& dir);
		//sf::IntRect getAnimationTextureRect() const;
		[[nodiscard]] bool isAnimationAtTheEnd() const;
		[[nodiscard]] AnimationDirection getAnimationDirection() const;
		[[nodiscard]] int getFrequency() const;
		void addAnimationSequence(const std::string& aName);
		void setAnimationSequence(const std::vector<std::string>& aName);
		void setRotation(sf::Angle angle);
		void setLoop(bool val);
	};
}
#endif // LOCALANIMATIONMANAGER_HPP