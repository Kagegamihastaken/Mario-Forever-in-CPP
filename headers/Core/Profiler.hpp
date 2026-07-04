#pragma once
#ifndef MFCPP_PROFILER_HPP
#define MFCPP_PROFILER_HPP

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#else
#define ZoneScoped
#define ZoneScopedN(x)
#define ZoneScopedNC(x, y)
#define ZoneName(x, y)
#define FrameMark
#define FrameMarkNamed(x)
#endif // TRACY_ENABLE
#endif //MFCPP_PROFILER_HPP
