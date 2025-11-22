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

		int m_indexAnimation = 0;
		int m_startingIndexAnimation = 0;
		int m_endingIndexAnimation = 0;
		int m_frequency = 50;
		sf::Clock m_TimeRun;
		float m_TimeRan = 0.0f;
		float m_TimeRemainSave = 0.0f;
		bool m_change_direction = false;
		sf::Angle m_angle = sf::degrees(0.f);
		AnimationDirection m_direction = AnimationDirection::ANIM_LEFT;
	public:
		void setAnimation(int startingIndexAnimation, int endingIndexAnimation, int frequency = 50, bool changeAnimDirection = false);
		void SetRangeIndexAnimation(int startingIndexAnimation, int endingIndexAnimation, int frequency = 50);
		void setIndexAnimation(int indexAnimation);
		void setStartingIndexAnimation(int startingIndexAnimation);
		void setEndingIndexAnimation(int endingIndexAnimation);
		void setFrequencyAnimation(int frequency);

		void FrameUpdate();
		void AnimationUpdate(const sf::Vector2f& pos, const sf::Vector2f& origin);
		void AnimationDraw() const;
		void setAnimationDirection(const AnimationDirection& dir);
		//sf::IntRect getAnimationTextureRect() const;
		[[nodiscard]] bool isAnimationAtTheEnd() const;
		[[nodiscard]] AnimationDirection getAnimationDirection() const;
		void AddAnimationSequence(const std::string& aName);
		void setAnimationSequence(const std::vector<std::string>& aName);
		void setRotation(sf::Angle angle);
	};
}
#endif // LOCALANIMATIONMANAGER_HPP