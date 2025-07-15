#ifndef SCOREEFFECTCLASS_HPP
#define SCOREEFFECTCLASS_HPP
#include "ActiveObjectClass.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"
#include "SFML/System/Vector2.hpp"

namespace MFCPP {
    class ScoreEffect final : public ActiveObject<float>, public StaticAnimationObject {
    public:
        ScoreEffect(const sf::Vector2f& pos, const sf::Vector2f& origin);
        ~ScoreEffect() = default;
        void setVelocity(float val);
        [[nodiscard]] float getVelocity() const;
        void setAlpha(float val);
        [[nodiscard]] float getAlpha() const;
    private:
        float m_velocity;
        float m_alpha;
    };
}
#endif //SCOREEFFECTCLASS_HPP
