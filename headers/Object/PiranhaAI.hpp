#ifndef PIRANHAAI_HPP
#define PIRANHAAI_HPP

#include "Core/Class/PiranhaAIClass.hpp"
#include "Core/Loading/enum.hpp"

extern void AddPiranha(PiranhaID ID, PiranhaDirection dir, float x, float y);
extern void PiranhaAIInit();

extern std::vector<std::string> GreenPiranhaUpAnimName;
extern std::vector<std::string> GreenPiranhaDownAnimName;
extern std::vector<std::string> GreenPiranhaRightAnimName;
extern std::vector<std::string> GreenPiranhaLeftAnimName;

extern std::vector<std::string> PinkPiranhaUpAnimName;
extern std::vector<std::string> PinkPiranhaDownAnimName;
extern std::vector<std::string> PinkPiranhaRightAnimName;
extern std::vector<std::string> PinkPiranhaLeftAnimName;

#endif