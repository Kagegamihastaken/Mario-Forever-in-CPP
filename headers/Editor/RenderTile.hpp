#ifndef RENDERTILE_HPP
#define RENDERTILE_HPP

class RenderTile final : public sf::Drawable, public sf::Transformable {
public:
    RenderTile(const sf::Texture& texture, const sf::IntRect& rect);
    RenderTile(const sf::Texture& texture, const sf::Vector2f& position, int page = 0, int id = 0);
    RenderTile(const sf::Texture& texture, const sf::Vector2f& position, const sf::IntRect& rect);
    RenderTile(const sf::Vector2f& position);
    void setTextureRect(const sf::IntRect& rect, bool resetSize = false);
    int getID() const;
    int getPage() const;
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::VertexArray m_vertices;
    sf::Texture     m_texture;
    sf::Vector2f    m_position;
    int             m_page;
    int             m_id;
};

#endif