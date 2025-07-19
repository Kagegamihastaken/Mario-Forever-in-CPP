#include "Editor/RenderTile.hpp"
#include "Editor/SelectTile.hpp"

RenderTile::RenderTile(const sf::Vector2f &position): m_props(CustomTileProperty()) {
    m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
    m_vertices.resize(4);
    setPosition(position);
    m_page = 0;
    m_id = 0;
}

RenderTile::RenderTile(): m_props(CustomTileProperty()) {
    m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
    m_vertices.resize(4);
    m_page = 0;
    m_id = 0;
}

RenderTile::RenderTile(const CustomTileProperty &props, const sf::Texture& texture, const sf::Vector2f& position, const int page, const int id, const sf::Vector2f& endPos)
    : m_endPos(endPos), m_props(props){
    m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
    m_vertices.resize(4);
    setPosition(position);
    m_texture = &texture;
    m_page = page;
    m_id = id;
    m_vertices[0].position = sf::Vector2f(0, 0);
    m_vertices[1].position = sf::Vector2f(m_texture->getSize().x, 0);
    m_vertices[2].position = sf::Vector2f(0, m_texture->getSize().y);
    m_vertices[3].position = sf::Vector2f(m_texture->getSize().x, m_texture->getSize().y);

    m_vertices[0].texCoords = sf::Vector2f(0, 0);
    m_vertices[1].texCoords = sf::Vector2f(m_texture->getSize().x, 0);
    m_vertices[2].texCoords = sf::Vector2f(0, m_texture->getSize().y);
    m_vertices[3].texCoords = sf::Vector2f(m_texture->getSize().x, m_texture->getSize().y);
}
void RenderTile::setTextureRect(const sf::IntRect& rect, const bool resetSize) {
    m_vertices[0].texCoords = sf::Vector2f(rect.position.x, rect.position.y);
    m_vertices[1].texCoords = sf::Vector2f(rect.position.x + rect.size.x, rect.position.y);
    m_vertices[2].texCoords = sf::Vector2f(rect.position.x, rect.position.y + rect.size.y);
    m_vertices[3].texCoords = sf::Vector2f(rect.position.x + rect.size.x, rect.position.y + rect.size.y);

    if (resetSize) {
        m_vertices[0].position = sf::Vector2f(0, 0);
        m_vertices[1].position = sf::Vector2f(rect.size.x, 0);
        m_vertices[2].position = sf::Vector2f(0, rect.size.y);
        m_vertices[3].position = sf::Vector2f(rect.size.x, rect.size.y);
    }
}
int RenderTile::getID() const {
    return m_id;
}
int RenderTile::getPage() const {
    return m_page;
}
sf::Vector2f RenderTile::getEndPos() const {
    return m_endPos;
}
const sf::Texture* RenderTile::getTexture() const {
    return m_texture;
}
CustomTileProperty& RenderTile::getProperty() {
    return m_props;
}
const CustomTileProperty& RenderTile::getProperty() const {
    return m_props;
}
void RenderTile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = m_texture;
    target.draw(m_vertices, states);
}