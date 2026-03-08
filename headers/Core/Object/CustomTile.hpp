#ifndef MFCPP_CUSTOMTILE_HPP
#define MFCPP_CUSTOMTILE_HPP

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
    protected:
        CustomTileManager& m_customTileManager;
    };
}

#endif //MFCPP_CUSTOMTILE_HPP