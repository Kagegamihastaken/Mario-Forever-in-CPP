#ifndef MFCPP_PAUSEENGINE_HPP
#define MFCPP_PAUSEENGINE_HPP

#include <optional>
#include "SFML/Window/Event.hpp"

namespace PauseEngine {
    static bool isPaused = false;
    static sf::VertexArray PauseBG(sf::PrimitiveType::TriangleStrip, 4);

    void InitPauseEngine();
    void PauseEvent(const std::optional<sf::Event>& event);
    void PauseDraw();
    [[nodiscard]] bool isGamePaused();
}

#endif //MFCPP_PAUSEENGINE_HPP
