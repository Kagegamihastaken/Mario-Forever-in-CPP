#pragma once

#ifndef SCROLL_HPP
#define SCROLL_HPP

extern void ViewInit();
extern bool MarioLockedView;
extern sf::View view;
extern float ViewX, ViewY;
extern float ViewXOff, ViewYOff;
extern void WindowSetView();
extern void updateView();
extern void setRotate(int degree);
extern bool isOutScreen(float xPos, float yPos, float OffsetX, float OffsetY);
extern bool isOutScreenX(float xPos, float OffsetX);
extern bool isOutScreenY(float yPos, float OffsetY);
extern bool isOutScreenYBottom(float yPos, float OffsetY);
extern void moveView(float x, float y);

#endif // SCROLL_HPP