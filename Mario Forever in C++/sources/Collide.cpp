#include <SFML/Graphics.hpp>
#include <array>
#include <functional>
#include <vector>

#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Core/Collision/Collide.hpp"
#include "../headers/Object/Mario.hpp"
#include "../headers/Block/Obstacles.hpp"
#include "../headers/Block/Slopes.hpp"

void setHitbox(sf::FloatRect& hitbox, const sf::FloatRect& Sethitbox) {
	hitbox = Sethitbox;
}
sf::FloatRect getGlobalHitbox(const sf::FloatRect& hitbox, const sf::Vector2f& pos) {
	return sf::FloatRect({ pos.x + hitbox.position.x, pos.y + hitbox.position.y }, { hitbox.size.x, hitbox.size.y });
}
sf::FloatRect getGlobalHitbox(const sf::FloatRect& hitbox, const sf::Sprite& sprite) {
	return sprite.getTransform().transformRect(hitbox);
}
bool isCollide(const sf::FloatRect& hitbox, const sf::Sprite& sprite, const sf::FloatRect& other) {
	return bool(getGlobalHitbox(hitbox, sprite).findIntersection(other));
}

// Mario && GoombaAI only
std::pair<bool, bool> isAccurateCollideSidet(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, int first, int last, float distance, const std::vector<std::pair<float, float>>& SaveList = {}) {
	bool isCollideLeftBool = false, isCollideRightBool = false;
	sf::FloatRect hitbox_loop;
	for (int i = first; i <= last; ++i) {
		if (f_abs(OL[i].property.getPosition().y - object.property.getPosition().y) > distance) continue;
		hitbox_loop = getGlobalHitbox(OL[i].hitbox, OL[i].property);
		if (SaveList.size() > 0) {
			hitbox_loop.position.y = SaveList[i].second;
			hitbox_loop.position.x = SaveList[i].first;
		}
		// Check if collide
		if (isCollide(object.hitboxLeft, object.property, hitbox_loop)) {
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
		if (isCollide(object.hitboxRight, object.property, hitbox_loop)) {
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
std::pair<bool, bool> isAccurateCollideSide(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, const std::vector<std::pair<float, float>>& SaveList = {}) {
	bool isCollideLeftBool = false, isCollideRightBool = false;
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = getGlobalHitbox(OL[i].hitbox, OL[i].property);
		if (SaveList.size() > 0) {
			hitbox_loop.position.y = SaveList[i].second;
			hitbox_loop.position.x = SaveList[i].first;
		}
		// Check if collide
		if (isCollide(object.hitboxLeft, object.property, hitbox_loop)) {
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
		if (isCollide(object.hitboxRight, object.property, hitbox_loop)) {
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
bool isCollideBotSlope(const MovableObject& object, float Yvelo, float deltaTime) {
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < SlopesList.size(); ++i) {
		hitbox_loop = getGlobalHitbox(SlopesList[i].hitbox, SlopesList[i].property);
		hitbox_loop.position.y -= 32.0f;
		hitbox_loop.size.y += 32.0f;
		if (isCollide(object.hitboxBot2, object.property, hitbox_loop)) {
			for (auto& j : SlopeRectList[SlopesIDList[i]]) {
				hitbox_loop = getGlobalHitbox(j, SlopesList[i].property);
				if (Yvelo == 0.0f) {
					hitbox_loop.position.y -= 9.0f;
					hitbox_loop.size.y += 9.0f;
				}
				else {
					hitbox_loop.position.y += 1.0f / deltaTime - 3.0f;
					hitbox_loop.size.y -= 1.0f / deltaTime - 3.0f;
				}
				if (isCollide(object.hitboxBot2, object.property, hitbox_loop)) return true;
			}
		}
	}
	return false;
}
bool isCollideBot(const MovableObject& object, const std::vector<Obstacles>& OL) {
	sf::FloatRect hitbox_loop;
	for (auto& i : OL) {
		hitbox_loop = getGlobalHitbox(i.hitbox, i.property);
		if (isCollide(object.hitboxBot, object.property, hitbox_loop)) return true;
	}
	return false;
}
bool isAccurateCollideBott(const MovableObject& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OLVert, float& CurrPosYCollide, bool& NoAdd, int first, int last, float distance) {
	bool isCollideBotBool = false;
	sf::FloatRect hitbox_loop;
	for (int i = first; i <= last; ++i) {
		if (f_abs(OLVert[i].second.x - object.property.getPosition().x) > distance) continue;
		hitbox_loop = getGlobalHitbox(OLVert[i].first, OLVert[i].second);
		if (isCollide(object.hitboxBot, object.property, hitbox_loop)) {
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
std::pair<bool, std::pair<bool, bool>> isAccuratelyCollideBotSlope(const MovableObject& object, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, float& ID, std::vector<std::array<float, 3>>& SlopeTemp) {
	bool isCollideBotBool = false, isCollideRight = false, isCollideLeft = false;
	sf::FloatRect hitbox_loop;
	const sf::FloatRect hitbox = sf::FloatRect({ 0.0f, -32.0f }, { 32.0f, 64.0f });
	for (int i = 0; i < SlopesList.size(); ++i) {
		hitbox_loop = getGlobalHitbox(hitbox, SlopesList[i].property);
		if (isCollide(object.hitboxSlopeBot, object.property, hitbox_loop)) {
			isCollideBotBool = true;
			CurrPosXCollide = SlopesList[i].property.getPosition().x;
			CurrPosYCollide = hitbox_loop.position.y + 32.0f;
			ID = SlopesIDList[i];
			SlopeTemp.push_back({ CurrPosXCollide, CurrPosYCollide, ID });
		}
		if (isCollide(object.hitboxRight, object.property, hitbox_loop)) isCollideRight = true;
		if (isCollide(object.hitboxLeft, object.property, hitbox_loop)) isCollideLeft = true;
		if (isCollideBotBool && isCollideRight && isCollideLeft) break;
	}
	return { isCollideBotBool, { isCollideRight, isCollideLeft } };
}
std::pair<bool, std::pair<bool, bool>> isAccuratelyCollideBot(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, float& ID, std::vector<std::array<float, 3>>& SlopeTemp) {
	bool isCollideBotBool = false, isCollideRight = false, isCollideLeft = false;
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = getGlobalHitbox(OL[i].hitbox, OL[i].property);
		if (isCollide(object.hitboxBot, object.property, hitbox_loop)) {
			isCollideBotBool = true;
			if (!NoAdd) {
				CurrPosXCollide = OL[i].property.getPosition().x;
				CurrPosYCollide = hitbox_loop.position.y;
				ID = SlopesIDList[i];
				NoAdd = true;
			}
			CurrPosXCollide = OL[i].property.getPosition().x;
			CurrPosYCollide = hitbox_loop.position.y;
			ID = SlopesIDList[i];
			SlopeTemp.push_back({ CurrPosXCollide, CurrPosYCollide, ID });
		}
		if (isCollide(object.hitboxRight, object.property, hitbox_loop)) isCollideRight = true;
		if (isCollide(object.hitboxLeft, object.property, hitbox_loop)) isCollideLeft = true;
		if (isCollideBotBool && isCollideRight && isCollideLeft) break;
	}
	return { isCollideBotBool, { isCollideRight, isCollideLeft } };
}
bool isCollideTop(const MovableObject& object, const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList = {}) {
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = getGlobalHitbox(OL[i].hitbox, OL[i].property);
		if (SaveList.size() > 0) hitbox_loop.position.y = SaveList[i].second;
		if (isCollide(object.hitboxTop, object.property, hitbox_loop)) return true;
	}
	return false;
}
std::vector<std::pair<float, float>> isCollideTopDetailed(const MovableObject& object, const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList = {}) {
	std::vector<std::pair<float, float>> result;
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = getGlobalHitbox(OL[i].hitbox, OL[i].property);
		if (SaveList.size() > 0) hitbox_loop.position.y = SaveList[i].second;
		if (isCollide(object.hitboxTop, object.property, hitbox_loop)) result.push_back({ hitbox_loop.position.x, hitbox_loop.position.y });
	}
	return result;
}
bool isAccurateCollideTopt(const MovableObject& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OLVert, float& CurrPosYCollide, bool& NoAdd, int first, int last, float distance) {
	bool isCollideTopBool = false, isCollideSide = false;
	sf::FloatRect hitbox_loop;
	for (int i = first; i <= last; ++i) {
		hitbox_loop = getGlobalHitbox(OLVert[i].first, OLVert[i].second);
		if (f_abs(OLVert[i].second.x - object.property.getPosition().x) > distance) continue;
		//if (SaveList.size() > 0) hitbox_loop.position.y = SaveList[i].second;
		if (isCollide(object.hitboxTop, object.property, hitbox_loop)) {
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
	int l = 0, r = OL.size() - 1, m, out = 0;
	while (l <= r) {
		m = (l + r) / 2;
		if (OL[m].property.getPosition().x - 32.0f <= object.property.getPosition().x) {
			l = m + 1;
			out = m;
		}
		else r = m - 1;
	}
	return out;
}
int find_max_inx_dist(const MovableObject& object, const std::vector<Obstacles>& OL, float dist) {
	int l = 0, r = OL.size() - 1, m, out = 0;
	while (l <= r) {
		m = (l + r) / 2;
		if (OL[m].property.getPosition().x - dist <= object.property.getPosition().x) {
			l = m + 1;
			out = m;
		}
		else r = m - 1;
	}
	return out;
}
int find_min_inx(const MovableObject& object, const std::vector<Obstacles>& OL) {
	int l = 0, r = OL.size() - 1, m, out = 0;
	while (l <= r) {
		m = (l + r) / 2;
		if (OL[m].property.getPosition().x + 32.0f >= object.property.getPosition().x) {
			r = m - 1;
			out = m;
		}
		else l = m + 1;
	}
	return out;
}
int find_min_inx_dist(const MovableObject& object, const std::vector<Obstacles>& OL, float dist) {
	int l = 0, r = OL.size() - 1, m, out = 0;
	while (l <= r) {
		m = (l + r) / 2;
		if (OL[m].property.getPosition().x + dist >= object.property.getPosition().x) {
			r = m - 1;
			out = m;
		}
		else l = m + 1;
	}
	return out;
}
int find_max_iny(const MovableObject& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL) {
	int l = 0, r = OL.size() - 1, m, out = 0;
	while (l <= r) {
		m = (l + r) / 2;
		if (OL[m].second.y - 32.0f <= object.property.getPosition().y) {
			l = m + 1;
			out = m;
		}
		else r = m - 1;
	}
	return out;
}
int find_max_iny_dist(const MovableObject& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, float dist) {
	int l = 0, r = OL.size() - 1, m, out = 0;
	while (l <= r) {
		m = (l + r) / 2;
		if (OL[m].second.y - dist <= object.property.getPosition().y) {
			l = m + 1;
			out = m;
		}
		else r = m - 1;
	}
	return out;
}
int find_min_iny(const MovableObject& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL) {
	int l = 0, r = OL.size() - 1, m, out = 0;
	while (l <= r) {
		m = (l + r) / 2;
		if (OL[m].second.y + 32.0f >= object.property.getPosition().y) {
			r = m - 1;
			out = m;
		}
		else l = m + 1;
	}
	return out;
}
int find_min_iny_dist(const MovableObject& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, float dist) {
	int l = 0, r = OL.size() - 1, m, out = 0;
	while (l <= r) {
		m = (l + r) / 2;
		if (OL[m].second.y + dist >= object.property.getPosition().y) {
			r = m - 1;
			out = m;
		}
		else l = m + 1;
	}
	return out;
}