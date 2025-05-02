#ifndef SPIKE_HPP
#define SPIKE_HPP

#include "../Core/Loading/enum.hpp"
#include "../Core/Animate/LocalAnimationManager.hpp"

#include <vector>
#include <string>

extern std::vector<sf::Sprite> SpikeList;
extern std::vector<SpikeID> SpikeIDList;
extern std::vector<LocalAnimationManager> SpikeAnimationList;

extern void DeleteAllSpike();
extern void DeleteSpike(float x, float y);
extern void SpikeInit();
extern void AddSpike(SpikeID ID, float x, float y);
extern void SpikeUpdate();
extern void SpikeStatusUpdate();

#endif