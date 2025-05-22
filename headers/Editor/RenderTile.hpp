#ifndef RENDERTILE_HPP
#define RENDERTILE_HPP
#include <SFML/Graphics.hpp>

class RenderTile : public sf::Drawable, public sf::Transformable {
public:
    RenderTile(const sf::Texture& texture);
    RenderTile(const sf::Texture& texture, const sf::IntRect& rect);
    RenderTile(const sf::Texture& texture, const sf::Vector2f& position);
    RenderTile(const sf::Texture& texture, const sf::Vector2f& position, const sf::IntRect& rect);
    RenderTile(const sf::Vector2f& position);
    void setTextureRect(const sf::IntRect& rect, bool resetSize = false);
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::VertexArray m_vertices;
    sf::Texture     m_texture;
};

#endif