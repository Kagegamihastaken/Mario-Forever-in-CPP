#include "Core/Checkpoint.hpp"
#include "Core/Class/CheckpointClass.hpp"
#include "Core/ExternalHeaders/plf_colony.h"
#include "Core/ImageManager.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Collision/Collide.hpp"
#include "Object/Mario.hpp"

static plf::colony<MFCPP::Checkpoint> CheckpointList;
static constexpr int CHECKPOINT_IMAGE_WIDTH = 321;
static constexpr int CHECKPOINT_HEIGHT = 112;
static constexpr int CHECKPOINT_WIDTH = 107;
static std::vector<std::string> CheckpointAnimName;
static sf::Vector2f StartPos(0.f, 0.f);

void CheckpointInit() {
    for (int i = 0; i < CHECKPOINT_IMAGE_WIDTH / CHECKPOINT_WIDTH; ++i) {
        ImageManager::AddTexture(fmt::format("Checkpoint_{}", i), "data/resources/Checkpoint.png", sf::IntRect({ CHECKPOINT_WIDTH * i, 0}, {CHECKPOINT_WIDTH, CHECKPOINT_HEIGHT}));
        CheckpointAnimName.emplace_back(fmt::format("Checkpoint_{}", i));
    }
}
void AddCheckpoint(const sf::Vector2f& pos) {
    auto it = CheckpointList.emplace(sf::FloatRect({0.f, 0.f}, {CHECKPOINT_WIDTH, CHECKPOINT_HEIGHT}), pos, sf::Vector2f(45.f, 112.f));
    it->setAnimationSequence(CheckpointAnimName);
}
void CheckpointAnimationUpdate() {
    for (auto it = CheckpointList.begin(); it != CheckpointList.end(); ++it) {
        if (it->getLastState() == it->getTouch()) continue;

        if (!it->getTouch()) it->SetRangeIndexAnimation(0, 0, 10);
        else it->SetRangeIndexAnimation(1, 2, 10);
        it->setLastState(it->getTouch());
    }
}
void CheckpointCollision() {
    const sf::FloatRect playerHitbox = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());
    for (auto it = CheckpointList.begin(); it != CheckpointList.end(); ++it) {
        if (it->getTouch()) continue;

        if (sf::FloatRect loopHitbox = getGlobalHitbox(it->getHitbox(), it->getInterpolatedPosition(), it->getOrigin()); isCollide(loopHitbox, playerHitbox)) {
            it->setTouch(true);
            StartPos = it->getCurrentPosition();
            SoundManager::PlaySound("Vine");
        }
    }
}
void CheckpointDraw() {
    for (auto it = CheckpointList.begin(); it != CheckpointList.end(); ++it) {
        if (!isOutScreen(it->getInterpolatedPosition().x, it->getInterpolatedPosition().y, CHECKPOINT_WIDTH, CHECKPOINT_HEIGHT)) {
            it->AnimationUpdate(it->getInterpolatedPosition(), it->getOrigin());
            it->AnimationDraw();
        }
    }
}
void setStartPosition(const sf::Vector2f& pos) {
    StartPos = pos;
}
sf::Vector2f getStartPosition() {
    return StartPos;
}