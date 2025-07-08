#ifndef SPIKE_HPP
#define SPIKE_HPP

#include "Core/Loading/enum.hpp"
#include "Core/Class/SpikeClass.hpp"
#include "Core/ExternalHeaders/plf_colony.h"

extern plf::colony<MFCPP::Spike> SpikeList;

extern void DeleteAllSpike();
extern void DeleteSpike(float x, float y);
extern void SpikeInit();
extern void AddSpike(SpikeID ID, float x, float y);
extern void SpikeDraw();
extern void SpikeStatusUpdate();
extern void SpikeCleanup();
#endif