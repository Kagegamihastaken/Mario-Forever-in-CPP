#include "Core/Input.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "Core/Profiler.hpp"
#include "Core/WindowFrame.hpp"
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>

void MFCPP::Input::inputHandling() {
    while (WindowFrame::running) {
        ZoneScopedNC("Input::inputHandling", 0xffeeff);
        if (WindowFrame::getWindow().hasFocus()) {
            up = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
            down = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
            left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
            right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
            jump = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z);
            fire = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X);
        }
        sf::sleep(sf::milliseconds(1));
    }
}

std::atomic<bool>& MFCPP::Input::isJumpKeyPressed() {
    return jump;
}

std::atomic<bool> &MFCPP::Input::isLeftKeyPressed() {
    return left;
}

std::atomic<bool> &MFCPP::Input::isRightKeyPressed() {
    return right;
}
std::atomic<bool>& MFCPP::Input::isDownKeyPressed() {
    return down;
}

std::atomic<bool> &MFCPP::Input::isUpKeyPressed() {
    return up;
}

std::atomic<bool> &MFCPP::Input::isFireKeyPressed() {
    return fire;
}
