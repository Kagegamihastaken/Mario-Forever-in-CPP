#ifndef TEXTCLASS_HPP
#define TEXTCLASS_HPP

#include "Core/Loading/enum.hpp"
#include "Text/Font.hpp"

namespace MFCPP {
    class Text {
        sf::Vector2f m_Position{};
        std::string m_text{};
        std::string m_Id{};
        TextMarginID m_Margin{};
    public:
        std::vector<sf::Sprite> m_SpriteText{};
        explicit Text() = default;
        void setPosition(const sf::Vector2f& position);
        [[nodiscard]] sf::Vector2f getPosition() const;
        void setID(const std::string& id);
        [[nodiscard]] std::string getID() const;
        void setText(const std::string& text);
        [[nodiscard]] std::string getText() const;
        void setMargin(TextMarginID margin);
        [[nodiscard]] TextMarginID getMargin() const;
    };

    class TextList final : public Font {
    public:
        explicit TextList() = default;
        std::vector<Text> List{};
    };
}

#endif //TEXTCLASS_HPP
