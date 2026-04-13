#pragma once
#ifndef PLATFORM_HPP
#define PLATFORM_HPP
extern void PlatformInit();
extern void AddPlatform(const sf::Vector2f& start, const sf::Vector2f& end, int skinID, float speed, bool smooth, bool fall, bool wait, bool small);

#endif //PLATFORM_HPP
