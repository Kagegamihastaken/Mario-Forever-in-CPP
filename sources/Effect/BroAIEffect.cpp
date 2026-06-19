#include "Effect/BroAIEffect.hpp"

#include "Core/AnimationSequenceManager.hpp"
#include "Core/ImageManager.hpp"

const std::vector<std::string> HammerBroDeath = {"DEAD_HammerBro"};
const std::vector<std::string> FireBroDeath = {"DEAD_FireBro"};

void BroAIEffectInit() {
	ImageManager::AddTexture("DEAD_HammerBro", "data/resources/Bro/DEAD_HammerBro.png");
	ImageManager::AddTexture("DEAD_FireBro", "data/resources/Bro/DEAD_FireBro.png");
	MFCPP::AnimationSequenceManager::addData("HammerBroDeath", HammerBroDeath);
	MFCPP::AnimationSequenceManager::addData("FireBroDeath", FireBroDeath);
}