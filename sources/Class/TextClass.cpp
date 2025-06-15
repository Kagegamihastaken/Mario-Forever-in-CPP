#include "Class/TextClass.hpp"

namespace MFCPP {
    void Text::setPosition(const sf::Vector2f& position) {
        m_Position = position;
    }
    sf::Vector2f Text::getPosition() const {
        return m_Position;
    }
    void Text::setID(const std::string& id) {
        m_Id = id;
    }
    std::string Text::getID() const {
        return m_Id;
    }
    void Text::setText(const std::string& text) {
        m_text = text;
    }
    std::string Text::getText() const {
        return m_text;
    }
    void Text::setMargin(TextMarginID margin) {
        m_Margin = margin;
    }
    TextMarginID Text::getMargin() const {
        return m_Margin;
    }
}