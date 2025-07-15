#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"

#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Tilemap.hpp"
#include "Object/Platform.hpp"

void setHitbox(sf::FloatRect& hitbox, const sf::FloatRect& Sethitbox) {
	hitbox = Sethitbox;
}
sf::FloatRect getGlobalHitbox(const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin) {
	return {{ pos.x + hitbox.position.x - origin.x, pos.y + hitbox.position.y - origin.y }, { hitbox.size.x, hitbox.size.y }};
}
sf::FloatRect getGlobalHitbox(const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Sprite& sprite) {
	return getGlobalHitbox(hitbox, pos, sprite.getOrigin());//
}
sf::FloatRect getGlobalHitbox(const sf::FloatRect& hitbox, const sf::Sprite& sprite) {
	return sprite.getTransform().transformRect(hitbox);
}
bool isCollide(const sf::FloatRect& hitbox, const sf::FloatRect& other) {
	return static_cast<bool>(hitbox.findIntersection(other));
}
bool isCollide(const sf::FloatRect& hitbox, const sf::Sprite& sprite, const sf::FloatRect& other) {
	return static_cast<bool>(getGlobalHitbox(hitbox, sprite).findIntersection(other));
}
// This only meant for slope
bool GetRelativeTilemapSlopeUp(const float CurrPosXCollide, const float CurrPosYCollide) {
	const std::pair<float, float> floorY = MFCPP::getIndexTilemapFloorY(CurrPosXCollide, CurrPosYCollide);
	const bool condition = floorY.first - floorY.second < 0;
	if (MFCPP::getIndexTilemapID(CurrPosXCollide + MFCPP::getTileSize() * (condition ? 1.f : -1.f), CurrPosYCollide) == 3) {
		const std::pair<float, float> direction = MFCPP::getIndexTilemapFloorY(CurrPosXCollide + MFCPP::getTileSize() * (condition ? 1.f : -1.f), CurrPosYCollide);
		if ((direction.first - direction.second < 0) == condition) return true;
	}
	if (MFCPP::getIndexTilemapID(CurrPosXCollide + MFCPP::getTileSize() * (condition ? 1.f : -1.f), CurrPosYCollide - MFCPP::getTileSize()) == 3) {
		const std::pair<float, float> direction = MFCPP::getIndexTilemapFloorY(CurrPosXCollide + MFCPP::getTileSize() * (condition ? 1.f : -1.f), CurrPosYCollide - MFCPP::getTileSize());
		if ((direction.first - direction.second < 0) == condition) return true;
	}
	return false;
}
bool GetRelativeTilemapSlopeDown(const float CurrPosXCollide, const float CurrPosYCollide) {
	const auto [fst, snd] = MFCPP::getIndexTilemapFloorY(CurrPosXCollide, CurrPosYCollide);
	const bool condition = fst - snd < 0;
	if (MFCPP::getIndexTilemapID(CurrPosXCollide - MFCPP::getTileSize() * (condition ? 1.f : -1.f), CurrPosYCollide) == 3) return true;
	if (MFCPP::getIndexTilemapID(CurrPosXCollide - MFCPP::getTileSize() * (condition ? 1.f : -1.f), CurrPosYCollide + MFCPP::getTileSize()) == 3) return true;
	return false;
}
bool GetRelativeTilemapSlopeLeft(const float CurrPosXCollide, const float CurrPosYCollide) {
	if (MFCPP::getIndexTilemapID(CurrPosXCollide - MFCPP::getTileSize(), CurrPosYCollide) == 3) {
		if (const auto [fst, snd] = MFCPP::getIndexTilemapFloorY(CurrPosXCollide - MFCPP::getTileSize(), CurrPosYCollide); fst - snd < 0) return true;
	}
	return false;
}
bool GetRelativeTilemapSlopeRight(const float CurrPosXCollide, const float CurrPosYCollide) {
	if (MFCPP::getIndexTilemapID(CurrPosXCollide + MFCPP::getTileSize(), CurrPosYCollide) == 3) {
		if (const auto [fst, snd] = MFCPP::getIndexTilemapFloorY(CurrPosXCollide + MFCPP::getTileSize(), CurrPosYCollide); fst - snd >= 0) return true;
	}
	return false;
}
bool GetRelativeTilemapSlopeBetween(const float CurrPosXCollide, const float CurrPosYCollide) {
	if (MFCPP::getIndexTilemapID(CurrPosXCollide + MFCPP::getTileSize(), CurrPosYCollide) == 3) return true;
	if (MFCPP::getIndexTilemapID(CurrPosXCollide - MFCPP::getTileSize(), CurrPosYCollide) == 3) return true;
	return false;
}
std::pair<bool, bool> isAccurateCollideSide(const MFCPP::CollisionObject& CollideObj, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, const unsigned ID) {
	//bool isCollideLeftBool = false, isCollideRightBool = false;
	std::pair<bool, bool> isCollideSide; //First:Left, Second:Right
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	for (int i = static_cast<int>(hitbox_intersect.position.x / 32.f); i <= static_cast<int>((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f); ++i) {
		for (int j = static_cast<int>(hitbox_intersect.position.y / 32.f); j <= static_cast<int>((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f); ++j) {
			if (!MFCPP::getIndexTilemapCollision(i, j) || MFCPP::getIndexTilemapID(i, j) != ID) continue;
			// Check if collide
			const sf::FloatRect hitbox_loop = getGlobalHitbox(sf::FloatRect({0.f, 0.f}, {MFCPP::getTileSize(), MFCPP::getTileSize()}), sf::Vector2f(static_cast<float>(i) * MFCPP::getTileSize(),  static_cast<float>(j) * MFCPP::getTileSize()), {0,0});
			if (isCollide(hitbox_intersect, hitbox_loop)) {
				if (hitbox_intersect.position.x >= hitbox_loop.position.x + 16.0f || hitbox_intersect.position.x + hitbox_intersect.size.x >= hitbox_loop.position.x + 16.0f) {
					if (GetRelativeTilemapSlopeLeft(hitbox_loop.position.x, hitbox_loop.position.y)) isCollideSide.first = true;
					if (!GetRelativeTilemapSlopeBetween(hitbox_loop.position.x, hitbox_loop.position.y)) isCollideSide.first = true;
				}
				if (hitbox_intersect.position.x + hitbox_intersect.size.x < hitbox_loop.position.x + 16.0f || hitbox_intersect.position.x < hitbox_loop.position.x + 16.0f) {
					if (GetRelativeTilemapSlopeRight(hitbox_loop.position.x, hitbox_loop.position.y)) isCollideSide.second = true;
					if (!GetRelativeTilemapSlopeBetween(hitbox_loop.position.x, hitbox_loop.position.y)) isCollideSide.second = true;
				}

				if (CurrPosXCollide != hitbox_loop.position.x || CurrPosYCollide != hitbox_loop.position.y) {
					CurrPosXCollide = hitbox_loop.position.x;
					CurrPosYCollide = hitbox_loop.position.y;
					NoAdd = true;
				}
				break;
			}
		}
	}
	return isCollideSide;
}
std::vector<sf::Vector2f> isAccurateCollideMainCollectable(const MFCPP::CollisionObject& CollideObj, const unsigned ID, const sf::FloatRect& rect) {
	std::vector<sf::Vector2f> result;
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	for (int i = static_cast<int>(hitbox_intersect.position.x / 32.f); i <= static_cast<int>((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f); ++i) {
		for (int j = static_cast<int>(hitbox_intersect.position.y / 32.f); j <= static_cast<int>((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f); ++j) {
			if (!MFCPP::getIndexCollectableMapCollision(i, j) || MFCPP::getIndexCollectableMapID(i, j) != ID) continue;
			if (const sf::FloatRect hitbox_loop = getGlobalHitbox(rect, sf::Vector2f(static_cast<float>(i) * MFCPP::getTileSize(),  static_cast<float>(j) * MFCPP::getTileSize()), {0,0}); isCollide(hitbox_intersect, hitbox_loop)) result.emplace_back(static_cast<float>(i) * MFCPP::getTileSize(), static_cast<float>(j) * MFCPP::getTileSize());
		}
	}
	return std::move(result);
}
//Y
bool isAccurateCollideBotStopEdge(const MFCPP::CollisionObject& CollideObj, const bool direction) {
	//if (NoAdd) return false;
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), {CollideObj.GetPosition().x + CollideObj.GetLeftHitbox().size.x * (direction ? -1.f : 1.f), CollideObj.GetPosition().y}, CollideObj.GetOrigin());
	const int SizeX = static_cast<int>((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f);
	const int SizeY = static_cast<int>((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f);
	for (int i = static_cast<int>(hitbox_intersect.position.x / 32.f); i <= SizeX; ++i) {
		for (int j = static_cast<int>(hitbox_intersect.position.y / 32.f); j <= SizeY; ++j) {
			if (const sf::FloatRect hitbox_loop = getGlobalHitbox(sf::FloatRect({0.f, 0.f}, {MFCPP::getTileSize(), MFCPP::getTileSize()}), sf::Vector2f(static_cast<float>(i) * MFCPP::getTileSize(),  static_cast<float>(j) * MFCPP::getTileSize()), {0,0}); isCollide(hitbox_intersect, hitbox_loop)) {
				if (f_abs(hitbox_intersect.position.x - CollideObj.GetLeftHitbox().size.x * (direction ? -1.f : 1.f) - hitbox_loop.position.x) > MFCPP::getTileSize()) continue;
				//if (f_abs(CollideObj.GetPosition().x - hitbox_loop.position.x) > MFCPP::getTileSize()) continue;

				if (MFCPP::getIndexTilemapCollision(hitbox_loop.position.x, hitbox_loop.position.y) || MFCPP::getIndexTilemapCollision(hitbox_loop.position.x, hitbox_loop.position.y + MFCPP::getTileSize()))
					return true;
			}
		}
	}
	return false;
}
bool isAccurateCollideBot(const MFCPP::CollisionObject& CollideObj, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, const unsigned ID) {
	//if (NoAdd) return false;
	bool isCollideBotBool = false;
	bool willCollide = false;
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	const int SizeX = static_cast<int>((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f);
	const int SizeY = static_cast<int>((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f);
	for (int i = static_cast<int>(hitbox_intersect.position.x / 32.f); i <= SizeX; ++i) {
		for (int j = static_cast<int>(hitbox_intersect.position.y / 32.f); j <= SizeY; ++j) {
			if (!MFCPP::getIndexTilemapCollision(i, j) || MFCPP::getIndexTilemapID(i, j) != ID) continue;
			if (const sf::FloatRect hitbox_loop = getGlobalHitbox(sf::FloatRect({0.f, 0.f}, {MFCPP::getTileSize(), MFCPP::getTileSize()}), sf::Vector2f(static_cast<float>(i) * MFCPP::getTileSize(),  static_cast<float>(j) * MFCPP::getTileSize()), {0,0}); isCollide(hitbox_intersect, hitbox_loop)) {
				if ((CollideObj.GetPosition().x - hitbox_loop.position.x) / MFCPP::getTileSize() < 0.f && !(i == SizeX && i == SizeY) && willCollide) continue;
				willCollide = true;
				//if (hitbox_intersect.position.x < hitbox_loop.position.x || hitbox_intersect.position.x > hitbox_loop.position.x + hitbox_loop.size.x) continue;
				if (hitbox_intersect.position.y + hitbox_intersect.size.y < hitbox_loop.position.y + 16.0f && MFCPP::getIndexTilemapID(i, j) != 3 || MFCPP::getIndexTilemapID(i, j) == 3) {
					isCollideBotBool = true;
					if (CurrPosYCollide <= hitbox_loop.position.y || MFCPP::getIndexTilemapID(CurrPosXCollide, CurrPosYCollide) != 3) {
						CurrPosYCollide = hitbox_loop.position.y;
						CurrPosXCollide = hitbox_loop.position.x;
					}
				}
			}
		}
	}
	return isCollideBotBool;
}
std::vector<std::pair<float, float>> isCollideTopDetailed(const MFCPP::CollisionObject& CollideObj, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, unsigned ID) {
	std::vector<std::pair<float, float>> result;
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	for (int i = static_cast<int>(hitbox_intersect.position.x / 32.f); i <= static_cast<int>((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f); ++i) {
		for (int j = static_cast<int>(hitbox_intersect.position.y / 32.f); j <= static_cast<int>((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f); ++j) {
			if (!MFCPP::getIndexTilemapCollision(i, j) || MFCPP::getIndexTilemapID(i, j) != ID) continue;
			if (const sf::FloatRect hitbox_loop = getGlobalHitbox(sf::FloatRect({0.f, 0.f}, {MFCPP::getTileSize(), MFCPP::getTileSize()}), sf::Vector2f(static_cast<float>(i) * MFCPP::getTileSize(),  static_cast<float>(j) * MFCPP::getTileSize()), {0,0}); isCollide(hitbox_intersect, hitbox_loop)) {
				if (hitbox_intersect.position.y >= hitbox_loop.position.y + 16.0f && hitbox_intersect.position.y <= hitbox_loop.position.y + 32.0f) {
					if (!NoAdd) {
						CurrPosYCollide = hitbox_loop.position.y;
						CurrPosXCollide = hitbox_loop.position.x;
						NoAdd = true;
					}
					result.emplace_back( hitbox_loop.position.x, hitbox_loop.position.y );
				}
			}
		}
	}
	return std::move(result);
}
bool isAccurateCollideTop(const MFCPP::CollisionObject& CollideObj, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, const unsigned ID) {
	if (NoAdd) return false;
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	bool isCollideTopBool = false;
	for (int i = static_cast<int>(hitbox_intersect.position.x / 32.f); i <= static_cast<int>((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f); ++i) {
		for (int j = static_cast<int>(hitbox_intersect.position.y / 32.f); j <= static_cast<int>((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f); ++j) {
			if (!MFCPP::getIndexTilemapCollision(i, j) || MFCPP::getIndexTilemapID(i, j) != ID) continue;
			//if (SaveList.size() > 0) hitbox_loop.position.y = SaveList[i].second;
			if (const sf::FloatRect hitbox_loop = getGlobalHitbox(sf::FloatRect({0.f, 0.f}, {MFCPP::getTileSize(), MFCPP::getTileSize()}), sf::Vector2f(static_cast<float>(i) * MFCPP::getTileSize(),  static_cast<float>(j) * MFCPP::getTileSize()), {0,0}); isCollide(hitbox_intersect, hitbox_loop)) {
				if (hitbox_intersect.position.y >= hitbox_loop.position.y + 16.0f && hitbox_intersect.position.y <= hitbox_loop.position.y + 32.0f) {
					isCollideTopBool = true;
					CurrPosYCollide = hitbox_loop.position.y;
					CurrPosXCollide = hitbox_loop.position.x;
					NoAdd = true;
					break;
				}
			}
		}
	}
	return isCollideTopBool;
}
/*
 * TEMPLATE COLLISION
 */
std::pair<bool, bool> CheckCollisionSide(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd, const unsigned ID) {
	std::pair<bool, bool> CollideLoop = isAccurateCollideSide(CollideObj, CurrX, CurrY, NoAdd, ID);
	return CollideLoop;
}
bool CheckCollisionBot(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd, const unsigned ID) {
	const bool CollideLoop = isAccurateCollideBot(CollideObj, CurrX, CurrY, NoAdd, ID);
	return CollideLoop;
}
bool CheckCollisionTop(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd, const unsigned ID) {
	const bool CollideLoop = isAccurateCollideTop(CollideObj, CurrX, CurrY, NoAdd, ID);
	return CollideLoop;
}
std::vector<std::pair<float, float>> CheckCollisionTopDetailed(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd, const unsigned ID) {
	const std::vector<std::pair<float, float>> CollideLoop = isCollideTopDetailed(CollideObj, CurrX, CurrY, NoAdd, ID);
	return CollideLoop;
}

std::pair<bool, bool> QuickCheckOnlyObstaclesSideCollision(const MFCPP::CollisionObject& CollideObj, const bool direction, float& CurrX, float& CurrY, bool& NoAdd) {
	std::pair out{false, false};
	float CurrPosXCollide = -1.f, CurrPosYCollide = -1.f;
	std::pair<bool, bool> CollideLoop = CheckCollisionSide(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 0);
	out.first |= CollideLoop.first, out.second |= CollideLoop.second;
	//CollideLoop = CheckCollisionSide(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 3);
	//out.first |= CollideLoop.first, out.second |= CollideLoop.second;
	if (out.first || out.second) CurrX = CurrPosXCollide, CurrY = CurrPosYCollide;
	return out;
}
std::pair<bool, bool> QuickCheckOnlyHittableSideCollision(const MFCPP::CollisionObject& CollideObj, const bool direction, float& CurrX, float& CurrY, bool& NoAdd) {
	std::pair out{false, false};
	float CurrPosXCollide = -1.f, CurrPosYCollide = -1.f;
	std::pair<bool, bool> CollideLoop = CheckCollisionSide(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 1);
	out.first |= CollideLoop.first, out.second |= CollideLoop.second;
	CollideLoop = CheckCollisionSide(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 2);
	out.first |= CollideLoop.first, out.second |= CollideLoop.second;
	if (out.first || out.second) CurrX = CurrPosXCollide, CurrY = CurrPosYCollide;
	return out;
}
bool QuickCheckOnlyObstacleTopCollision(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd) {
	float CurrPosYCollide, CurrPosXCollide;
	bool out = false;
	out |= CheckCollisionTop(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 0);
	out |= CheckCollisionTop(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 3);
	if (out) CurrY = CurrPosYCollide, CurrX = CurrPosXCollide;
	return out;
}
bool QuickCheckOnlyObstacleBotCollision(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd) {
	float CurrPosYCollide = 0.f, CurrPosXCollide = 0.f;
	bool out = false;
	out |= CheckCollisionBot(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 0);
	out |= CheckCollisionBot(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 3);
	if (out) CurrY = CurrPosYCollide, CurrX = CurrPosXCollide;
	return out;
}
bool QuickCheckOnlyHittableBotCollision(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd) {
	float CurrPosYCollide = 0.f, CurrPosXCollide = 0.f;
	bool out = false;
	out |= CheckCollisionBot(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 1);
	out |= CheckCollisionBot(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 2);
	if (out) CurrY = CurrPosYCollide, CurrX = CurrPosXCollide;
	return out;
}
bool QuickCheckOnlyHittableTopCollision(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd) {
	float CurrPosYCollide, CurrPosXCollide;
	bool out = false;
	out |= CheckCollisionTop(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 1);
	out |= CheckCollisionTop(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 2);
	if (out) CurrX = CurrPosXCollide, CurrY = CurrPosYCollide;
	return out;
}

// TEMPLATE
std::pair<bool, bool> QuickCheckSideCollision(const MFCPP::CollisionObject& CollideObj, const bool direction, float& CurrX, float& CurrY) {
	std::pair out{false, false};
	float CurrPosXCollide, CurrPosYCollide;
	bool NoAdd = false;
	std::pair<bool, bool> CollideLoop = QuickCheckOnlyObstaclesSideCollision(CollideObj, direction, CurrPosXCollide, CurrPosYCollide, NoAdd);
	out.first |= CollideLoop.first, out.second |= CollideLoop.second;
	CollideLoop = QuickCheckOnlyHittableSideCollision(CollideObj, direction, CurrPosXCollide, CurrPosYCollide, NoAdd);
	out.first |= CollideLoop.first, out.second |= CollideLoop.second;
	CurrX = CurrPosXCollide, CurrY = CurrPosYCollide;
	return out;
}
bool QuickCheckBotCollision(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY) {
	float CurrPosYCollide, CurrPosXCollide;
	bool NoAdd = false;

	//const MFCPP::CollisionObject obj({CollideObj.GetPosition().x, CollideObj.GetPosition().y + 1.f}, CollideObj.GetOrigin(), CollideObj.GetLeftHitbox());

	bool out = false;
	out |= QuickCheckOnlyHittableBotCollision(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd);
	out |= QuickCheckOnlyObstacleBotCollision(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd);
	CurrX = CurrPosXCollide, CurrY = CurrPosYCollide;
	return out;
}
bool QuickCheckTopCollision(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY) {
	float CurrPosYCollide, CurrPosXCollide;
	bool NoAdd = false;
	bool out = false;
	out |= QuickCheckOnlyHittableTopCollision(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd);
	out |= QuickCheckOnlyObstacleTopCollision(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd);
	CurrX = CurrPosXCollide, CurrY = CurrPosYCollide;
	return out;
}

float GetCurrFloorY(const sf::Vector2f& pos, const float CurrX, const float CurrY) {
	float t;
	const bool SlopeRelaUp = GetRelativeTilemapSlopeUp(CurrX, CurrY);
	const bool SlopeRelaDown = GetRelativeTilemapSlopeDown(CurrX, CurrY);
	const std::pair<float, float> CurrFloorY = MFCPP::getIndexTilemapFloorY(CurrX, CurrY);
	const bool CurrCondition = CurrFloorY.first - CurrFloorY.second <= 0;
	if (!SlopeRelaUp)
		if (!SlopeRelaDown)
			t = (std::max(std::min(pos.x, CurrX + MFCPP::getTileSize()), CurrX) - CurrX) / MFCPP::getTileSize();
		else
			if (CurrCondition)
				t = (std::min(pos.x, CurrX + MFCPP::getTileSize()) - CurrX) / MFCPP::getTileSize();
			else
				t = (std::max(pos.x, CurrX) - CurrX) / MFCPP::getTileSize();
	else
		if (!SlopeRelaDown)
			if (CurrCondition)
				t = (std::max(pos.x, CurrX) - CurrX) / MFCPP::getTileSize();
			else
				t = (std::min(pos.x, CurrX + MFCPP::getTileSize()) - CurrX) / MFCPP::getTileSize();
		else
			t = (pos.x - CurrX) / MFCPP::getTileSize();
	const float floorY = t * MFCPP::getIndexTilemapFloorY(CurrX, CurrY).first + (1 - t) * MFCPP::getIndexTilemapFloorY(CurrX, CurrY).second;
	return floorY;
}
bool PlatformYCollisionEdge(const MFCPP::CollisionObject& CollideObj, const float Yvelo, const bool direction) {
	const sf::FloatRect CollideHitbox = getGlobalHitbox(CollideObj.GetLeftHitbox(), sf::Vector2f(CollideObj.GetPosition().x + CollideObj.GetLeftHitbox().size.x * (direction ? -1.f : 1.f), CollideObj.GetPosition().y), CollideObj.GetOrigin());
	for (auto it = PlatformList.begin(); it != PlatformList.end(); ++it) {
		if (const sf::FloatRect PlatformHitbox = getGlobalHitbox(it->getHitbox(), it->getPreviousPosition(), it->getOrigin()); isCollide(CollideHitbox, PlatformHitbox)) {
			//Only Bottom allowed to collide
			if (CollideHitbox.position.y + CollideHitbox.size.y >= PlatformHitbox.position.y && CollideHitbox.position.y + CollideHitbox.size.y <= PlatformHitbox.position.y + PlatformHitbox.size.y) {
				if (Yvelo >= 0.f)
					return true;
			}
		}
	}
	return false;
}
bool PlatformYCollision(const MFCPP::CollisionObject& CollideObj, float& YPosOut, const float Yvelo, const bool ActivatePlatform) {
	const sf::FloatRect CollideHitbox = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	for (auto it = PlatformList.begin(); it != PlatformList.end(); ++it) {
		if (const sf::FloatRect PlatformHitbox = getGlobalHitbox(it->getHitbox(), it->getPreviousPosition(), it->getOrigin()); isCollide(CollideHitbox, PlatformHitbox)) {
			//Only Bottom allowed to collide
			if (CollideHitbox.position.y + CollideHitbox.size.y >= PlatformHitbox.position.y && CollideHitbox.position.y + CollideHitbox.size.y <= PlatformHitbox.position.y + PlatformHitbox.size.y) {
				if (Yvelo >= 0.f) {
					if (ActivatePlatform) {
						if (it->isFall()) it->setIsFall(true);
						if (it->isWait() && it->getWaitState() == 0) it->setWaitSate(1);
					}
					YPosOut = it->getCurrentPosition().y;
					return true;
					//MarioCurrentFalling = false;
					//Yvelo = 0.f;
					//break;
				}
			}
		}
	}
	return false;
}
bool PlatformXCollision(const MFCPP::CollisionObject& CollideObj, float& XDistance, const float Yvelo) {
	const sf::FloatRect CollideHitbox = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	for (auto it = PlatformList.begin(); it != PlatformList.end(); ++it) {
		if (const sf::FloatRect PlatformHitbox = getGlobalHitbox(it->getHitbox(), it->getPreviousPosition(), it->getOrigin()); isCollide(CollideHitbox, PlatformHitbox)) {
			//Only Bottom allowed to collide
			if (Yvelo >= 0.f) {
				if (CollideHitbox.position.y + CollideHitbox.size.y >= PlatformHitbox.position.y && CollideHitbox.position.y + CollideHitbox.size.y <= PlatformHitbox.position.y + PlatformHitbox.size.y) {
					XDistance = it->getCurrentPosition().x - it->getPreviousPosition().x;
					return true;
				}
			}
		}
	}
	return false;
}