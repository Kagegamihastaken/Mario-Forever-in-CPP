#include "Core/Object/Projectile/Behavior/BeetrootBehavior.hpp"
#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Collision/Collide.hpp"
#include <SFML/System/Vector2.hpp>

#include "SFML/Graphics/Rect.hpp"

BeetrootBehavior::BeetrootData BeetrootBehavior::BeetrootXUpdate(const sf::Vector2f &pos, int hit_count, const sf::Vector2f &velocity, bool direction, float deltaTime) {
    BeetrootData data{pos, velocity, hit_count};
    if (direction) data.position.x -= data.velocity.x * deltaTime;
    else data.position.x += data.velocity.x * deltaTime;
    return data;
}

BeetrootBehavior::BeetrootData BeetrootBehavior::BeetrootYUpdate(const sf::Vector2f &pos, int hit_count, const sf::Vector2f &velocity, float deltaTime) {
    BeetrootData data{pos, velocity, hit_count};

    data.position.y += data.velocity.y * deltaTime;
    data.velocity.y += (data.velocity.y >= 10.0f ? 0.0f : 1.f * deltaTime * 0.3f);
    if (data.velocity.y > 10.f) data.velocity.y = 10.f;

    return data;
}

BeetrootBehavior::BeetrootData BeetrootBehavior::BeetrootXCollision(const sf::Vector2f& pos, int hit_count, int& do_stuff, const sf::Vector2f& velocity, const bool direction, const float deltaTime, const sf::FloatRect& hitbox, const sf::Vector2f& origin) {
    BeetrootData data{pos, velocity, hit_count};
    do_stuff = 0;

    if (data.count > 2) return data;
    if (float PlatDistance; PlatformXCollision(MFCPP::CollisionObject(data.position, origin, hitbox), PlatDistance, data.velocity.y))
        data.position.x += PlatDistance;

    float CurrPosXCollide = 0, CurrPosYCollide = 0;
    const auto [fst, snd] = QuickCheckSideCollision(
        MFCPP::CollisionObject(data.position, origin, hitbox), direction,
        CurrPosXCollide, CurrPosYCollide);
    if (fst) {
        do_stuff = 1;
        ++data.count;
        data.position.x = CurrPosXCollide + 32.0f - hitbox.position.x + origin.x;
    }
    else if (snd) {
        do_stuff = 1;
        ++data.count;
        data.position.x = CurrPosXCollide - (hitbox.position.x + hitbox.size.x - origin.x);
    }
    return data;
}
BeetrootBehavior::BeetrootData BeetrootBehavior::BeetrootYCollision(const sf::Vector2f& pos, int hit_count, int& do_stuff, const sf::Vector2f& velocity, const float deltaTime, const sf::FloatRect& hitbox, const sf::Vector2f& origin) {
    BeetrootData data{pos, velocity, hit_count};
    do_stuff = 0;

    if (data.count > 2) return data;
    if (float PlatPosY; PlatformYCollision(MFCPP::CollisionObject(data.position, origin, hitbox), PlatPosY, data.velocity.y, false)) {
        data.position.y = PlatPosY;
        ++data.count;
        do_stuff = 1;
        //data.velocity.y = -5.f;
    }
    float CurrPosYCollide, CurrPosXCollide;
    if (QuickCheckTopCollision(MFCPP::CollisionObject(data.position, origin, hitbox), CurrPosXCollide, CurrPosYCollide)) {
        data.position.y = CurrPosYCollide + (32.0f + origin.y);
        ++data.count;
        do_stuff = 2;
    }
    if (QuickCheckBotCollision(MFCPP::CollisionObject(data.position, origin, hitbox), CurrPosXCollide, CurrPosYCollide)) {
        if (const float offset = data.velocity.x + 1.f; data.velocity.y >= -data.velocity.x) {
            const float floorY = GetCurrFloorY(data.position, CurrPosXCollide, CurrPosYCollide);
            if (data.position.y < CurrPosYCollide + floorY - offset) return data;
            data.position.y = CurrPosYCollide + floorY - (hitbox.size.y - origin.y);
            ++data.count;
            do_stuff = 1;
        }
    }
    return data;
}