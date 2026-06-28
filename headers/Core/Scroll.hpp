#pragma once

#ifndef SCROLL_HPP
#define SCROLL_HPP
#include "Class/CollisionObjectClass.hpp"

namespace Scroll {
    void Init();
    void setWindowView();
    void moveView(const sf::Vector2f& pos);
    void updateView();
    [[nodiscard]] bool isOutOfScreen(const MFCPP::CollisionObject& obj, const float offset);
    [[nodiscard]] bool isOutOfScreenYBottom(const MFCPP::CollisionObject& obj, const float offset);
    [[nodiscard]] bool isOutOfScreenXLeft(const MFCPP::CollisionObject& obj, const float offset);
    [[nodiscard]] bool isOutOfScreenXRight(const MFCPP::CollisionObject& obj, const float offset);

    [[nodiscard]] sf::View& getView();
    [[nodiscard]] const sf::Vector2f& getViewPosition();
    [[nodiscard]] const sf::Vector2f& getViewPositionOff();
};

#endif // SCROLL_HPP