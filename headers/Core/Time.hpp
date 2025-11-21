#ifndef MFCPP_TIME_HPP
#define MFCPP_TIME_HPP

extern void TimeUpdate(float deltaTime);
extern void TimeReset();
extern void TimeRingBehavior();
extern int getTime();
extern void TimeCounting(float deltaTime);
extern void setDefaultTime(int val);

#endif //MFCPP_TIME_HPP