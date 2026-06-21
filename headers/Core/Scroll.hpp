#pragma once

#ifndef SCROLL_HPP
#define SCROLL_HPP
#include "Class/CollisionObjectClass.hpp"

class Scroll final {
public:
    Scroll() = delete;
    Scroll(const Scroll&) = delete;
    Scroll(Scroll&&) = delete;
    Scroll& operator=(const Scroll&) = delete;
    Scroll& operator=(Scroll&&) = delete;

    static void Init();
    static void setWindowView();
    static void moveView(const sf::Vector2f& pos);
    static void updateView();
    [[nodiscard]] static bool isOutOfScreen(const MFCPP::CollisionObject& obj, const float offset);
    [[nodiscard]] static bool isOutOfScreenYBottom(const MFCPP::CollisionObject& obj, const float offset);
    [[nodiscard]] static bool isOutOfScreenXLeft(const MFCPP::CollisionObject& obj, const float offset);
    [[nodiscard]] static bool isOutOfScreenXRight(const MFCPP::CollisionObject& obj, const float offset);

    [[nodiscard]] static sf::View& getView();
    [[nodiscard]] static const sf::Vector2f& getViewPosition();
    [[nodiscard]] static const sf::Vector2f& getViewPositionOff();
private:
    static sf::View View;
    static sf::Vector2f ViewPosition;
    static sf::Vector2f ViewPositionOff;

    static void setLetterboxView(sf::View& viewVal, const sf::Vector2u& windowSize);
};

#endif // SCROLL_HPP