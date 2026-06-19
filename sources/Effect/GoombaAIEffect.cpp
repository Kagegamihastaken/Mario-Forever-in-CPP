#include "Effect/GoombaAIEffect.hpp"

#include "Core/AnimationSequenceManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/ImageManager.hpp"

const std::vector<std::string> GoombaDeathEffectFirst = {"DEAD_Goomba_1"};
const std::vector<std::string> GoombaDeathEffectSecond = {"DEAD_Goomba_2"};
const std::vector<std::string> GreenKoopaDeathEffect = {"DEAD_GreenKoopa"};
const std::vector<std::string> RedKoopaDeathEffect = {"DEAD_RedKoopa"};
const std::vector<std::string> RedSpinyDeathEffect = {"DEAD_Spiny_Red"};
const std::vector<std::string> GreenSpinyDeathEffect = {"DEAD_Spiny_Green"};

void GoombaAIEffectInit() {
	ImageManager::AddTexture("DEAD_Goomba_1", "data/resources/Goomba/DEAD_Goomba.png", sf::IntRect({0, 0}, {31, 32}));
	ImageManager::AddTexture("DEAD_Goomba_2", "data/resources/Goomba/DEAD_Goomba.png", sf::IntRect({31, 0}, {31, 32}));
	ImageManager::AddTexture("DEAD_GreenKoopa", "data/resources/Koopa/DEAD_GreenKoopa.png");
	ImageManager::AddTexture("DEAD_RedKoopa", "data/resources/Koopa/DEAD_RedKoopa.png");
	ImageManager::AddTexture("DEAD_Spiny_Red", "data/resources/Spiny/DEAD_RedSpiny.png");
	ImageManager::AddTexture("DEAD_Spiny_Green", "data/resources/Spiny/DEAD_GreenSpiny.png");
	MFCPP::AnimationSequenceManager::addData("GoombaDeathEffectFirst", GoombaDeathEffectFirst);
	MFCPP::AnimationSequenceManager::addData("GoombaDeathEffectSecond", GoombaDeathEffectSecond);
	MFCPP::AnimationSequenceManager::addData("GreenKoopaDeathEffect", GreenKoopaDeathEffect);
	MFCPP::AnimationSequenceManager::addData("RedKoopaDeathEffect", RedKoopaDeathEffect);
	MFCPP::AnimationSequenceManager::addData("RedSpinyDeathEffect", RedSpinyDeathEffect);
	MFCPP::AnimationSequenceManager::addData("GreenSpinyDeathEffect", GreenSpinyDeathEffect);

}