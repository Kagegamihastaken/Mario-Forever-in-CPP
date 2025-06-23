#include "Class/GoombaAIClass.hpp"

#include "Core/SoundManager.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Effect/GoombaAIEffect.hpp"

namespace MFCPP {
    GoombaAI::GoombaAI(const GoombaAIType type, const GoombaAIDirection dir, const GoombaAICollisionType collision_type, const float Xvelo, const sf::FloatRect& size, const sf::Vector2f& pos,
        const sf::Vector2f& origin, const bool is_appeared, const unsigned skin_ID, const float invincible_timer_limit, const bool can_death)
        : m_type(type), m_direction(dir), m_collision_type(collision_type), m_Xvelo(Xvelo), m_size(size), m_is_appearing(is_appeared), m_skinID(skin_ID), m_can_death(can_death), m_invincible_timer_limit(invincible_timer_limit) {
        setOrigin(origin);
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        m_Yvelo = 0.0f;
        m_disabled = true;
        m_appear_y = 0.0f;
        m_invincible_timer.restart();
        m_shell_hit_count = 0;
        m_collide_with = { false, -1 };
        //hitbox
        m_hitbox_main = sf::FloatRect({ 0.0f, 0.0f }, { size.size.x, size.size.y });
    }
    void GoombaAI::SetType(const GoombaAIType& type) {
        m_type = type;
    }
    GoombaAIType GoombaAI::GetType() const {
        return m_type;
    }
    void GoombaAI::SetDirection(const GoombaAIDirection& direction) {
        m_direction = direction;
    }
    GoombaAIDirection GoombaAI::GetDirection() const {
        return m_direction;
    }
    void GoombaAI::SetCollisionType(const GoombaAICollisionType& collision_type) {
        m_collision_type = collision_type;
    }
    GoombaAICollisionType GoombaAI::GetCollisionType() const {
        return m_collision_type;
    }
    void GoombaAI::SetXvelo(const float& Xvelo) {
        m_Xvelo = Xvelo;
    }
    float GoombaAI::GetXvelo() const {
        return m_Xvelo;
    }
    void GoombaAI::SetYvelo(const float& Yvelo) {
        m_Yvelo = Yvelo;
    }
    float GoombaAI::GetYvelo() const {
        return m_Yvelo;
    }
    void GoombaAI::SetSize(const sf::FloatRect& size) {
        m_size = size;
    }
    sf::FloatRect GoombaAI::GetSize() const {
        return m_size;
    }
    void GoombaAI::SetAppearing(const bool& is_appeared) {
        m_is_appearing = is_appeared;
    }
    bool GoombaAI::IsAppearing() const {
        return m_is_appearing;
    }
    void GoombaAI::SetSkinID(const unsigned& skinID) {
        m_skinID = skinID;
    }
    unsigned GoombaAI::GetSkinID() const {
        return m_skinID;
    }
    //Dont touch
    void GoombaAI::SetDisabled(const bool& disabled) {
        m_disabled = disabled;
    }
    bool GoombaAI::IsDisabled() const {
        return m_disabled;
    }
    void GoombaAI::SetAppearY(const float& appear_y) {
        m_appear_y = appear_y;
    }
    float GoombaAI::GetAppearY() const {
        return m_appear_y;
    }
    sf::Clock GoombaAI::GetInvincibleTimer() const {
        return m_invincible_timer;
    }
    void GoombaAI::SetInvincibleTimerLimit(const float& invincible_timer_limit) {
        m_invincible_timer_limit = invincible_timer_limit;
    }
    float GoombaAI::GetInvincibleTimerLimit() const {
        return m_invincible_timer_limit;
    }
    void GoombaAI::SetShellHitCount(const int& shell_hit_count) {
        m_shell_hit_count = shell_hit_count;
    }
    int GoombaAI::GetShellHitCount() const {
        return m_shell_hit_count;
    }
    void GoombaAI::SetCollideWith(const std::pair<bool, int>& collide_with) {
        m_collide_with = collide_with;
    }
    void GoombaAI::SetCollideWithLeft(const bool val) {
        m_collide_with.first = val;
    }
    void GoombaAI::SetCollideWithRight(const int val) {
        m_collide_with.second = val;
    }
    std::pair<bool, int> GoombaAI::GetCollideWith() const {
        return m_collide_with;
    }
    void GoombaAI::SetHitboxMain(const sf::FloatRect& hitbox_main) {
        m_hitbox_main = hitbox_main;
    }
    sf::FloatRect GoombaAI::GetHitboxMain() const {
        return m_hitbox_main;
    }
    void GoombaAI::SetCanDeath(const bool val) {
        m_can_death = val;
    }
    bool GoombaAI::IsCanDeath() const {
        return m_can_death;
    }

    void GoombaAI::DeathBehaviour(const ScoreID score_id) const {
        if (!IsCanDeath()) return;
        AddScoreEffect(score_id, getCurrentPosition().x, getCurrentPosition().y - getOrigin().y);
        AddGoombaAIEffect(GetType(), NONE, GetSkinID(), getCurrentPosition().x, getCurrentPosition().y);
        SoundManager::PlaySound("Kick2");
    }

}