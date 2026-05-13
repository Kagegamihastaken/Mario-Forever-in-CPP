#ifndef MFCPP_FIRELAUNCHERBEHAVIOR_HPP
#define MFCPP_FIRELAUNCHERBEHAVIOR_HPP

class FireLauncherBehavior {
public:
    struct FireLauncherData {
        float FireInterval, FireIntervalCounting;
        int ProjectileCount, ProjectileCounting;
        float FireBetweenInterval, FireBetweenIntervalCounting;
        bool PlaySound;
    };
    static FireLauncherData FireLauncherStatusUpdate(const FireLauncherData& data, bool& launch, bool& sound, float deltaTime);
};

#endif //MFCPP_FIRELAUNCHERBEHAVIOR_HPP
