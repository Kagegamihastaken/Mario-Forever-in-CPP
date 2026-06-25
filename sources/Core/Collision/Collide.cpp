#include "Core/Collision/Collide.hpp"

#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Tilemap.hpp"
#include "Core/Utility.hpp"
#include "Core/Scene/GameScene.hpp"

void setHitbox(sf::FloatRect& hitbox, const sf::FloatRect& Sethitbox) {
	hitbox = Sethitbox;
}
sf::FloatRect HitboxExtend(const sf::FloatRect& hitbox, const float val) {
	return {{hitbox.position.x - val, hitbox.position.y - val}, {hitbox.size.x + val * 2.f, hitbox.size.y + val * 2.f}};
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
sf::FloatRect extendHitbox(const sf::FloatRect& hitbox, float val) {
	return {hitbox.position - sf::Vector2f(val, val), hitbox.size + sf::Vector2f(val * 2.f, val * 2.f)};
}
sf::FloatRect extendHitboxX(const sf::FloatRect& hitbox, float val) {
	return {hitbox.position - sf::Vector2f(val, 0.f), hitbox.size + sf::Vector2f(val * 2.f, 0.f)};
}
bool isCollide(const sf::FloatRect& hitbox, const sf::FloatRect& other) {
	return static_cast<bool>(hitbox.findIntersection(other));
}
bool isCollide(const sf::FloatRect& hitbox, const sf::Sprite& sprite, const sf::FloatRect& other) {
	return static_cast<bool>(getGlobalHitbox(hitbox, sprite).findIntersection(other));
}
// This only meant for slope
bool GetRelativeTilemapSlopeUp(const float CurrPosXCollide, const float CurrPosYCollide) {
	const std::pair<float, float> floorY = MFCPP::Tilemap::getIndexTilemapFloorY(CurrPosXCollide, CurrPosYCollide);
	const bool condition = floorY.first < floorY.second;
	if (MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide + MFCPP::Tilemap::getTileSize() * (condition ? 1.f : -1.f), CurrPosYCollide) == 3) {
		const std::pair<float, float> direction = MFCPP::Tilemap::getIndexTilemapFloorY(CurrPosXCollide + MFCPP::Tilemap::getTileSize() * (condition ? 1.f : -1.f), CurrPosYCollide);
		if ((direction.first - direction.second < 0) == condition) return true;
	}
	if (MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide + MFCPP::Tilemap::getTileSize() * (condition ? 1.f : -1.f), CurrPosYCollide - MFCPP::Tilemap::getTileSize()) == 3) {
		const std::pair<float, float> direction = MFCPP::Tilemap::getIndexTilemapFloorY(CurrPosXCollide + MFCPP::Tilemap::getTileSize() * (condition ? 1.f : -1.f), CurrPosYCollide - MFCPP::Tilemap::getTileSize());
		if ((direction.first - direction.second < 0) == condition) return true;
	}
	return false;
}
bool GetRelativeTilemapSlopeDown(const float CurrPosXCollide, const float CurrPosYCollide) {
	const auto [fst, snd] = MFCPP::Tilemap::getIndexTilemapFloorY(CurrPosXCollide, CurrPosYCollide);
	const bool condition = fst < snd;
	if (MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide - MFCPP::Tilemap::getTileSize() * (condition ? 1.f : -1.f), CurrPosYCollide) == 3) {
		return true;
	}
	if (MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide - MFCPP::Tilemap::getTileSize() * (condition ? 1.f : -1.f), CurrPosYCollide + MFCPP::Tilemap::getTileSize()) == 3) {
		return true;
	}
	return false;
}
bool GetRelativeTilemapSlopeLeft(const float CurrPosXCollide, const float CurrPosYCollide) {
	// Check if left has slope AND right DOESN'T have slope
	if (MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide - MFCPP::Tilemap::getTileSize(), CurrPosYCollide) == 3 && MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide + MFCPP::Tilemap::getTileSize(), CurrPosYCollide) != 3) {
		if (const auto [fst, snd] = MFCPP::Tilemap::getIndexTilemapFloorY(CurrPosXCollide - MFCPP::Tilemap::getTileSize(), CurrPosYCollide); fst - snd < 0) return true;
	}
	return false;
}
bool GetRelativeTilemapSlopeRight(const float CurrPosXCollide, const float CurrPosYCollide) {
	// Check if right has slope AND left DOESN'T have slope
	if (MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide + MFCPP::Tilemap::getTileSize(), CurrPosYCollide) == 3 && MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide - MFCPP::Tilemap::getTileSize(), CurrPosYCollide) != 3) {
		if (const auto [fst, snd] = MFCPP::Tilemap::getIndexTilemapFloorY(CurrPosXCollide + MFCPP::Tilemap::getTileSize(), CurrPosYCollide); fst - snd >= 0) return true;
	}
	return false;
}
bool GetRelativeTilemapSlopeRightReverse(const float CurrPosXCollide, const float CurrPosYCollide) {
	if (MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide - MFCPP::Tilemap::getTileSize(), CurrPosYCollide) == 3 && MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide + MFCPP::Tilemap::getTileSize(), CurrPosYCollide) != 3) {
		if (const auto [fst, snd] = MFCPP::Tilemap::getIndexTilemapFloorY(CurrPosXCollide - MFCPP::Tilemap::getTileSize(), CurrPosYCollide); fst - snd >= 0) return true;
	}
	return false;
}
bool GetRelativeTilemapSlopeLeftReverse(const float CurrPosXCollide, const float CurrPosYCollide) {
	if (MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide + MFCPP::Tilemap::getTileSize(), CurrPosYCollide) == 3 && MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide - MFCPP::Tilemap::getTileSize(), CurrPosYCollide) != 3) {
		if (const auto [fst, snd] = MFCPP::Tilemap::getIndexTilemapFloorY(CurrPosXCollide + MFCPP::Tilemap::getTileSize(), CurrPosYCollide); fst - snd < 0) return true;
	}
	return false;
}
bool GetRelativeTilemapSlopeBetween(const float CurrPosXCollide, const float CurrPosYCollide) {
	if (MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide + MFCPP::Tilemap::getTileSize(), CurrPosYCollide) == 3) return true;
	if (MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide - MFCPP::Tilemap::getTileSize(), CurrPosYCollide) == 3) return true;
	return false;
}
bool GetInbetweenTilemapSlope(const float CurrPosXCollide, const float CurrPosYCollide) {
	if (MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide + MFCPP::Tilemap::getTileSize(), CurrPosYCollide) == 3 && MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide - MFCPP::Tilemap::getTileSize(), CurrPosYCollide) == 3) {
		auto ObjLeft = MFCPP::Tilemap::getIndexTilemapFloorY(CurrPosXCollide - MFCPP::Tilemap::getTileSize(), CurrPosYCollide);
		auto ObjRight = MFCPP::Tilemap::getIndexTilemapFloorY(CurrPosXCollide + MFCPP::Tilemap::getTileSize(), CurrPosYCollide);
		if (ObjLeft.first > ObjLeft.second && ObjRight.first < ObjRight.second)
			return true;
	}
	return false;
}
std::pair<bool, bool> isAccurateCollideSide(const MFCPP::CollisionObject& CollideObj, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, const unsigned ID) {
	//bool isCollideLeftBool = false, isCollideRightBool = false;
	std::pair<bool, bool> isCollideSide; //First:Left, Second:Right
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	for (int i = static_cast<int>(std::floor(hitbox_intersect.position.x / 32.f)); i <= static_cast<int>(std::floor((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f)); ++i) {
		for (int j = static_cast<int>(std::floor(hitbox_intersect.position.y / 32.f)); j <= static_cast<int>(std::floor((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f)); ++j) {
			if (!MFCPP::Tilemap::getIndexTilemapCollision(i, j) || MFCPP::Tilemap::getIndexTilemapID(i, j) != ID) continue;
			// Check if collide
			const sf::FloatRect hitbox_loop = getGlobalHitbox(sf::FloatRect({0.f, (MFCPP::Tilemap::getIndexTilemapID(i, j) != 3 ? MFCPP::Tilemap::getIndexTilemapFloorY(i, j).first : 0.f)}, {MFCPP::Tilemap::getTileSize(),  (MFCPP::Tilemap::getIndexTilemapID(i, j) != 3 ? MFCPP::Tilemap::getIndexTilemapFloorY(i, j).second - MFCPP::Tilemap::getIndexTilemapFloorY(i, j).first : MFCPP::Tilemap::getTileSize())}), sf::Vector2f(static_cast<float>(i) * MFCPP::Tilemap::getTileSize(),  static_cast<float>(j) * MFCPP::Tilemap::getTileSize()), {0,0});
			if (isCollide(hitbox_intersect, hitbox_loop)) {
				if (hitbox_intersect.position.x >= hitbox_loop.position.x + 16.0f || hitbox_intersect.position.x + hitbox_intersect.size.x >= hitbox_loop.position.x + 16.0f) {
					if (!GetRelativeTilemapSlopeBetween(i * MFCPP::Tilemap::getTileSize(), j * MFCPP::Tilemap::getTileSize())) isCollideSide.first = true;
					else if (GetInbetweenTilemapSlope(i * MFCPP::Tilemap::getTileSize(), j * MFCPP::Tilemap::getTileSize())) isCollideSide.first = true;
					else if (GetRelativeTilemapSlopeLeft(i * MFCPP::Tilemap::getTileSize(), j * MFCPP::Tilemap::getTileSize())) isCollideSide.first = true;
					else if (GetRelativeTilemapSlopeLeftReverse(i * MFCPP::Tilemap::getTileSize(), j * MFCPP::Tilemap::getTileSize())) isCollideSide.first = true;
					else if (GetRelativeTilemapSlopeRightReverse(i * MFCPP::Tilemap::getTileSize(), j * MFCPP::Tilemap::getTileSize())) isCollideSide.first = true;
				}
				if (hitbox_intersect.position.x + hitbox_intersect.size.x < hitbox_loop.position.x + 16.0f || hitbox_intersect.position.x < hitbox_loop.position.x + 16.0f) {
					if (!GetRelativeTilemapSlopeBetween(i * MFCPP::Tilemap::getTileSize(), j * MFCPP::Tilemap::getTileSize())) isCollideSide.second = true;
					else if (GetInbetweenTilemapSlope(i * MFCPP::Tilemap::getTileSize(), j * MFCPP::Tilemap::getTileSize())) isCollideSide.second = true;
					else if (GetRelativeTilemapSlopeRight(i * MFCPP::Tilemap::getTileSize(), j * MFCPP::Tilemap::getTileSize())) isCollideSide.second = true;
					else if (GetRelativeTilemapSlopeRightReverse(i * MFCPP::Tilemap::getTileSize(), j * MFCPP::Tilemap::getTileSize())) isCollideSide.second = true;
					else if (GetRelativeTilemapSlopeLeftReverse(i * MFCPP::Tilemap::getTileSize(), j * MFCPP::Tilemap::getTileSize())) isCollideSide.second = true;
				}

				if (CurrPosXCollide != i * MFCPP::Tilemap::getTileSize() || CurrPosYCollide != j * MFCPP::Tilemap::getTileSize()) {
					CurrPosXCollide = i * MFCPP::Tilemap::getTileSize();
					CurrPosYCollide = j * MFCPP::Tilemap::getTileSize();
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
	for (int i = static_cast<int>(std::floor(hitbox_intersect.position.x / 32.f)); i <= static_cast<int>(std::floor((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f)); ++i) {
		for (int j = static_cast<int>(std::floor(hitbox_intersect.position.y / 32.f)); j <= static_cast<int>(std::floor((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f)); ++j) {
			if (!MFCPP::Tilemap::getIndexCollectableMapCollision(i, j) || MFCPP::Tilemap::getIndexCollectableMapID(i, j) != ID) continue;
			if (const sf::FloatRect hitbox_loop = getGlobalHitbox(rect, sf::Vector2f(static_cast<float>(i) * MFCPP::Tilemap::getTileSize(),  static_cast<float>(j) * MFCPP::Tilemap::getTileSize()), {0,0}); isCollide(hitbox_intersect, hitbox_loop)) result.emplace_back(static_cast<float>(i) * MFCPP::Tilemap::getTileSize(), static_cast<float>(j) * MFCPP::Tilemap::getTileSize());
		}
	}
	return std::move(result);
}
//Y
bool isAccurateCollideBotStopEdge(const MFCPP::CollisionObject& CollideObj, const bool direction) {
	//if (NoAdd) return false;
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), {CollideObj.GetPosition().x + CollideObj.GetLeftHitbox().size.x * (direction ? -1.f : 1.f), CollideObj.GetPosition().y}, CollideObj.GetOrigin());
	const int SizeX = static_cast<int>(std::floor((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f));
	const int SizeY = static_cast<int>(std::floor((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f));
	for (int i = static_cast<int>(std::floor(hitbox_intersect.position.x / 32.f)); i <= SizeX; ++i) {
		for (int j = static_cast<int>(std::floor(hitbox_intersect.position.y / 32.f)); j <= SizeY; ++j) {
			if (const sf::FloatRect hitbox_loop = getGlobalHitbox(sf::FloatRect({0.f, 0.f}, {MFCPP::Tilemap::getTileSize(), MFCPP::Tilemap::getTileSize()}), sf::Vector2f(static_cast<float>(i) * MFCPP::Tilemap::getTileSize(),  static_cast<float>(j) * MFCPP::Tilemap::getTileSize()), {0,0}); isCollide(hitbox_intersect, hitbox_loop)) {
				if (Utility::f_abs(hitbox_intersect.position.x - CollideObj.GetLeftHitbox().size.x * (direction ? -1.f : 1.f) - hitbox_loop.position.x) > MFCPP::Tilemap::getTileSize()) continue;
				//if (f_abs(CollideObj.GetPosition().x - hitbox_loop.position.x) > MFCPP::getTileSize()) continue;

				if (MFCPP::Tilemap::getIndexTilemapCollision(hitbox_loop.position.x, hitbox_loop.position.y))
					return true;
				if (MFCPP::Tilemap::getIndexTilemapID(hitbox_loop.position.x - CollideObj.GetLeftHitbox().size.x * (direction ? -1.f : 1.f), hitbox_loop.position.y) == 3) {
					if (MFCPP::Tilemap::getIndexTilemapCollision(hitbox_loop.position.x, hitbox_loop.position.y + MFCPP::Tilemap::getTileSize()))
						return true;
				}
				else {
					if (MFCPP::Tilemap::getIndexTilemapCollision(hitbox_loop.position.x, hitbox_loop.position.y + MFCPP::Tilemap::getTileSize()) && MFCPP::Tilemap::getIndexTilemapID(hitbox_loop.position.x, hitbox_loop.position.y + MFCPP::Tilemap::getTileSize()) == 3)
						return true;
				}
			}
		}
	}
	return false;
}
bool isAccurateCollideBot(const MFCPP::CollisionObject& CollideObj, const float offset, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, const unsigned ID) {
	//if (NoAdd) return false;
	bool isCollideBotBool = false;
	bool willCollide = false;
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	const int SizeX = static_cast<int>(std::floor((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f));
	const int SizeY = static_cast<int>(std::floor((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f));
	for (int i = SizeX; i >= static_cast<int>(std::floor(hitbox_intersect.position.x / 32.f)); --i) {
		for (int j = static_cast<int>(std::floor(hitbox_intersect.position.y / 32.f)); j <= SizeY; ++j) {
			if (!MFCPP::Tilemap::getIndexTilemapCollision(i, j) || MFCPP::Tilemap::getIndexTilemapID(i, j) != ID) continue;
			if (const sf::FloatRect hitbox_loop = getGlobalHitbox(sf::FloatRect({0.f, (MFCPP::Tilemap::getIndexTilemapID(i, j) != 3 ? MFCPP::Tilemap::getIndexTilemapFloorY(i, j).first : 0.f)}, {MFCPP::Tilemap::getTileSize(), (MFCPP::Tilemap::getIndexTilemapID(i, j) != 3 ? MFCPP::Tilemap::getIndexTilemapFloorY(i, j).second - MFCPP::Tilemap::getIndexTilemapFloorY(i, j).first : MFCPP::Tilemap::getTileSize())}), sf::Vector2f(static_cast<float>(i) * MFCPP::Tilemap::getTileSize(),  static_cast<float>(j) * MFCPP::Tilemap::getTileSize()), {0,0}); isCollide(hitbox_intersect, hitbox_loop)) {
				if ((CollideObj.GetPosition().x - i * MFCPP::Tilemap::getTileSize()) / MFCPP::Tilemap::getTileSize() < 0.f && willCollide) continue;
				willCollide = true;
				//if (hitbox_intersect.position.x < hitbox_loop.position.x || hitbox_intersect.position.x > hitbox_loop.position.x + hitbox_loop.size.x) continue;
				if ((hitbox_intersect.position.y + hitbox_intersect.size.y < hitbox_loop.position.y + hitbox_loop.size.y / 2.f && MFCPP::Tilemap::getIndexTilemapID(i, j) != 3) || MFCPP::Tilemap::getIndexTilemapID(i, j) == 3) {
					if (MFCPP::Tilemap::getIndexTilemapID(i * MFCPP::Tilemap::getTileSize(), j * MFCPP::Tilemap::getTileSize()) == 3) {
						bool BlockDirection = MFCPP::Tilemap::getIndexTilemapFloorY(i * MFCPP::Tilemap::getTileSize(), j * MFCPP::Tilemap::getTileSize()).first < MFCPP::Tilemap::getIndexTilemapFloorY(i * MFCPP::Tilemap::getTileSize(), j * MFCPP::Tilemap::getTileSize()).second;
						if (MFCPP::Tilemap::getIndexTilemapID(i * MFCPP::Tilemap::getTileSize() - (BlockDirection ? MFCPP::Tilemap::getTileSize() : -MFCPP::Tilemap::getTileSize()), j * MFCPP::Tilemap::getTileSize()) != 3 && MFCPP::Tilemap::getIndexTilemapCollision(i * MFCPP::Tilemap::getTileSize() - (BlockDirection ? MFCPP::Tilemap::getTileSize() : -MFCPP::Tilemap::getTileSize()), j * MFCPP::Tilemap::getTileSize())) {
							const float floorY = GetCurrFloorY(CollideObj.GetPosition(), i * MFCPP::Tilemap::getTileSize(), j * MFCPP::Tilemap::getTileSize());
							if (CollideObj.GetPosition().y < j * MFCPP::Tilemap::getTileSize() + floorY - offset) continue;
						}
					}
					isCollideBotBool = true;
					if (CurrPosYCollide <= j * MFCPP::Tilemap::getTileSize() || MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide, CurrPosYCollide) != 3) {
						CurrPosYCollide = j * MFCPP::Tilemap::getTileSize();
						CurrPosXCollide = i * MFCPP::Tilemap::getTileSize();
					}
				}
			}
		}
	}
	return isCollideBotBool;
}
bool isAccurateCollideTop(const MFCPP::CollisionObject& CollideObj, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, const unsigned ID) {
	if (NoAdd) return false;
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	for (int i = static_cast<int>(std::floor(hitbox_intersect.position.x / 32.f)); i <= static_cast<int>(std::floor((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f)); ++i) {
		for (int j = static_cast<int>(std::floor(hitbox_intersect.position.y / 32.f)); j <= static_cast<int>(std::floor((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f)); ++j) {
			if (!MFCPP::Tilemap::getIndexTilemapCollision(i, j) || MFCPP::Tilemap::getIndexTilemapID(i, j) != ID) continue;
			//if (SaveList.size() > 0) hitbox_loop.position.y = SaveList[i].second;
			sf::FloatRect hitbox_loop = getGlobalHitbox(sf::FloatRect({0.f, (MFCPP::Tilemap::getIndexTilemapID(i, j) != 3 ? MFCPP::Tilemap::getIndexTilemapFloorY(i, j).first : 0.f)}, {MFCPP::Tilemap::getTileSize(), (MFCPP::Tilemap::getIndexTilemapID(i, j) != 3 ? MFCPP::Tilemap::getIndexTilemapFloorY(i, j).second - MFCPP::Tilemap::getIndexTilemapFloorY(i, j).first : MFCPP::Tilemap::getTileSize())}), sf::Vector2f(static_cast<float>(i) * MFCPP::Tilemap::getTileSize(),  static_cast<float>(j) * MFCPP::Tilemap::getTileSize()), {0,0});
			if (isCollide(hitbox_intersect, hitbox_loop)) {
				if (hitbox_intersect.position.y >= hitbox_loop.position.y + hitbox_loop.size.y / 2.f && hitbox_intersect.position.y <= hitbox_loop.position.y + hitbox_loop.size.y) {
					CurrPosYCollide = j * MFCPP::Tilemap::getTileSize();
					CurrPosXCollide = i * MFCPP::Tilemap::getTileSize();
					NoAdd = true;
					return true;
				}
			}
		}
	}
	return false;
}
bool isCollideMain(const MFCPP::CollisionObject& CollideObj, const unsigned ID) {
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	for (int i = static_cast<int>(std::floor(hitbox_intersect.position.x / 32.f)); i <= static_cast<int>(std::floor((hitbox_intersect.position.x + hitbox_intersect.size.x) / 32.f)); ++i) {
		for (int j = static_cast<int>(std::floor(hitbox_intersect.position.y / 32.f)); j <= static_cast<int>(std::floor((hitbox_intersect.position.y + hitbox_intersect.size.y) / 32.f)); ++j) {
			if (!MFCPP::Tilemap::getIndexTilemapCollision(i, j) || MFCPP::Tilemap::getIndexTilemapID(i, j) != ID) continue;
			if (const sf::FloatRect hitbox_loop = getGlobalHitbox(sf::FloatRect({0.f, 0.f}, {MFCPP::Tilemap::getTileSize(), MFCPP::Tilemap::getTileSize()}), sf::Vector2f(static_cast<float>(i) * MFCPP::Tilemap::getTileSize(),  static_cast<float>(j) * MFCPP::Tilemap::getTileSize()), {0,0}); isCollide(hitbox_intersect, hitbox_loop)) {
				return true;
			}
		}
	}
	return false;
}
/*
 * TEMPLATE COLLISION
 */
std::pair<bool, bool> CheckCollisionSide(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd, const unsigned ID) {
	std::pair<bool, bool> CollideLoop = isAccurateCollideSide(CollideObj, CurrX, CurrY, NoAdd, ID);
	return CollideLoop;
}
bool CheckCollisionBot(const MFCPP::CollisionObject& CollideObj, const float offset, float& CurrX, float& CurrY, bool& NoAdd, const unsigned ID) {
	const bool CollideLoop = isAccurateCollideBot(CollideObj, offset, CurrX, CurrY, NoAdd, ID);
	return CollideLoop;
}
bool CheckCollisionTop(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd, const unsigned ID) {
	const bool CollideLoop = isAccurateCollideTop(CollideObj, CurrX, CurrY, NoAdd, ID);
	return CollideLoop;
}
bool CheckCollisionMain(const MFCPP::CollisionObject& CollideObj, const unsigned ID) {
	const bool CollideLoop = isCollideMain(CollideObj, ID);
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
bool QuickCheckOnlyObstacleMainCollision(const MFCPP::CollisionObject& CollideObj) {
	bool out = false;
	out |= CheckCollisionMain(CollideObj, 0);
	out |= CheckCollisionMain(CollideObj, 3);
	return out;
}
bool QuickCheckOnlyObstacleBotCollision(const MFCPP::CollisionObject& CollideObj, const float offset, float& CurrX, float& CurrY, bool& NoAdd) {
	float CurrPosYCollide = 0.f, CurrPosXCollide = 0.f;
	bool out = false;
	out |= CheckCollisionBot(CollideObj, offset, CurrPosXCollide, CurrPosYCollide, NoAdd, 0);
	out |= CheckCollisionBot(CollideObj, offset, CurrPosXCollide, CurrPosYCollide, NoAdd, 3);
	if (out) CurrY = CurrPosYCollide, CurrX = CurrPosXCollide;
	return out;
}
bool QuickCheckOnlyHittableBotCollision(const MFCPP::CollisionObject& CollideObj, const float offset, float& CurrX, float& CurrY, bool& NoAdd) {
	float CurrPosYCollide = 0.f, CurrPosXCollide = 0.f;
	bool out = false;
	out |= CheckCollisionBot(CollideObj, offset, CurrPosXCollide, CurrPosYCollide, NoAdd, 1);
	out |= CheckCollisionBot(CollideObj, offset, CurrPosXCollide, CurrPosYCollide, NoAdd, 2);
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
bool QuickCheckOnlyHittableMainCollision(const MFCPP::CollisionObject& CollideObj) {
	bool out = false;
	out |= CheckCollisionMain(CollideObj, 1);
	out |= CheckCollisionMain(CollideObj, 2);
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
bool QuickCheckBotCollision(const MFCPP::CollisionObject& CollideObj, const float offset, float& CurrX, float& CurrY) {
	float CurrPosYCollide, CurrPosXCollide;
	bool NoAdd = false;

	//const MFCPP::CollisionObject obj({CollideObj.GetPosition().x, CollideObj.GetPosition().y + 1.f}, CollideObj.GetOrigin(), CollideObj.GetLeftHitbox());

	bool out = false;
	out |= QuickCheckOnlyHittableBotCollision(CollideObj, offset, CurrPosXCollide, CurrPosYCollide, NoAdd);
	out |= QuickCheckOnlyObstacleBotCollision(CollideObj, offset, CurrPosXCollide, CurrPosYCollide, NoAdd);
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
bool QuickCheckMainCollision(const MFCPP::CollisionObject& CollideObj) {
	bool out = false;
	out |= QuickCheckOnlyHittableMainCollision(CollideObj);
	out |= QuickCheckOnlyObstacleMainCollision(CollideObj);
	return out;
}

float GetCurrFloorY(const sf::Vector2f& pos, const float CurrX, const float CurrY) {
	float t;
	const bool SlopeRelaUp = GetRelativeTilemapSlopeUp(CurrX, CurrY);
	const bool SlopeRelaDown = GetRelativeTilemapSlopeDown(CurrX, CurrY);
	const std::pair<float, float> CurrFloorY = MFCPP::Tilemap::getIndexTilemapFloorY(CurrX, CurrY);
	const bool CurrCondition = CurrFloorY.first - CurrFloorY.second < 0;
	if (!SlopeRelaUp)
		if (!SlopeRelaDown)
			t = (std::max(std::min(pos.x, CurrX + MFCPP::Tilemap::getTileSize()), CurrX) - CurrX) / MFCPP::Tilemap::getTileSize();
		else
			if (CurrCondition)
				t = (std::min(pos.x, CurrX + MFCPP::Tilemap::getTileSize()) - CurrX) / MFCPP::Tilemap::getTileSize();
			else
				t = (std::max(pos.x, CurrX) - CurrX) / MFCPP::Tilemap::getTileSize();
	else
		if (!SlopeRelaDown)
			if (CurrCondition)
				t = (std::max(pos.x, CurrX) - CurrX) / MFCPP::Tilemap::getTileSize();
			else
				t = (std::min(pos.x, CurrX + MFCPP::Tilemap::getTileSize()) - CurrX) / MFCPP::Tilemap::getTileSize();
		else
			t = (pos.x - CurrX) / MFCPP::Tilemap::getTileSize();
	const float floorY = (MFCPP::Tilemap::getIndexTilemapID(CurrX, CurrY) == 3 ? t * MFCPP::Tilemap::getIndexTilemapFloorY(CurrX, CurrY).first + (1 - t) * MFCPP::Tilemap::getIndexTilemapFloorY(CurrX, CurrY).second : MFCPP::Tilemap::getIndexTilemapFloorY(CurrX, CurrY).first);
	return floorY;
}
bool PlatformYCollisionEdge(const MFCPP::CollisionObject& CollideObj, const float Yvelo, const bool direction) {
	const sf::FloatRect CollideHitbox = getGlobalHitbox(CollideObj.GetLeftHitbox(), sf::Vector2f(CollideObj.GetPosition().x + CollideObj.GetLeftHitbox().size.x * (direction ? -1.f : 1.f), CollideObj.GetPosition().y), CollideObj.GetOrigin());
	for (auto &it : GameScene::movingBlockManager.getPlatformList()) {
		if (const sf::FloatRect PlatformHitbox = getGlobalHitbox(it.getHitbox(), it.getPreviousPosition(), it.getOrigin()); isCollide(CollideHitbox, PlatformHitbox)) {
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
	for (auto &it : GameScene::movingBlockManager.getPlatformList()) {
		if (const sf::FloatRect PlatformHitbox = getGlobalHitbox(it.getHitbox(), it.getPreviousPosition(), it.getOrigin()); isCollide(CollideHitbox, PlatformHitbox)) {
			//Only Bottom allowed to collide
			if (CollideHitbox.position.y + CollideHitbox.size.y >= PlatformHitbox.position.y && CollideHitbox.position.y + CollideHitbox.size.y <= PlatformHitbox.position.y + PlatformHitbox.size.y) {
				if (Yvelo >= 0.f) {
					if (ActivatePlatform) {
						it.activate();
					}
					YPosOut = it.getPosition().y;
					return true;
				}
			}
		}
	}
	return false;
}
bool PlatformXCollision(const MFCPP::CollisionObject& CollideObj, float& XDistance, const float Yvelo) {
	const sf::FloatRect CollideHitbox = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	for (auto &it : GameScene::movingBlockManager.getPlatformList()) {
		if (it.canCollision()) continue;

		if (const sf::FloatRect PlatformHitbox = getGlobalHitbox(it.getHitbox(), it.getPreviousPosition(), it.getOrigin()); isCollide(CollideHitbox, PlatformHitbox)) {
			//Only Bottom allowed to collide
			if (Yvelo >= 0.f) {
				if (CollideHitbox.position.y + CollideHitbox.size.y >= PlatformHitbox.position.y && CollideHitbox.position.y + CollideHitbox.size.y <= PlatformHitbox.position.y + PlatformHitbox.size.y) {
					XDistance = it.getPosition().x - it.getPreviousPosition().x;
					return true;
				}
			}
		}
	}
	return false;
}