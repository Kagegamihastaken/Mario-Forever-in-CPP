#ifndef SIMPLESPRITE_HPP
#define SIMPLESPRITE_HPP

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace MFCPP {
    class SimpleSprite : public sf::Drawable, public sf::Transformable {
    public:
        SimpleSprite();
        SimpleSprite(const sf::Texture& tex);
        SimpleSprite(const sf::Texture&& tex) = delete;
        SimpleSprite(const sf::Texture& tex, const sf::IntRect& rect);
        SimpleSprite(const sf::Texture&& tex, const sf::IntRect& rect) = delete;
        void setTexture(const sf::Texture&& tex, bool resetRect = false) = delete;
        void setTexture(const sf::Texture& tex, bool resetRect = false);
        void setTextureRect(const sf::IntRect& rect);
        const sf::Texture& getTexture() const;
        void setColor(const sf::Color& color);
        [[nodiscard]] sf::Color getColor() const;
    private:
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        sf::VertexArray m_vertices;
        sf::Texture     m_texture;
    };
}

#endif //SIMPLESPRITE_HPP
