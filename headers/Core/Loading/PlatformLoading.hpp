#pragma once
#ifndef PLATFORM_LOADING_HPP
#define PLATFORM_LOADING_HPP

namespace MFCPP {
    void AddPlatform(const sf::Vector2f& start, const sf::Vector2f& end, int skinID, float speed, bool smooth, bool fall, bool wait, bool small);
}

#endif //PLATFORM_LOADING_HPP
