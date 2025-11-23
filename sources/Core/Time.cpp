#include "Core/Time.hpp"

#include "Core/SoundManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/ExitGate.hpp"
#include "Object/Mario.hpp"

static int TimeCountBegin = 360; //Default
static int TimeCount = TimeCountBegin;
static constexpr int TimeRangThreshold = 100;
static float TimeRan = 0.f;
static bool TimeRang = false;
static constexpr float TimeBetween = 0.5f * 50.f;

static float TimeRanCounting = 0.f;
static float TimeRanCountingSound = 0.f; //for "ticking" sound only
void TimeUpdate(const float deltaTime) {
    if (EffectActive || LevelCompleteEffect) return;
    if (TimeCount < 1) {
        CanControlMario = false;
        ActiveMarioEffect();
        TimeCount = 0;
        return;
    }
    TimeRan += deltaTime;
    if (TimeRan >= TimeBetween) {
        const int TimeRanOver = static_cast<int>(TimeRan / TimeBetween);
        TimeRan -= static_cast<float>(TimeRanOver) * TimeBetween;
        TimeCount -= TimeRanOver;
    }
}
void TimeCounting(const float deltaTime) {
    TimeRanCounting += deltaTime;
    if (TimeRanCounting >= 1.f) { // 1/50
        const int TimeRanOver = static_cast<int>(TimeRanCounting);
        if (TimeCount >= 4 * TimeRanOver) {
            TimeCount -= 4 * TimeRanOver;
            Score += 40 * TimeRanOver;
        }
        else {
            Score += 10 * TimeCount;
            TimeCount = 0;
        }
        TimeRanCounting -= static_cast<float>(TimeRanOver);
    }
    //Ticking purpose
    TimeRanCountingSound += deltaTime;
    if (TimeRanCountingSound >= 5.f) {
        SoundManager::PlaySound("Throw");
        TimeRanCountingSound = f_mod(TimeRanCountingSound, 5.f);
    }
}
int getTime() {
    return TimeCount;
}
void TimeRingBehavior() {
    if (LevelCompleteEffect || EffectActive) return;
    if (TimeCount > TimeRangThreshold) TimeRang = false;
    if (TimeRang) return;
    if (TimeCount <= TimeRangThreshold) {
        SoundManager::PlaySound("Timeout");
        TimeRang = true;
    }
}
void TimeReset() {
    TimeRang = false;
    TimeCount = TimeCountBegin;
    TimeRan = 0.f;
}
void setDefaultTime(const int val) {
    TimeCountBegin = val;
    TimeReset();
}
