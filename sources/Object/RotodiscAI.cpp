#include "Object/RotodiscAI.hpp"
#include "Core/ImageManager.hpp"
std::vector<std::string> RotodiscAnimName;
static int ROTODISC_WIDTH = 34;
static int ROTODISC_HEIGHT = 32;
static int ROTODISC_WIDTH_IMAGE = 884;
void RotodiscInit() {
    ImageManager::AddTexture("RotodiscBase", "data/resources/RotodiscBase.png");
    for (int i = 0; i < ROTODISC_WIDTH_IMAGE / ROTODISC_WIDTH; i++) {
        ImageManager::AddTexture(fmt::format("Rotodisc_{}", i), "data/resources/Rotodisc.png", sf::IntRect({i * ROTODISC_WIDTH, 0}, {ROTODISC_WIDTH, ROTODISC_HEIGHT}));
        RotodiscAnimName.emplace_back(fmt::format("Rotodisc_{}", i));
    }
}