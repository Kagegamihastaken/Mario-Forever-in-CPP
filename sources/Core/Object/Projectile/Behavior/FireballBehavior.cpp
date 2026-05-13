#include "Core/Object/Projectile/Behavior/FireballBehavior.hpp"
#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Collision/Collide.hpp"
#include <SFML/System/Vector2.hpp>

#include "SFML/Graphics/Rect.hpp"

FireballBehavior::FireballData FireballBehavior::FireballX(const sf::Vector2f& pos, const sf::Vector2f& velocity, const bool direction, const float deltaTime, const sf::FloatRect& hitbox, const sf::Vector2f& origin) {
    FireballData data{pos, velocity, false};
    if (direction) data.position.x -= data.velocity.x * deltaTime;
    else data.position.x += data.velocity.x * deltaTime;

    if (float PlatDistance; PlatformXCollision(MFCPP::CollisionObject(data.position, origin, hitbox), PlatDistance, data.velocity.y))
        data.position.x += PlatDistance;

    float CurrPosXCollide = 0, CurrPosYCollide = 0;
    const auto [fst, snd] = QuickCheckSideCollision(
        MFCPP::CollisionObject(data.position, origin, hitbox), direction,
        CurrPosXCollide, CurrPosYCollide);
    if (fst || snd)
        data.remove = true;
    return data;
}
FireballBehavior::FireballData FireballBehavior::FireballY(const sf::Vector2f& pos, bool& do_stuff, const sf::Vector2f& velocity, const float deltaTime, const sf::FloatRect& hitbox, const sf::Vector2f& origin) {
    FireballData data{pos, velocity, false};
    do_stuff = false;
    data.position.y += data.velocity.y * deltaTime;
    data.velocity.y += (data.velocity.y >= 10.0f ? 0.0f : 1.f * deltaTime * 0.3f);
    if (data.velocity.y > 10.f) data.velocity.y = 10.f;

    if (float PlatPosY; PlatformYCollision(MFCPP::CollisionObject(data.position, origin, hitbox), PlatPosY, data.velocity.y, false)) {
        data.position.y = PlatPosY;
        do_stuff = true;
        //data.velocity.y = -5.f;
    }
    float CurrPosYCollide, CurrPosXCollide;
    //if (QuickCheckTopCollision(MFCPP::CollisionObject(sf::Vector2f(data.X, data.Y), origin, hitbox), CurrPosXCollide, CurrPosYCollide))
        //data.remove = true;
    const float offset = data.velocity.x + 1.f;
    if (QuickCheckBotCollision(MFCPP::CollisionObject(data.position, origin, hitbox), offset, CurrPosXCollide, CurrPosYCollide)) {
        if (data.velocity.y >= -data.velocity.x) {
            const float floorY = GetCurrFloorY(data.position, CurrPosXCollide, CurrPosYCollide);
            if (data.position.y < CurrPosYCollide + floorY - offset) return data;
            data.position.y = CurrPosYCollide + floorY - (hitbox.size.y - origin.y);
            do_stuff = true;
            //data.velocity.y = -5.f;
        }
    }
    return data;
}