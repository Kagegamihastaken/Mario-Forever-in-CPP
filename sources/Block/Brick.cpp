#include "Block/Brick.hpp"
#include "Core/WindowFrame.hpp"
#include "Object/Coin.hpp"
#include "Core/Scroll.hpp"
#include "Effect/CoinEffect.hpp"
#include "Core/Loading/enum.hpp"
#include "Object/Mario.hpp"
#include "Effect/BrickParticle.hpp"
#include "Object/GoombaAI.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Sound.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Tilemap.hpp"
#include "Object/BroAI.hpp"

plf::colony<MFCPP::Brick> Bricks;
static bool BrickDeleteGate = false;
void BricksInitTemp() {
	ImageManager::AddTexture("NormalBrick", "data/resources/Brick.png", sf::IntRect({0, 0}, {32, 32}));
	ImageManager::AddTexture("NormalHittedBrick", "data/resources/Brick.png", sf::IntRect({0, 32}, {32, 32}));
	ImageManager::AddTexture("GrayBrick", "data/resources/Brick.png", sf::IntRect({32, 0}, {32, 32}));
	ImageManager::AddTexture("GrayHittedBrick", "data/resources/Brick.png", sf::IntRect({32, 32}, {32, 32}));
}
void ForceLoadBricksTexture(const BrickID ID) {
	switch (ID) {
		case BRICK_NORMAL:
			ImageManager::LoadTexture(R"(NormalBrick)");
			ImageManager::LoadTexture(R"(NormalHittedBrick)");
			ImageManager::LoadTexture(R"(NormalBrickParticle)");
			break;
		case BRICK_GRAY:
			ImageManager::LoadTexture("GrayBrick");
			ImageManager::LoadTexture("GrayHittedBrick");
			ImageManager::LoadTexture("GrayBrickParticle");
			break;
		default: ;
	}
}
void BricksInit() {
	// Loading Texture
	ImageManager::PreloadTexture("NormalBrick", "data/resources/Brick.png", sf::IntRect({0, 0}, {32, 32}));
	ImageManager::PreloadTexture("NormalHittedBrick", "data/resources/Brick.png", sf::IntRect({0, 32}, {32, 32}));
	ImageManager::PreloadTexture("GrayBrick", "data/resources/Brick.png", sf::IntRect({32, 0}, {32, 32}));
	ImageManager::PreloadTexture("GrayHittedBrick", "data/resources/Brick.png", sf::IntRect({32, 32}, {32, 32}));
	//ImageManager::Loadingtexture("data/resources/Brick.png", "Bricks", 0, 0, 64, 64);
}
void SetPrevBricksPos() {
	for (auto & Brick : Bricks) {
		Brick.setPreviousPosition(Brick.getCurrentPosition());
	}
}
void InterpolateBricksPos(const float alpha) {
	for (auto & Brick : Bricks) {
		Brick.setInterpolatedPosition(linearInterpolation(Brick.getPreviousPosition(), Brick.getCurrentPosition(), alpha));
	}
}
void AddBrick(const BrickID ID, const BrickAtt att, const float x, const float y) {
	const auto it = Bricks.emplace(ID, att, sf::Vector2f(x, y));
	switch (ID) {
	case BRICK_GRAY:
		it->setTexture("GrayBrick");
		break;
	case BRICK_NORMAL:
		it->setTexture("NormalBrick");
		break;
	}
}
void BrickHittedUpdate(const plf::colony<MFCPP::Brick>::colony_iterator<false>& it) {
	if (it->getID() == BRICK_GRAY) it->setTexture("GrayHittedBrick");
	else if (it->getID() == BRICK_NORMAL) it->setTexture("NormalHittedBrick");
}
void BrickDraw() {
	for (auto & Brick : Bricks) {
		if (!isOutScreen(Brick.getInterpolatedPosition().x, Brick.getInterpolatedPosition().y, 32, 32)) {
			Brick.AnimationUpdate(Brick.getInterpolatedPosition(), Brick.getOrigin());
			Brick.AnimationDraw(window);
		}
	}
}
void BrickUpdate(const float deltaTime) {
	for (auto &i : Bricks) {
		if (i.getState()) {
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
	}
}
void MultiBrickCoin(const float x, const float y, const plf::colony<MFCPP::Brick>::colony_iterator<false>& it) {
	if (it->getAtt() == MULTICOIN && !it->getDisabled()) {
		if (!it->getWasHit()) {
			it->setWasHit(true);
			it->restartClock();
		}
		else {
			if (it->getClock().getElapsedTime().asSeconds() > 6.0f && it->getAtt() == MULTICOIN) {
				it->setDisabled(true);
				it->setState(true);
				it->setUpDown(false);
				it->setStateCount(0.f);
				BrickHittedUpdate(it);
			}
		}
		SoundManager::PlaySound("Coin");
		AddCoinEffect(COIN_NORMAL, ONE_COIN, x + 15.0f, y);
		++CoinCount;
		it->setState(true);
		it->setUpDown(false);
		it->setStateCount(0.f);
	}
}
void BrickBreak(const float x, const float y) {
	for (auto it = Bricks.begin(); it != Bricks.end(); ++it) {
		if (it->getCurrentPosition().x == x && it->getCurrentPosition().y == y) {
			switch (it->getAtt()) {
				case NORMAL:
					//i.SetDirection(static_cast<GoombaAIDirection>(!i.GetDirection()));
					SoundManager::PlaySound("Break");
					AddBrickParticle(it->getID(), x, y);
					DeleteBrick(x, y);
					MFCPP::setIndexTilemapCollision(x, y, false);
					Score += 50;
					break;
				case MULTICOIN:
					MultiBrickCoin(x, y, it);
					break;
				default: ;
			}
		}
	}
}
void HitEvent(const float x, const float y) {
	for (auto i = Bricks.begin(); i != Bricks.end(); ++i) {
		if (i->getCurrentPosition().x == x && i->getCurrentPosition().y == y && !i->getState()) {
			sf::FloatRect BrickLoop = getGlobalHitbox(sf::FloatRect({0.f , 0.f}, {32.f, 32.f}), i->getCurrentPosition(), i->getOrigin());
			BrickLoop.position.y -= 16.0f;
			for (auto jt = CoinList.begin(); jt != CoinList.end(); ++jt) {
				if (sf::FloatRect CoinCollide = getGlobalHitbox(jt->getHitbox(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(CoinCollide, BrickLoop)) {
					AddCoinEffect(jt->getID(), jt->getAttribute(), jt->getCurrentPosition().x + 15.0f, jt->getCurrentPosition().y + 32.0f);
					DeleteIndexCoin(jt);
					SoundManager::PlaySound("Coin");
					++CoinCount;
				}
			}
			for (auto jt = GoombaAIList.begin(); jt != GoombaAIList.end(); ++jt) {
				if (sf::FloatRect GoombaAICollide = getGlobalHitbox(jt->GetHitboxMain(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(GoombaAICollide, BrickLoop)) {
					jt->DeathBehaviour(SCORE_100);
					if (jt->IsCanDeath()) DeleteGoombaAIIndex(jt);
				}
			}
			for (auto jt = BroAIList.begin(); jt != BroAIList.end(); ++jt) {
				if (sf::FloatRect BroAICollide = getGlobalHitbox(jt->getHitbox(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(BroAICollide, BrickLoop)) {
					jt->DeathBehaviour(SCORE_200);
					SoundManager::PlaySound("Kick2");
					DeleteBroAIIndex(jt);
				}
			}
			MultiBrickCoin(BrickLoop.position.x, BrickLoop.position.y + 16.f, i);
			if (i->getAtt() == NORMAL && PowerState == 0) {
				i->setState(true);
				i->setUpDown(false);
				i->setStateCount(0.f);
				SoundManager::PlaySound("Bump");
			}
			else if (i->getAtt() == NORMAL && PowerState > 0) {
				SoundManager::PlaySound("Break");
				AddBrickParticle(i->getID(), i->getCurrentPosition().x, i->getCurrentPosition().y);
				DeleteBrickIndex(i);
				MFCPP::setIndexTilemapCollision(i->getCurrentPosition().x, i->getCurrentPosition().y, false);
				Score += 50;
			}
		}
	}
}
void DeleteBrickIndex(const plf::colony<MFCPP::Brick>::colony_iterator<false>& it) {
	it->setDestroyed(true);
	BrickDeleteGate = true;
}
void DeleteBrick(const float x, const float y) {;
	for (auto it = Bricks.begin(); it != Bricks.end(); ++it) {
		if (it->getCurrentPosition().x == x && it->getCurrentPosition().y == y) {
			DeleteBrickIndex(it);
		}
	}
}
void DeleteAllBrick() {
	Bricks.clear();
}
void BrickCleanup() {
	if (!BrickDeleteGate) return;
	auto it = Bricks.begin();
	while (it != Bricks.end()) {
		if (!it->isDestroyed()) ++it;
		else it = Bricks.erase(it);
	}
	BrickDeleteGate = false;
}