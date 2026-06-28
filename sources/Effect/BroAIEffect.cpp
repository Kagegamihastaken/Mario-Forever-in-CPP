#include "Effect/BroAIEffect.hpp"

#include "Core/AnimationSequenceManager.hpp"
#include "Core/ImageManager.hpp"

void BroAIEffectInit() {
	ImageManager::AddTexture("DEAD_HammerBro", "data/resources/Bro/DEAD_HammerBro.png");
	ImageManager::AddTexture("DEAD_FireBro", "data/resources/Bro/DEAD_FireBro.png");
	MFCPP::AnimationSequenceManager::newData("HammerBroDeath");
	MFCPP::AnimationSequenceManager::addSingleFrame("HammerBroDeath", "DEAD_HammerBro");
	MFCPP::AnimationSequenceManager::newData("FireBroDeath");
	MFCPP::AnimationSequenceManager::addSingleFrame("FireBroDeath", "DEAD_FireBro");
}