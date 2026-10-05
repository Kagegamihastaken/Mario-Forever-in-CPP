#ifndef MFCPP_WEATHERCONTROLLER_HPP
#define MFCPP_WEATHERCONTROLLER_HPP
#include "Enumeration/enum.hpp"

namespace WeatherController {
    static void RainEffectUpdate();
    void updateWeather(float deltaTime);
    void setWeather(WeatherID id);
    void setWeatherActive(bool val);
    void playWeatherSound();
}

#endif //MFCPP_WEATHERCONTROLLER_HPP
