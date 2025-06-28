#ifndef BULLETLAUNCHERCLASS_HPP
#define BULLETLAUNCHERCLASS_HPP
#include "ActiveObjectClass.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"

namespace MFCPP {
    class BulletLauncher final : public ActiveObject, public StaticAnimationObject {
    public:
        BulletLauncher() = default;
        ~BulletLauncher() = default;
    };
}

#endif //BULLETLAUNCHERCLASS_HPP
