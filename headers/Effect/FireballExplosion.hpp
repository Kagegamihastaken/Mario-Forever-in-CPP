#ifndef FIREBALLEXPLOSION_HPP
#define FIREBALLEXPLOSION_HPP
#include <string>
#include <vector>
#include "Class/FireballExplosionClass.hpp"

extern std::vector<MFCPP::FireballExplosion> FireballExplosionList;
extern void FireballExplosionInit();
extern void AddFireballExplosion(float x, float y);
extern void DeleteFireballExplosion(int i);
extern void DeleteFireballExplosion(float x, float y);
extern void DeleteAllFireballExplosion();
extern void FireballExplosionStatusUpdate();
extern void FireballExplosionDraw();

static std::vector<std::string> FireballExplosionAnimName;
static constexpr int FIREBALLEXPLOSION_IMAGE_WIDTH = 99;
static constexpr int FIREBALLEXPLOSION_WIDTH = 33;
static constexpr int FIREBALLEXPLOSION_HEIGHT = 30;

#endif //FIREBALLEXPLOSION_HPP
