#ifndef BROAIEFFECTCLASS_HPP
#define BROAIEFFECTCLASS_HPP
#include "Core/Class/ActiveObjectClass.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"

namespace MFCPP {
    class BroAIEffect final : public ActiveObject<float>, public StaticAnimationObject {
    public:
        explicit BroAIEffect(float Yvelo, const sf::Vector2f& pos, const sf::Vector2f& origin);
        ~BroAIEffect() = default;
        void setYVelo(float val);
        [[nodiscard]] float getYVelo() const;
    private:
        float            m_Yvelo;
    };
}
#endif //BROAIEFFECTCLASS_HPP
