#ifndef MFCPP_BULLETBILLLAUNCHERBEHAVIOR_HPP
#define MFCPP_BULLETBILLLAUNCHERBEHAVIOR_HPP

class BulletBillLauncherBehavior {
public:
    struct BulletBillLauncherData {
        sf::Vector2f position;
        bool disabled, state;
        float timing, launch_interval, first_time_shot;
        int random_fire_interval;
    };
    static BulletBillLauncherData BulletBillLauncherUpdate(const BulletBillLauncherData& data, bool& output, float deltaTime);
};

#endif //MFCPP_BULLETBILLLAUNCHERBEHAVIOR_HPP