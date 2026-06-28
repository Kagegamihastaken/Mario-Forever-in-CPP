#include "Effect/GoombaAIEffect.hpp"

#include "Core/AnimationSequenceManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/ImageManager.hpp"

void GoombaAIEffectInit() {
	ImageManager::AddTexture("DEAD_Goomba_1", "data/resources/Goomba/DEAD_Goomba.png", sf::IntRect({0, 0}, {31, 32}));
	ImageManager::AddTexture("DEAD_Goomba_2", "data/resources/Goomba/DEAD_Goomba.png", sf::IntRect({31, 0}, {31, 32}));
	ImageManager::AddTexture("DEAD_GreenKoopa", "data/resources/Koopa/DEAD_GreenKoopa.png");
	ImageManager::AddTexture("DEAD_RedKoopa", "data/resources/Koopa/DEAD_RedKoopa.png");
	ImageManager::AddTexture("DEAD_Spiny_Red", "data/resources/Spiny/DEAD_RedSpiny.png");
	ImageManager::AddTexture("DEAD_Spiny_Green", "data/resources/Spiny/DEAD_GreenSpiny.png");
	MFCPP::AnimationSequenceManager::newData("GoombaDeathEffectFirst");
	MFCPP::AnimationSequenceManager::addSingleFrame("GoombaDeathEffectFirst", "DEAD_Goomba_1");
	MFCPP::AnimationSequenceManager::newData("GoombaDeathEffectSecond");
	MFCPP::AnimationSequenceManager::addSingleFrame("GoombaDeathEffectSecond", "DEAD_Goomba_2");
	MFCPP::AnimationSequenceManager::newData("GreenKoopaDeathEffect");
	MFCPP::AnimationSequenceManager::addSingleFrame("GreenKoopaDeathEffect", "DEAD_GreenKoopa");
	MFCPP::AnimationSequenceManager::newData("RedKoopaDeathEffect");
	MFCPP::AnimationSequenceManager::addSingleFrame("RedKoopaDeathEffect", "DEAD_RedKoopa");
	MFCPP::AnimationSequenceManager::newData("RedSpinyDeathEffect");
	MFCPP::AnimationSequenceManager::addSingleFrame("RedSpinyDeathEffect", "DEAD_Spiny_Red");
	MFCPP::AnimationSequenceManager::newData("GreenSpinyDeathEffect");
	MFCPP::AnimationSequenceManager::addSingleFrame("GreenSpinyDeathEffect", "DEAD_Spiny_Green");
}