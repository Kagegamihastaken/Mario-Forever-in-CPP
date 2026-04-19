#ifndef MFCPP_CUSTOMTILE_HPP
#define MFCPP_CUSTOMTILE_HPP

#include <bitset>
#include <boost/intrusive/list.hpp>
#include "Core/Class/ActiveObjectClass.hpp"

using CustomTileIntrusiveHook = boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;
class CustomTileManager;

namespace MFCPP {
    class CustomTile : public CustomTileIntrusiveHook, public ActiveObject<float> {
    public:
        explicit CustomTile(CustomTileManager& manager) : m_customTileManager(manager) {};

        virtual void setPreviousData() = 0;
        virtual void interpolateData(float alpha) = 0;
        virtual void statusUpdate(float deltaTime) = 0;
        virtual void HitEvent() = 0;
        virtual void KickEvent() = 0;
        virtual void draw() = 0;

        void setDrawingPriority(const int val) {
            m_option[0] = val & 1;
            m_option[1] = (val >> 1) & 1;
        }
        [[nodiscard]] int getDrawingPriority() const {
            return m_option[0] + (m_option[1] << 1);
        }
    protected:
        CustomTileManager& m_customTileManager;
        std::bitset<2> m_option = 0;
    };
}

#endif //MFCPP_CUSTOMTILE_HPP