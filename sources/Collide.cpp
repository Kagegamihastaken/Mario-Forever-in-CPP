#include <SFML/Graphics.hpp>
#include <vector>

#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Object/Mario.hpp"
#include "Class/CollisionObjectClass.hpp"

static constexpr float DEFAULT_OBJECT_WIDTH = 32.0f;

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

// Mario && GoombaAI only
std::pair<bool, bool> isAccurateCollideSidet(const MFCPP::CollisionObject& CollideObj, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, const int first, const int last, const float distance) {
	if (NoAdd) return { false, false };
	bool isCollideLeftBool = false, isCollideRightBool = false;
	const sf::FloatRect hitbox_intersect_left = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	const sf::FloatRect hitbox_intersect_right = getGlobalHitbox(CollideObj.GetRightHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	for (int i = first; i <= last; ++i) {
		if (f_abs(OL[i].second.y - CollideObj.GetPosition().y) > distance) continue;
		sf::FloatRect hitbox_loop = getGlobalHitbox(OL[i].first, OL[i].second, {0,0});
		// Check if collide
		if (isCollide(hitbox_intersect_left, hitbox_loop)) {
			isCollideLeftBool = true;
			if (CurrPosXCollide != hitbox_loop.position.x || CurrPosYCollide != hitbox_loop.position.y) {
				CurrPosXCollide = hitbox_loop.position.x;
				CurrPosYCollide = hitbox_loop.position.y;
				NoAdd = true;
			}
			break;
		}
		if (isCollide(hitbox_intersect_right, hitbox_loop)) {
			isCollideRightBool = true;
			if (CurrPosXCollide != hitbox_loop.position.x || CurrPosYCollide != hitbox_loop.position.y) {
				CurrPosXCollide = hitbox_loop.position.x;
				CurrPosYCollide = hitbox_loop.position.y;
				NoAdd = true;
			}
			break;
		}
	}
	return { isCollideLeftBool, isCollideRightBool };
}
std::vector<sf::Vector2f> isAccurateCollideMaint(const MFCPP::CollisionObject& CollideObj, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, const int& first, const int& last, const float& distance) {
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	std::vector<sf::Vector2f> result;
	if (!OL.empty()) result.reserve(OL.size() / 4);
	for (int i = first; i <= last; ++i) {
		if (f_abs(OL[i].second.y - CollideObj.GetPosition().y) > distance) continue;
		// Check if collide
		if (sf::FloatRect hitbox_loop = getGlobalHitbox(OL[i].first, OL[i].second, {0,0}); isCollide(hitbox_intersect, hitbox_loop)) result.emplace_back(OL[i].second.x, OL[i].second.y);
	}
	return result;
}
//Y
bool isAccurateCollideBott(const MFCPP::CollisionObject& CollideObj, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OLVert, float& CurrPosYCollide, bool& NoAdd, const int first, const int last, const float distance) {
	if (NoAdd) return false;
	bool isCollideBotBool = false;
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	for (int i = first; i <= last; ++i) {
		if (f_abs(OLVert[i].second.x - CollideObj.GetPosition().x) > distance) continue;
		if (const sf::FloatRect hitbox_loop = getGlobalHitbox(OLVert[i].first, OLVert[i].second, {0,0}); isCollide(hitbox_intersect, hitbox_loop)) {
			isCollideBotBool = true;
			CurrPosYCollide = hitbox_loop.position.y;
			NoAdd = true;
			break;
		}
	}
	return isCollideBotBool;
}
std::vector<std::pair<float, float>> isCollideTopDetailed(const MFCPP::CollisionObject& CollideObj, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OLVert, const int first, const int last, const float distance) {
	std::vector<std::pair<float, float>> result;
	if (!OLVert.empty()) result.reserve(OLVert.size() / 4);
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	for (int i = first; i <= last; ++i) {
		if (f_abs(OLVert[i].second.x - CollideObj.GetPosition().x) > distance) continue;
		if (const sf::FloatRect hitbox_loop = getGlobalHitbox(OLVert[i].first, OLVert[i].second, {0,0}); isCollide(hitbox_intersect, hitbox_loop)) result.emplace_back( hitbox_loop.position.x, hitbox_loop.position.y );
	}
	return result;
}
bool isAccurateCollideTopt(const MFCPP::CollisionObject& CollideObj, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OLVert, float& CurrPosYCollide, bool& NoAdd, const int first, const int last, const float distance) {
	if (NoAdd) return false;
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(CollideObj.GetLeftHitbox(), CollideObj.GetPosition(), CollideObj.GetOrigin());
	bool isCollideTopBool = false;
	for (int i = first; i <= last; ++i) {
		sf::FloatRect hitbox_loop = getGlobalHitbox(OLVert[i].first, OLVert[i].second, {0,0});
		if (f_abs(OLVert[i].second.x - CollideObj.GetPosition().x) > distance) continue;
		//if (SaveList.size() > 0) hitbox_loop.position.y = SaveList[i].second;
		if (isCollide(hitbox_intersect, hitbox_loop)) {
			isCollideTopBool = true;
			CurrPosYCollide = hitbox_loop.position.y;
			break;
		}
	}
	return isCollideTopBool;
}
int find_max_inx(const sf::Vector2f& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL) {
	int l = 0, r = static_cast<int>(OL.size()) - 1, out = 0;
	while (l <= r) {
		if (const int m = l + (r - l) / 2; OL[m].second.x - DEFAULT_OBJECT_WIDTH <= object.x) {
			l = m + 1;
			out = m;
		}
		else r = m - 1;
	}
	return out;
}
int find_max_inx_dist(const sf::Vector2f& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, const float dist) {
	int l = 0, r = static_cast<int>(OL.size()) - 1, out = 0;
	while (l <= r) {
		if (const int m = l + (r - l) / 2; OL[m].second.x - dist <= object.x) {
			l = m + 1;
			out = m;
		}
		else r = m - 1;
	}
	return out;
}
int find_min_inx(const sf::Vector2f& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL) {
	int l = 0, r = static_cast<int>(OL.size()) - 1, out = 0;
	while (l <= r) {
		if (const int m = l + (r - l) / 2; OL[m].second.x + DEFAULT_OBJECT_WIDTH >= object.x) {
			r = m - 1;
			out = m;
		}
		else l = m + 1;
	}
	return out;
}
int find_min_inx_dist(const sf::Vector2f& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, const float dist) {
	int l = 0, r = static_cast<int>(OL.size()) - 1, out = 0;
	while (l <= r) {
		if (const int m = l + (r - l) / 2; OL[m].second.x + dist >= object.x) {
			r = m - 1;
			out = m;
		}
		else l = m + 1;
	}
	return out;
}
int find_max_iny(const sf::Vector2f& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL) {
	int l = 0, r = static_cast<int>(OL.size()) - 1, out = 0;
	while (l <= r) {
		if (const int m = l + (r - l) / 2; OL[m].second.y - DEFAULT_OBJECT_WIDTH <= object.y) {
			l = m + 1;
			out = m;
		}
		else r = m - 1;
	}
	return out;
}
int find_max_iny_dist(const sf::Vector2f& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, const float dist) {
	int l = 0, r = static_cast<int>(OL.size()) - 1, out = 0;
	while (l <= r) {
		if (const int m = l + (r - l) / 2; OL[m].second.y - dist <= object.y) {
			l = m + 1;
			out = m;
		}
		else r = m - 1;
	}
	return out;
}
int find_min_iny(const sf::Vector2f& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL) {
	int l = 0, r = static_cast<int>(OL.size()) - 1, out = 0;
	while (l <= r) {
		if (const int m = l + (r - l) / 2; OL[m].second.y + DEFAULT_OBJECT_WIDTH >= object.y) {
			r = m - 1;
			out = m;
		}
		else l = m + 1;
	}
	return out;
}
int find_min_iny_dist(const sf::Vector2f& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, const float dist) {
	int l = 0, r = static_cast<int>(OL.size()) - 1, out = 0;
	while (l <= r) {
		if (const int m = l + (r - l) / 2; OL[m].second.y + dist >= object.y) {
			r = m - 1;
			out = m;
		}
		else l = m + 1;
	}
	return out;
}