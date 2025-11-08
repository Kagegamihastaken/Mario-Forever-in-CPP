#include "Block/LuckyBlock.hpp"
#include "Core/WindowFrame.hpp"
#include "Object/Coin.hpp"
#include "Core/Scroll.hpp"
#include "Effect/CoinEffect.hpp"
#include "Core/Loading/enum.hpp"
#include "Object/GoombaAI.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Sound.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Object/BroAI.hpp"
#include "Object/Mario.hpp"

plf::colony<MFCPP::LuckyBlock> LuckyBlock;
static bool LuckyBlockDeleteGate = false;
static std::vector<std::string> NormLuckyBlockAnimName;
static std::vector<std::string> TreeLuckyBlockAnimName;
void LuckyBLockTemp() {
	for (int i = 0; i < 3; ++i) {
		ImageManager::AddTexture(fmt::format("NormalLuckyBlock_{}", i), "data/resources/luckyblock.png", sf::IntRect({i * 32, 0}, {32, 32}));
		NormLuckyBlockAnimName.emplace_back(fmt::format("NormalLuckyBlock_{}", i));
	}
	ImageManager::AddTexture("NormalLuckyBlockHit", "data/resources/luckyblock.png", sf::IntRect({96, 0}, {32, 32}));
	NormLuckyBlockAnimName.emplace_back("NormalLuckyBlockHit");
	ImageManager::AddTexture("TreeLuckyBlock", "data/resources/luckyblock.png", sf::IntRect({64, 32}, {32, 32}));
	TreeLuckyBlockAnimName.emplace_back("TreeLuckyBlock");
	ImageManager::AddTexture("TreeLuckyBlockHit", "data/resources/luckyblock.png", sf::IntRect({96, 32}, {32, 32}));
	TreeLuckyBlockAnimName.emplace_back("TreeLuckyBlockHit");
}
void ForceLoadLuckyBlockTexture(const LuckyBlockID ID) {
	switch (ID) {
		case LUCKY_BLOCK:
			for (int i = 0; i < 3; ++i)
				ImageManager::LoadTexture(fmt::format("NormalLuckyBlock_{}", i));
			ImageManager::LoadTexture("NormalLuckyBlockHit");
			break;
		case TREE_LUCKY_BLOCK:
			ImageManager::LoadTexture("TreeLuckyBlock");
			ImageManager::LoadTexture("TreeLuckyBlockHit");
			break;
		default: ;
	}
}
void LoadLuckyBlock() {
	for (int i = 0; i < 3; ++i) {
		ImageManager::PreloadTexture(fmt::format("NormalLuckyBlock_{}", i), "data/resources/luckyblock.png", sf::IntRect({i * 32, 0}, {32, 32}));
		NormLuckyBlockAnimName.emplace_back(fmt::format("NormalLuckyBlock_{}", i));
	}
	ImageManager::PreloadTexture("NormalLuckyBlockHit", "data/resources/luckyblock.png", sf::IntRect({96, 0}, {32, 32}));
	NormLuckyBlockAnimName.emplace_back("NormalLuckyBlockHit");
	ImageManager::PreloadTexture("TreeLuckyBlock", "data/resources/luckyblock.png", sf::IntRect({64, 32}, {32, 32}));
	TreeLuckyBlockAnimName.emplace_back("TreeLuckyBlock");
	ImageManager::PreloadTexture("TreeLuckyBlockHit", "data/resources/luckyblock.png", sf::IntRect({96, 32}, {32, 32}));
	TreeLuckyBlockAnimName.emplace_back("TreeLuckyBlockHit");
}
void SetPrevLuckyBlockPos() {
	for (auto & i : LuckyBlock) {
		if (i.isDestroyed()) continue;

		i.setPreviousPosition(i.getCurrentPosition());
	}
}
void InterpolateLuckyBlockPos(const float alpha) {
	for (auto & i : LuckyBlock) {
		if (i.isDestroyed()) continue;

		i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
	}
}
void AddLuckyBlock(const LuckyBlockID ID, const LuckyBlockAtt Att, float x, float y) {
	plf::colony<MFCPP::LuckyBlock>::colony_iterator<false> it;
	switch (ID) {
		case LUCKY_BLOCK:
			it = LuckyBlock.emplace(ID, Att, sf::Vector2f(x, y));
			it->setAnimation(0, 2, 9);
			it->setAnimationSequence(NormLuckyBlockAnimName);
			break;
		case TREE_LUCKY_BLOCK:
			it = LuckyBlock.emplace(ID, Att, sf::Vector2f(x, y));
			it->setAnimation(0, 0, 100);
			it->setAnimationSequence(TreeLuckyBlockAnimName);
			break;
		default: ;
	}
}
void LuckyBlockDraw() {
	for (auto it = LuckyBlock.begin(); it != LuckyBlock.end(); ++it) {
		if (it->isDestroyed()) continue;

		if (!isOutScreen(it->getInterpolatedPosition().x, it->getInterpolatedPosition().y, 32, 32)) {
			it->AnimationUpdate(it->getInterpolatedPosition(), it->getOrigin());
			it->AnimationDraw();
		}
	}
}
void LuckyBlockUpdate(const float deltaTime) {
	for (auto &i : LuckyBlock) {
		if (i.isDestroyed()) continue;

		if (i.getState()) {
			if (i.getAtt() == LUCKY_COIN) {
				if (!i.getUpDown()) {
					if (i.getStateCount() < 11.0f) {
						i.move(sf::Vector2f(0.f, - (i.getStateCount() < 6.0f ? 3.0f : (i.getStateCount() < 10.0f ? 2.0f : 1.0f)) * deltaTime));
						i.setStateCount(i.getStateCount() + (i.getStateCount() < 6.0f ? 3.0f : (i.getStateCount() < 10.0f ? 2.0f : 1.0f)) * deltaTime);
					}
					else {
						i.setStateCount(11.f);
						i.setUpDown(true);
					}
				}
				else {
					if (i.getStateCount() > 0.0f) {
						i.move(sf::Vector2f(0.f, (i.getStateCount() > 10.0f ? 1.0f : (i.getStateCount() > 6.0f ? 2.0f : 3.0f)) * deltaTime));
						i.setStateCount(i.getStateCount() - (i.getStateCount() > 10.0f ? 1.0f : (i.getStateCount() > 6.0f ? 2.0f : 3.0f)) * deltaTime);
					}
					else {
						i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, i.getYPos()));
						i.setStateCount(0.f);
						i.setUpDown(false);
						i.setState(false);
					}
				}
			}
			else {
				if (!i.getUpDown()) {
					if (i.getStateCount() < 4.0f) {
						i.move(sf::Vector2f(0.f, - 1.f * deltaTime));
						i.setStateCount(i.getStateCount() + 1.f * deltaTime);
					}
					else {
						i.setStateCount(4.f);
						i.setUpDown(true);
					}
				}
				else {
					if (i.getStateCount() > 0.0f) {
						i.move(sf::Vector2f(0.f, 1.f * deltaTime));
						i.setStateCount(i.getStateCount() - 1.f * deltaTime);
					}
					else {
						i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, i.getYPos()));
						i.setStateCount(0.f);
						i.setUpDown(false);
						i.setState(false);
					}
				}
			}
		}
	}
}
void LuckyBlockHitUpdate(const plf::colony<MFCPP::LuckyBlock>::colony_iterator<false>& it) {
	switch (it->getID()) {
		case LUCKY_BLOCK:
			it->setAnimation(3, 3, 100);
			break;
		case TREE_LUCKY_BLOCK:
			it->setAnimation(1, 1, 100);
			break;
	}
}
void LuckyHitIndex(const plf::colony<MFCPP::LuckyBlock>::colony_iterator<false>& it) {
	if (!it->getState() && !it->WasHit()) {
		switch (it->getAtt()) {
		case LUCKY_COIN:
			SoundManager::PlaySound("Coin");
			AddCoinEffect(COIN_NORMAL, ONE_COIN, it->getCurrentPosition().x + 15.0f, it->getCurrentPosition().y);
			++CoinCount;
			break;
		case LUCKY_MUSHROOM:
			SoundManager::PlaySound("Vine");
			AddGoombaAI(MUSHROOM, 0, it->getCurrentPosition().x + 16.0f, it->getCurrentPosition().y, RIGHT);
			break;
		case LUCKY_FIRE_FLOWER:
			SoundManager::PlaySound("Vine");
			if (PowerState == 0)
				AddGoombaAI(MUSHROOM, 0, it->getCurrentPosition().x + 16.0f, it->getCurrentPosition().y, RIGHT);
			else
				AddGoombaAI(FIRE_FLOWER, 0, it->getCurrentPosition().x + 16.0f, it->getCurrentPosition().y, RIGHT);
			break;
		case LUCKY_GREEN_MUSHROOM:
			SoundManager::PlaySound("Vine");
			AddGoombaAI(MUSHROOM, 1, it->getCurrentPosition().x + 16.0f, it->getCurrentPosition().y, RIGHT);
			break;
		default: ;
		}
		LuckyBlockHitUpdate(it);
		it->setState(true);
		it->setWasHit(true);
		it->setStateCount(0.f);
		it->setUpDown(false);
	}
}
void LuckyHit(const float x, const float y) {
	for (auto it = LuckyBlock.begin(); it != LuckyBlock.end(); ++it) {
		if (it->getCurrentPosition().x == x && it->getCurrentPosition().y == y)
			LuckyHitIndex(it);
	}
}
void LuckyHitEvent(const float x, const float y) {
	for (auto it = LuckyBlock.begin(); it != LuckyBlock.end(); ++it) {
		if (it->getCurrentPosition().x == x && it->getCurrentPosition().y == y && !it->getState() && !it->WasHit()) {
			sf::FloatRect LuckyLoop = getGlobalHitbox(it->getHitbox(), it->getCurrentPosition(), it->getOrigin());
			LuckyLoop.position.y -= 16.0f;
			for (auto jt = CoinList.begin(); jt != CoinList.end(); ++jt) {
				if (jt->isDestroyed()) continue;
				if (sf::FloatRect CoinCollide = getGlobalHitbox(jt->getHitbox(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(CoinCollide, LuckyLoop)) {
					AddCoinEffect(jt->getID(), jt->getAttribute(), jt->getCurrentPosition().x + 15.0f, jt->getCurrentPosition().y + 32.0f);
					DeleteIndexCoin(jt);
					SoundManager::PlaySound("Coin");
					++CoinCount;
				}
			}
			for (auto jt = GoombaAIList.begin(); jt != GoombaAIList.end(); ++jt) {
				if (jt->isDestroyed()) continue;
				if (sf::FloatRect GoombaAICollide = getGlobalHitbox(jt->GetHitboxMain(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(GoombaAICollide, LuckyLoop)) {
					jt->DeathBehaviour(SCORE_100);
					if (jt->IsCanDeath()) DeleteGoombaAIIndex(jt);
				}
			}
			for (auto jt = BroAIList.begin(); jt != BroAIList.end(); ++jt) {
				if (jt->isDestroyed()) continue;
				if (sf::FloatRect BroAICollide = getGlobalHitbox(jt->getHitbox(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(BroAICollide, LuckyLoop)) {
					jt->DeathBehaviour(SCORE_200);
					SoundManager::PlaySound("Kick2");
					DeleteBroAIIndex(jt);
				}
			}
			LuckyHitIndex(it);
			break;
		}
	}
}
void DeleteLuckyBlockIndex(const plf::colony<MFCPP::LuckyBlock>::colony_iterator<false>& it) {
	it->setDestroyed(true);
	LuckyBlockDeleteGate = true;
}
void DeleteLuckyBlock(const float x, const float y) {
	for (auto it = LuckyBlock.begin(); it != LuckyBlock.end(); ++it) {
		if (it->getCurrentPosition().x == x && it->getCurrentPosition().y == y) {
			DeleteLuckyBlockIndex(it);
			break;
		}
	}
}
void DeleteAllLuckyBlock() {
	LuckyBlock.clear();
}
void LuckyBlockCleanup() {
	if (!LuckyBlockDeleteGate) return;
	auto it = LuckyBlock.begin();
	while (it != LuckyBlock.end()) {
		if (!it->isDestroyed()) ++it;
		else it = LuckyBlock.erase(it);
	}
	LuckyBlockDeleteGate = false;
}