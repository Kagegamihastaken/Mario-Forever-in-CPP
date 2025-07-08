#ifndef GOOMBAAIEFFECTCLASS_HPP
#define GOOMBAAIEFFECTCLASS_HPP

#include "Core/Class/ActiveObjectClass.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Loading/enum.hpp"

namespace MFCPP {
    class GoombaAIEffect final : public ActiveObject, public StaticAnimationObject {
    public:
        GoombaAIEffect(GoombaAIEffectID id, int SkinID, float Yvelo, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin);
        ~GoombaAIEffect() = default;
        void setID(GoombaAIEffectID id);
        [[nodiscard]] GoombaAIEffectID getID() const;
        void setYVelo(float val);
        [[nodiscard]] float getYVelo() const;
        void setHitbox(const sf::FloatRect& hitbox);
        [[nodiscard]] sf::FloatRect getHitbox() const;
        void setAlpha(float val);
        [[nodiscard]] float getAlpha() const;
        void setClock(float val);
        [[nodiscard]] float getClock() const;
        void setSkinID(int val);
        [[nodiscard]] int getSkinID() const;
    private:
        GoombaAIEffectID m_id;
        float            m_Yvelo;
        sf::FloatRect    m_hitbox;
        float            m_alpha;
        float            m_clock;
        int              m_skinID;
    };
}

#endif //GOOMBAAIEFFECTCLASS_HPP
