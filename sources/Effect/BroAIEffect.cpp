#include "Effect/BroAIEffect.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/ImageManager.hpp"

void BroAIEffectInit() {
	ImageManager::AddTexture("DEAD_HammerBro", "data/resources/Bro/DEAD_HammerBro.png");
	ImageManager::AddTexture("DEAD_FireBro", "data/resources/Bro/DEAD_FireBro.png");
}