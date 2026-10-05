#include "Core/WeatherController.hpp"

#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Utility.hpp"
#include "Core/Enumeration/enum.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Effect/RainEffect.hpp"

bool g_enableWeather = true;
WeatherID g_currentWeather = WeatherID::RAIN;
//Range
float g_YupperBound = 0.f;
float g_YlowerBound = 12640.f;
float g_XleftBound = 0.f;
float g_XrightBound = 12640.f;

float g_timePass = 0.f;

void WeatherController::RainEffectUpdate() {
    GameScene::effectManager.addEffect<RainEffect>(sf::Vector2f(Utility::RandomFloatNumberGenerator(-320.f, 1100.f) + Scroll::getViewPosition().x + 320.f, -48.f + g_YupperBound));
    GameScene::effectManager.addEffect<RainEffect>(sf::Vector2f(Utility::RandomFloatNumberGenerator(-320.f, 1100.f) + Scroll::getViewPosition().x + 320.f, -48.f + g_YupperBound));
    GameScene::effectManager.addEffect<RainEffect>(sf::Vector2f(Utility::RandomFloatNumberGenerator(-320.f, 1100.f) + Scroll::getViewPosition().x + 320.f, -48.f + g_YupperBound));
}

void WeatherController::setWeather(WeatherID id) {
    g_currentWeather = id;
}

void WeatherController::setWeatherActive(bool val) {
    g_enableWeather = val;
}

void WeatherController::updateWeather(float deltaTime) {
    if (!g_enableWeather) return;
    g_timePass += deltaTime;
    if (g_timePass >= 1.f) {
        // limit time update to 50fps only
        switch (g_currentWeather) {
            case WeatherID::RAIN:
                RainEffectUpdate();
                break;
            case WeatherID::SNOW:
                break;
            default:
                break;
        }
        g_timePass = Utility::f_mod(g_timePass, 1.f);
    }
}
void WeatherController::playWeatherSound() {
    if (!g_enableWeather) return;
    switch (g_currentWeather) {
        case WeatherID::RAIN:
            SoundManager::SetVolume(SoundID::GAME_RAIN, 0.5f);
            SoundManager::SetLoop(SoundID::GAME_RAIN, true);
            SoundManager::PlaySound(SoundID::GAME_RAIN);
            break;
        default:
            break;
    }
}