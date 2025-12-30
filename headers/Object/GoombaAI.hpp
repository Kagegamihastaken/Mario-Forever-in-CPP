#pragma once

#ifndef GOOMBAAI_HPP
#define GOOMBAAI_HPP

#include "Core/Loading/enum.hpp"

extern void GoombaAIInit();
extern void AddGoombaAI(GoombaAIType type, int SkinID, float x, float y, GoombaAIDirection Dir);

extern std::vector<std::string> GoombaAnimName;
extern std::vector<std::string> GreenKoopaAnimName;
extern std::vector<std::string> GreenKoopaShellAnimName;
extern std::vector<std::string> FireFlowerAnimName;
extern std::vector<std::string> RedKoopaAnimName;
extern std::vector<std::string> RedKoopaShellAnimName;
extern std::vector<std::string> RedSpinyAnimName;
extern std::vector<std::string> GreenSpinyAnimName;
#endif // GOOMBAAI_HPP