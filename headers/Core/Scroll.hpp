#pragma once

#ifndef SCROLL_HPP
#define SCROLL_HPP
#include "Class/CollisionObjectClass.hpp"

extern void ViewInit();
extern bool MarioLockedView;
extern sf::View view;
extern float ViewX, ViewY;
extern float ViewXOff, ViewYOff;
extern void WindowSetView();
extern void updateView();
extern void setRotate(int degree);
extern void moveView(float x, float y);
extern bool isOutOfScreen(const MFCPP::CollisionObject& obj, float offset);
extern bool isOutOfScreenYBottom(const MFCPP::CollisionObject& obj, float offset);
extern bool isOutOfScreenXLeft(const MFCPP::CollisionObject& obj, const float offset);
extern bool isOutOfScreenXRight(const MFCPP::CollisionObject& obj, const float offset);

#endif // SCROLL_HPP