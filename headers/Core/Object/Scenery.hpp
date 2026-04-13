#ifndef MFCPP_SCENERY_HPP
#define MFCPP_SCENERY_HPP

#include <boost/intrusive/list_hook.hpp>
#include "Core/Class/ActiveObjectClass.hpp"

using SceneryIntrusiveHook = boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;
class SceneryManager;

namespace MFCPP {
    class Scenery : public SceneryIntrusiveHook, public ActiveObject<float> {
    public:
        explicit Scenery(SceneryManager& manager) : m_manager(manager) {};
        virtual void setPreviousData() = 0;
        virtual void interpolateData(float alpha) = 0;
        virtual void draw() = 0;
    protected:
        SceneryManager& m_manager;
    };
}

#endif //MFCPP_SCENERY_HPP
