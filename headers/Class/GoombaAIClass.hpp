#ifndef GOOMBAAICLASS_HPP
#define GOOMBAAICLASS_HPP

#include "Class/ActiveObjectClass.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Loading/enum.hpp"

namespace MFCPP {
    class GoombaAI final : public ActiveObject, public SingleAnimationObject {
    private:
        GoombaAIType           m_type{};
        GoombaAIDirection      m_direction{};
        GoombaAICollisionType  m_collision_type{};
        float                  m_Xvelo{};
        float                  m_Yvelo{};
        sf::FloatRect          m_size{};
        bool                   m_is_appearing{};
        unsigned               m_skinID{};
        sf::FloatRect          m_hitbox_main{};
        sf::FloatRect          m_hitbox_wall{};
        bool                   m_can_death{};
        //Dont touch
        bool                   m_disabled{};
        float                  m_appear_y{}; //for m_is_appeared = true
        sf::Clock              m_invincible_timer{};
        float                  m_invincible_timer_limit{};
        int                    m_shell_hit_count{};
        std::pair<bool, int>   m_collide_with{};
    public:
        GoombaAI(GoombaAIType type, GoombaAIDirection dir, GoombaAICollisionType collision_type, float Xvelo, const sf::FloatRect& size, const sf::Vector2f& pos,
        const sf::Vector2f& origin, bool is_appeared, unsigned skin_ID, float invincible_timer_limit, bool can_death = true);
        ~GoombaAI() = default;
        void SetType(const GoombaAIType& type);
        [[nodiscard]] GoombaAIType GetType() const;
        void SetDirection(const GoombaAIDirection& direction);
        [[nodiscard]] GoombaAIDirection GetDirection() const;
        void SetCollisionType(const GoombaAICollisionType& collision_type);
        [[nodiscard]] GoombaAICollisionType GetCollisionType() const;
        void SetXvelo(const float& Xvelo);
        [[nodiscard]] float GetXvelo() const;
        void SetYvelo(const float& Yvelo);
        [[nodiscard]] float GetYvelo() const;
        void SetSize(const sf::FloatRect& size);
        [[nodiscard]] sf::FloatRect GetSize() const;
        void SetAppearing(const bool& is_appeared);
        [[nodiscard]] bool IsAppearing() const;
        void SetSkinID(const unsigned& skinID);
        [[nodiscard]] unsigned GetSkinID() const;
        void SetHitboxMain(const sf::FloatRect& hitbox_main);
        [[nodiscard]] sf::FloatRect GetHitboxMain() const;
        //Dont touch
        void SetDisabled(const bool& disabled);
        [[nodiscard]] bool IsDisabled() const;
        void SetAppearY(const float& appear_y);
        [[nodiscard]] float GetAppearY() const;
        [[nodiscard]] sf::Clock GetInvincibleTimer() const;
        void SetInvincibleTimerLimit(const float& invincible_timer_limit);
        [[nodiscard]] float GetInvincibleTimerLimit() const;
        void SetShellHitCount(const int& shell_hit_count);
        [[nodiscard]] int GetShellHitCount() const;
        void SetCollideWith(const std::pair<bool, int>& collide_with);
        [[nodiscard]] std::pair<bool, int> GetCollideWith() const;
        void SetCanDeath(bool val);
        [[nodiscard]] bool IsCanDeath() const;
        void SetCollideWithLeft(bool val);
        void SetCollideWithRight(int val);

        void DeathBehaviour(ScoreID score_id) const;
        void SetHitboxWall(const sf::FloatRect& val);
        [[nodiscard]] sf::FloatRect GetHitboxWall() const;
    };
}

#endif //GOOMBAAICLASS_HPP