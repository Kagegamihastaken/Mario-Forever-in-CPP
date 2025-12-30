#include "Effect/GoombaAIEffect.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/ImageManager.hpp"

void GoombaAIEffectInit() {
	ImageManager::AddTexture("DEAD_Goomba_1", "data/resources/Goomba/DEAD_Goomba.png", sf::IntRect({0, 0}, {31, 32}));
	ImageManager::AddTexture("DEAD_Goomba_2", "data/resources/Goomba/DEAD_Goomba.png", sf::IntRect({31, 0}, {31, 32}));
	ImageManager::AddTexture("DEAD_GreenKoopa", "data/resources/Koopa/DEAD_GreenKoopa.png");
	ImageManager::AddTexture("DEAD_RedKoopa", "data/resources/Koopa/DEAD_RedKoopa.png");
	ImageManager::AddTexture("DEAD_Spiny_Red", "data/resources/Spiny/DEAD_RedSpiny.png");
	ImageManager::AddTexture("DEAD_Spiny_Green", "data/resources/Spiny/DEAD_GreenSpiny.png");
}