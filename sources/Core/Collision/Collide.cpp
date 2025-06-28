#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"

#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Tilemap.hpp"

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
std::pair<bool, bool> isAccurateCollideSide(const MFCPP::CollisionObject& CollideObj, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, const unsigned ID) {
	bool isCollideLeftBool = false, isCollideRightBool = false;
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	for (int i = static_cast<int>(hitbox_intersect.position.x / 32.f); i <= static_cast<int>((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f); ++i) {
		for (int j = static_cast<int>(hitbox_intersect.position.y / 32.f); j <= static_cast<int>((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f); ++j) {
			if (!MFCPP::getIndexTilemapCollision(i, j) || MFCPP::getIndexTilemapID(i, j) != ID) continue;
			// Check if collide
			const sf::FloatRect hitbox_loop = getGlobalHitbox(sf::FloatRect({0.f, 0.f}, MFCPP::getTileSize()), sf::Vector2f(static_cast<float>(i) * MFCPP::getTileSize().x,  static_cast<float>(j) * MFCPP::getTileSize().y), {0,0});
			if (isCollide(hitbox_intersect, hitbox_loop)) {
				if (hitbox_intersect.position.x >= hitbox_loop.position.x + 16.0f || hitbox_intersect.position.x + hitbox_intersect.size.x >= hitbox_loop.position.x + 16.0f) isCollideLeftBool = true;
				if (hitbox_intersect.position.x + hitbox_intersect.size.x < hitbox_loop.position.x + 16.0f || hitbox_intersect.position.x < hitbox_loop.position.x + 16.0f) isCollideRightBool = true;

				if (CurrPosXCollide != hitbox_loop.position.x || CurrPosYCollide != hitbox_loop.position.y) {
					CurrPosXCollide = hitbox_loop.position.x;
					CurrPosYCollide = hitbox_loop.position.y;
					NoAdd = true;
				}
				break;
			}
		}
	}
	return { isCollideLeftBool, isCollideRightBool };
}
std::vector<sf::Vector2f> isAccurateCollideMainCollectable(const MFCPP::CollisionObject& CollideObj, const unsigned ID, const sf::FloatRect& rect) {
	std::vector<sf::Vector2f> result;
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	for (int i = static_cast<int>(hitbox_intersect.position.x / 32.f); i <= static_cast<int>((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f); ++i) {
		for (int j = static_cast<int>(hitbox_intersect.position.y / 32.f); j <= static_cast<int>((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f); ++j) {
			if (!MFCPP::getIndexCollectableMapCollision(i, j) || MFCPP::getIndexCollectableMapID(i, j) != ID) continue;
			const sf::FloatRect hitbox_loop = getGlobalHitbox(rect, sf::Vector2f(static_cast<float>(i) * MFCPP::getTileSize().x,  static_cast<float>(j) * MFCPP::getTileSize().y), {0,0});
			if (isCollide(hitbox_intersect, hitbox_loop)) result.emplace_back(static_cast<float>(i) * MFCPP::getTileSize().x, static_cast<float>(j) * MFCPP::getTileSize().y);
		}
	}
	return result;
}
//Y
bool isAccurateCollideBot(const MFCPP::CollisionObject& CollideObj, float& CurrPosYCollide, bool& NoAdd, const unsigned ID) {
	if (NoAdd) return false;
	bool isCollideBotBool = false;
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	for (int i = static_cast<int>(hitbox_intersect.position.x / 32.f); i <= static_cast<int>((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f); ++i) {
		for (int j = static_cast<int>(hitbox_intersect.position.y / 32.f); j <= static_cast<int>((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f); ++j) {
			if (!MFCPP::getIndexTilemapCollision(i, j) || MFCPP::getIndexTilemapID(i, j) != ID) continue;
			if (const sf::FloatRect hitbox_loop = getGlobalHitbox(sf::FloatRect({0.f, 0.f}, MFCPP::getTileSize()), sf::Vector2f(static_cast<float>(i) * MFCPP::getTileSize().x,  static_cast<float>(j) * MFCPP::getTileSize().y), {0,0}); isCollide(hitbox_intersect, hitbox_loop)) {
				if (hitbox_intersect.position.y + hitbox_intersect.size.y < hitbox_loop.position.y + 16.0f) {
					isCollideBotBool = true;
					CurrPosYCollide = hitbox_loop.position.y;
					NoAdd = true;
					break;
				}
			}
		}
	}
	return isCollideBotBool;
}
std::vector<std::pair<float, float>> isCollideTopDetailed(const MFCPP::CollisionObject& CollideObj, float& CurrPosYCollide, bool& NoAdd, const unsigned ID) {
	std::vector<std::pair<float, float>> result;
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	for (int i = static_cast<int>(hitbox_intersect.position.x / 32.f); i <= static_cast<int>((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f); ++i) {
		for (int j = static_cast<int>(hitbox_intersect.position.y / 32.f); j <= static_cast<int>((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f); ++j) {
			if (!MFCPP::getIndexTilemapCollision(i, j) || MFCPP::getIndexTilemapID(i, j) != ID) continue;
			const sf::FloatRect hitbox_loop = getGlobalHitbox(sf::FloatRect({0.f, 0.f}, MFCPP::getTileSize()), sf::Vector2f(static_cast<float>(i) * MFCPP::getTileSize().x,  static_cast<float>(j) * MFCPP::getTileSize().y), {0,0});
			if (isCollide(hitbox_intersect, hitbox_loop)) {
				if (!NoAdd) {
					CurrPosYCollide = hitbox_loop.position.y;
					NoAdd = true;
				}
				result.emplace_back( hitbox_loop.position.x, hitbox_loop.position.y );
			}
		}
	}
	return result;
}
bool isAccurateCollideTop(const MFCPP::CollisionObject& CollideObj,  float& CurrPosYCollide, bool& NoAdd, const unsigned ID) {
	if (NoAdd) return false;
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	bool isCollideTopBool = false;
	for (int i = static_cast<int>(hitbox_intersect.position.x / 32.f); i <= static_cast<int>((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f); ++i) {
		for (int j = static_cast<int>(hitbox_intersect.position.y / 32.f); j <= static_cast<int>((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f); ++j) {
			if (!MFCPP::getIndexTilemapCollision(i, j) || MFCPP::getIndexTilemapID(i, j) != ID) continue;
			const sf::FloatRect hitbox_loop = getGlobalHitbox(sf::FloatRect({0.f, 0.f}, MFCPP::getTileSize()), sf::Vector2f(static_cast<float>(i) * MFCPP::getTileSize().x,  static_cast<float>(j) * MFCPP::getTileSize().y), {0,0});
			//if (SaveList.size() > 0) hitbox_loop.position.y = SaveList[i].second;
			if (isCollide(hitbox_intersect, hitbox_loop)) {
				if (hitbox_intersect.position.y >= hitbox_loop.position.y + 16.0f && hitbox_intersect.position.y <= hitbox_loop.position.y + 32.0f) {
					isCollideTopBool = true;
					CurrPosYCollide = hitbox_loop.position.y;
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
std::pair<bool, bool> CheckCollisionRight(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd, const unsigned ID) {
	std::pair<bool, bool> CollideLoop = isAccurateCollideSide(CollideObj, CurrX, CurrY, NoAdd, ID);
	return CollideLoop;
}
std::pair<bool, bool> CheckCollisionLeft(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd, const unsigned ID) {
	const std::pair<bool, bool> CollideLoop = isAccurateCollideSide(CollideObj, CurrX, CurrY, NoAdd, ID);
	return CollideLoop;
}
bool CheckCollisionBot(const MFCPP::CollisionObject& CollideObj, float& CurrY, bool& NoAdd, const unsigned ID) {
	const bool CollideLoop = isAccurateCollideBot(CollideObj, CurrY, NoAdd, ID);
	return CollideLoop;
}
bool CheckCollisionTop(const MFCPP::CollisionObject& CollideObj, float& CurrY, bool& NoAdd, const unsigned ID) {
	const bool CollideLoop = isAccurateCollideTop(CollideObj, CurrY, NoAdd, ID);
	return CollideLoop;
}
std::vector<std::pair<float, float>> CheckCollisionTopDetailed(const MFCPP::CollisionObject& CollideObj, float& CurrY, bool& NoAdd, const unsigned ID) {
	const std::vector<std::pair<float, float>> CollideLoop = isCollideTopDetailed(CollideObj, CurrY, NoAdd, ID);
	return CollideLoop;
}

std::pair<bool, bool> QuickCheckOnlyObstaclesSideCollision(const MFCPP::CollisionObject& CollideObj, const bool direction, float& CurrX, float& CurrY, bool& NoAdd) {
	std::pair out{false, false};
	std::pair<bool, bool> CollideLoop;
	float CurrPosXCollide = -1.f, CurrPosYCollide = -1.f;
	if (!direction) {
		CollideLoop = CheckCollisionRight(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 0);
		out.first |= CollideLoop.first, out.second |= CollideLoop.second;
	}
	else {
		CollideLoop = CheckCollisionLeft(CollideObj,  CurrPosXCollide, CurrPosYCollide, NoAdd, 0);
		out.first |= CollideLoop.first, out.second |= CollideLoop.second;
	}
	if (out.first || out.second) CurrX = CurrPosXCollide, CurrY = CurrPosYCollide;
	return out;
}
std::pair<bool, bool> QuickCheckOnlyHittableSideCollision(const MFCPP::CollisionObject& CollideObj, const bool direction, float& CurrX, float& CurrY, bool& NoAdd) {
	std::pair out{false, false};
	std::pair<bool, bool> CollideLoop;
	float CurrPosXCollide = -1.f, CurrPosYCollide = -1.f;
	if (!direction) {
		CollideLoop = CheckCollisionRight(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 1);
		out.first |= CollideLoop.first, out.second |= CollideLoop.second;
		CollideLoop = CheckCollisionRight(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 2);
		out.first |= CollideLoop.first, out.second |= CollideLoop.second;
	}
	else {
		CollideLoop = CheckCollisionLeft(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 1);
		out.first |= CollideLoop.first, out.second |= CollideLoop.second;
		CollideLoop = CheckCollisionLeft(CollideObj, CurrPosXCollide, CurrPosYCollide, NoAdd, 2);
		out.first |= CollideLoop.first, out.second |= CollideLoop.second;
	}
	if (out.first || out.second) CurrX = CurrPosXCollide, CurrY = CurrPosYCollide;
	return out;
}
bool QuickCheckOnlyObstacleTopCollision(const MFCPP::CollisionObject& CollideObj, float& CurrY, bool& NoAdd) {
	float CurrPosYCollide;
	bool out = false;
	out |= CheckCollisionTop(CollideObj, CurrPosYCollide, NoAdd, 0);
	if (out) CurrY = CurrPosYCollide;
	return out;
}
bool QuickCheckOnlyObstacleBotCollision(const MFCPP::CollisionObject& CollideObj, float& CurrY, bool& NoAdd) {
	float CurrPosYCollide;
	bool out = false;
	out |= CheckCollisionBot(CollideObj, CurrPosYCollide, NoAdd, 0);
	if (out) CurrY = CurrPosYCollide;
	return out;
}
bool QuickCheckOnlyHittableBotCollision(const MFCPP::CollisionObject& CollideObj, float& CurrY, bool& NoAdd) {
	float CurrPosYCollide;
	bool out = false;
	out |= CheckCollisionBot(CollideObj, CurrPosYCollide, NoAdd, 1);
	out |= CheckCollisionBot(CollideObj, CurrPosYCollide, NoAdd, 2);
	if (out) CurrY = CurrPosYCollide;
	return out;
}
bool QuickCheckOnlyHittableTopCollision(const MFCPP::CollisionObject& CollideObj, float& CurrY, bool& NoAdd) {
	float CurrPosYCollide;
	bool out = false;
	out |= CheckCollisionTop(CollideObj, CurrPosYCollide, NoAdd, 1);
	out |= CheckCollisionTop(CollideObj, CurrPosYCollide, NoAdd, 2);
	if (out) CurrY = CurrPosYCollide;
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
bool QuickCheckBotCollision(const MFCPP::CollisionObject& CollideObj, float& CurrY) {
	float CurrPosYCollide;
	bool NoAdd = false;

	//const MFCPP::CollisionObject obj({CollideObj.GetPosition().x, CollideObj.GetPosition().y + 1.f}, CollideObj.GetOrigin(), CollideObj.GetLeftHitbox());

	bool out = false;
	out |= QuickCheckOnlyHittableBotCollision(CollideObj,CurrPosYCollide, NoAdd);
	out |= QuickCheckOnlyObstacleBotCollision(CollideObj,CurrPosYCollide, NoAdd);
	CurrY = CurrPosYCollide;
	return out;
}
bool QuickCheckTopCollision(const MFCPP::CollisionObject& CollideObj, float& CurrY) {
	float CurrPosYCollide;
	bool NoAdd = false;
	bool out = false;
	out |= QuickCheckOnlyHittableTopCollision(CollideObj, CurrPosYCollide, NoAdd);
	out |= QuickCheckOnlyObstacleTopCollision(CollideObj,  CurrPosYCollide, NoAdd);
	CurrY = CurrPosYCollide;
	return out;
}