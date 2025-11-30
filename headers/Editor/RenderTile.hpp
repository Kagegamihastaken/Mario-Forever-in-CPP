#ifndef RENDERTILE_HPP
#define RENDERTILE_HPP
#include "SelectTile.hpp"

class RenderTile final : public sf::Drawable, public sf::Transformable {
public:
    RenderTile(const CustomTileProperty &props, const sf::Texture& texture, const sf::Vector2f& position, int page = 0, int id = 0, const sf::Vector2f& endPos = sf::Vector2f(-1.f, -1.f));
    RenderTile();
    RenderTile(const sf::Vector2f& position, int page);
    void setTextureRect(const sf::IntRect& rect, bool resetSize = false);
    int getID() const;
    int getPage() const;
    sf::Vector2f getEndPos() const;
    const sf::Texture* getTexture() const;
    CustomTileProperty& getProperty();
    const CustomTileProperty& getProperty() const;
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::VertexArray m_vertices;
    const sf::Texture *m_texture;
    sf::Vector2f    m_position;
    sf::Vector2f    m_endPos;
    int             m_page;
    int             m_id;
    CustomTileProperty m_props;
};

#endif