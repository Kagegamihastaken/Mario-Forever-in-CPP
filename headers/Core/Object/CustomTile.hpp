#ifndef MFCPP_CUSTOMTILE_HPP
#define MFCPP_CUSTOMTILE_HPP

#include <bitset>
#include <boost/intrusive/list.hpp>
#include "ActiveObject.hpp"

using CustomTileIntrusiveHook = boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;
class CustomTileManager;

namespace MFCPP {
    class CustomTile : public CustomTileIntrusiveHook {
        enum Flag : uint8_t {
            DrawingPriorityLow = 0,
            DrawingPriorityHigh = 1
        };
    public:
        explicit CustomTile(CustomTileManager& manager) : m_customTileManager(manager) {};

        virtual void updatePreviousData() = 0;
        virtual void statusUpdate(float deltaTime) = 0;
        virtual void HitEvent() = 0;
        virtual void KickEvent() = 0;
        virtual void draw(float alpha) = 0;
        virtual void animationUpdate(float deltaTime) = 0;
        [[nodiscard]] virtual bool isDestroyed() = 0;
        [[nodiscard]] virtual sf::Vector2f getPosition() = 0;
        [[nodiscard]] virtual sf::Vector2f getOrigin() = 0;
        [[nodiscard]] virtual sf::FloatRect getHitbox() = 0;

        [[nodiscard]] int getDrawingPriority() const {
            return m_option[DrawingPriorityLow] + (m_option[DrawingPriorityHigh] << 1);
        }
        virtual ~CustomTile() = default;
    protected:
        void setDrawingPriority(int val) {
            const uint8_t num = val & 3;
            m_option[DrawingPriorityLow] = num & 1;
            m_option[DrawingPriorityHigh] = (num >> 1) & 1;
        }

        CustomTileManager& m_customTileManager;
        std::bitset<2> m_option = 0;
    };
}

#endif //MFCPP_CUSTOMTILE_HPP