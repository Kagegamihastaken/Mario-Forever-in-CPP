#include <SFML/Graphics.hpp>
#include <vector>

#include "Core/WindowFrame.hpp"
#include "Core/Collision/Collide.hpp"
#include "Object/Mario.hpp"
#include "Block/Obstacles.hpp"

void setHitbox(sf::FloatRect& hitbox, const sf::FloatRect& Sethitbox) {
	hitbox = Sethitbox;
}
sf::FloatRect getGlobalHitbox(const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin) {
	return sf::FloatRect({ pos.x + hitbox.position.x - origin.x, pos.y + hitbox.position.y - origin.y }, { hitbox.size.x, hitbox.size.y });
}
sf::FloatRect getGlobalHitbox(const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Sprite& sprite) {
	sf::Sprite obj(sprite.getTexture());
	obj.setPosition(pos);
	obj.setOrigin(sprite.getOrigin());
	return obj.getTransform().transformRect(hitbox);
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
std::pair<bool, bool> isAccurateCollideSidet(const MovableObject& object, const sf::Vector2f& pos, const std::vector<Obstacles>& OL, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, const int first, const int last, const float distance, const std::vector<std::pair<float, float>>& SaveList = {}) {
	bool isCollideLeftBool = false, isCollideRightBool = false;
	const sf::FloatRect hitbox_intersect_left = getGlobalHitbox(object.hitboxLeft, pos, object.property.getOrigin());
	const sf::FloatRect hitbox_intersect_right = getGlobalHitbox(object.hitboxRight, pos, object.property.getOrigin());
	for (int i = first; i <= last; ++i) {
		if (f_abs(OL[i].property.getPosition().y - object.property.getPosition().y) > distance) continue;
		sf::FloatRect hitbox_loop = getGlobalHitbox(OL[i].hitbox, OL[i].property);
		if (SaveList.size() > 0) {
			hitbox_loop.position.y = SaveList[i].second;
			hitbox_loop.position.x = SaveList[i].first;
		}
		// Check if collide
		if (isCollide(hitbox_intersect_left, hitbox_loop)) {
			isCollideLeftBool = true;
			if (CurrPosXCollide != hitbox_loop.position.x || CurrPosYCollide != hitbox_loop.position.y) {
				if (!NoAdd) {
					CurrPosXCollide = hitbox_loop.position.x;
					CurrPosYCollide = hitbox_loop.position.y;
					NoAdd = true;
				}
			}
			break;
		}
		if (isCollide(hitbox_intersect_right, hitbox_loop)) {
			isCollideRightBool = true;
			if (CurrPosXCollide != hitbox_loop.position.x || CurrPosYCollide != hitbox_loop.position.y) {
				if (!NoAdd) {
					CurrPosXCollide = hitbox_loop.position.x;
					CurrPosYCollide = hitbox_loop.position.y;
					NoAdd = true;
				}
			}
			break;
		}
	}
	return { isCollideLeftBool, isCollideRightBool };
}
//Y
bool isAccurateCollideBott(const MovableObject& object, const sf::Vector2f& pos, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OLVert, float& CurrPosYCollide, bool& NoAdd, const int first, const int last, const float distance) {
	bool isCollideBotBool = false;
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(object.hitboxBot, pos, object.property.getOrigin());
	for (int i = first; i <= last; ++i) {
		if (f_abs(OLVert[i].second.x - object.property.getPosition().x) > distance) continue;
		if (const sf::FloatRect hitbox_loop = getGlobalHitbox(OLVert[i].first, OLVert[i].second, {0,0}); isCollide(hitbox_intersect, hitbox_loop)) {
			isCollideBotBool = true;
			if (!NoAdd) {
				CurrPosYCollide = hitbox_loop.position.y;
				NoAdd = true;
				break;
			}
		}
	}
	return isCollideBotBool;
}
std::vector<std::pair<float, float>> isCollideTopDetailed(const MovableObject& object, const sf::Vector2f& pos, const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList = {}) {
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(object.hitboxTop, pos, object.property.getOrigin());
	std::vector<std::pair<float, float>> result;
	for (int i = 0; i < OL.size(); ++i) {
		sf::FloatRect hitbox_loop = getGlobalHitbox(OL[i].hitbox, OL[i].property);
		if (SaveList.size() > 0) hitbox_loop.position.y = SaveList[i].second;
		if (isCollide(hitbox_intersect, hitbox_loop)) result.push_back({ hitbox_loop.position.x, hitbox_loop.position.y });
	}
	return result;
}
bool isAccurateCollideTopt(const MovableObject& object, const sf::Vector2f& pos, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OLVert, float& CurrPosYCollide, bool& NoAdd, const int first, const int last, const float distance) {
	const sf::FloatRect hitbox_intersect = getGlobalHitbox(object.hitboxTop, pos, object.property.getOrigin());
	bool isCollideTopBool = false;
	for (int i = first; i <= last; ++i) {
		sf::FloatRect hitbox_loop = getGlobalHitbox(OLVert[i].first, OLVert[i].second, {0,0});
		if (f_abs(OLVert[i].second.x - object.property.getPosition().x) > distance) continue;
		//if (SaveList.size() > 0) hitbox_loop.position.y = SaveList[i].second;
		if (isCollide(hitbox_intersect, hitbox_loop)) {
			isCollideTopBool = true;
			if (!NoAdd) {
				CurrPosYCollide = hitbox_loop.position.y;
				NoAdd = true;
				break;
			}
		}
	}
	return isCollideTopBool;
}
int find_max_inx(const MovableObject& object, const std::vector<Obstacles>& OL) {
	int l = 0, r = OL.size() - 1, out = 0;
	while (l <= r) {
		if (const int m = (l + r) / 2; OL[m].property.getPosition().x - 32.0f <= object.property.getPosition().x) {
			l = m + 1;
			out = m;
		}
		else r = m - 1;
	}
	return out;
}
int find_max_inx_dist(const MovableObject& object, const std::vector<Obstacles>& OL, const float dist) {
	int l = 0, r = OL.size() - 1, out = 0;
	while (l <= r) {
		if (const int m = (l + r) / 2; OL[m].property.getPosition().x - dist <= object.property.getPosition().x) {
			l = m + 1;
			out = m;
		}
		else r = m - 1;
	}
	return out;
}
int find_min_inx(const MovableObject& object, const std::vector<Obstacles>& OL) {
	int l = 0, r = OL.size() - 1, out = 0;
	while (l <= r) {
		if (const int m = (l + r) / 2; OL[m].property.getPosition().x + 32.0f >= object.property.getPosition().x) {
			r = m - 1;
			out = m;
		}
		else l = m + 1;
	}
	return out;
}
int find_min_inx_dist(const MovableObject& object, const std::vector<Obstacles>& OL, const float dist) {
	int l = 0, r = OL.size() - 1, out = 0;
	while (l <= r) {
		if (const int m = (l + r) / 2; OL[m].property.getPosition().x + dist >= object.property.getPosition().x) {
			r = m - 1;
			out = m;
		}
		else l = m + 1;
	}
	return out;
}
int find_max_iny(const MovableObject& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL) {
	int l = 0, r = OL.size() - 1, out = 0;
	while (l <= r) {
		if (const int m = (l + r) / 2; OL[m].second.y - 32.0f <= object.property.getPosition().y) {
			l = m + 1;
			out = m;
		}
		else r = m - 1;
	}
	return out;
}
int find_max_iny_dist(const MovableObject& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, const float dist) {
	int l = 0, r = OL.size() - 1, out = 0;
	while (l <= r) {
		if (const int m = (l + r) / 2; OL[m].second.y - dist <= object.property.getPosition().y) {
			l = m + 1;
			out = m;
		}
		else r = m - 1;
	}
	return out;
}
int find_min_iny(const MovableObject& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL) {
	int l = 0, r = OL.size() - 1, out = 0;
	while (l <= r) {
		if (const int m = (l + r) / 2; OL[m].second.y + 32.0f >= object.property.getPosition().y) {
			r = m - 1;
			out = m;
		}
		else l = m + 1;
	}
	return out;
}
int find_min_iny_dist(const MovableObject& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, const float dist) {
	int l = 0, r = OL.size() - 1, out = 0;
	while (l <= r) {
		if (const int m = (l + r) / 2; OL[m].second.y + dist >= object.property.getPosition().y) {
			r = m - 1;
			out = m;
		}
		else l = m + 1;
	}
	return out;
}