#include <SFML/Graphics.hpp>
#include "Editor/RenderTile.hpp"

RenderTile::RenderTile(const sf::Vector2f &position) {
    m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
    m_vertices.resize(4);
    setPosition(position);
    m_type = 0;
}

RenderTile::RenderTile(const sf::Texture& texture, const sf::Vector2f& position, const sf::IntRect& rect) {
    m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
    m_vertices.resize(4);
    setPosition(position);
    m_texture = texture;
    m_type = 0;
    m_vertices[0].position = sf::Vector2f(0, 0);
    m_vertices[1].position = sf::Vector2f(rect.size.x, 0);
    m_vertices[2].position = sf::Vector2f(0, rect.size.y);
    m_vertices[3].position = sf::Vector2f(rect.size.x, rect.size.y);

    m_vertices[0].texCoords = sf::Vector2f(rect.position.x, rect.position.y);
    m_vertices[1].texCoords = sf::Vector2f(rect.position.x + rect.size.x, rect.position.y);
    m_vertices[2].texCoords = sf::Vector2f(rect.position.x, rect.position.y + rect.size.y);
    m_vertices[3].texCoords = sf::Vector2f(rect.position.x + rect.size.x, rect.position.y + rect.size.y);
}
RenderTile::RenderTile(const sf::Texture& texture, const sf::IntRect& rect) {
    m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
    m_vertices.resize(4);
    m_texture = texture;
    m_type = 0;
    m_vertices[0].position = sf::Vector2f(0, 0);
    m_vertices[1].position = sf::Vector2f(rect.size.x, 0);
    m_vertices[2].position = sf::Vector2f(0, rect.size.y);
    m_vertices[3].position = sf::Vector2f(rect.size.x, rect.size.y);

    m_vertices[0].texCoords = sf::Vector2f(rect.position.x, rect.position.y);
    m_vertices[1].texCoords = sf::Vector2f(rect.position.x + rect.size.x, rect.position.y);
    m_vertices[2].texCoords = sf::Vector2f(rect.position.x, rect.position.y + rect.size.y);
    m_vertices[3].texCoords = sf::Vector2f(rect.position.x + rect.size.x, rect.position.y + rect.size.y);
}
RenderTile::RenderTile(const sf::Texture& texture, const sf::Vector2f& position, const int type) {
    m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
    m_vertices.resize(4);
    setPosition(position);
    m_texture = texture;
    m_type = type;
    m_vertices[0].position = sf::Vector2f(0, 0);
    m_vertices[1].position = sf::Vector2f(m_texture.getSize().x, 0);
    m_vertices[2].position = sf::Vector2f(0, m_texture.getSize().y);
    m_vertices[3].position = sf::Vector2f(m_texture.getSize().x, m_texture.getSize().y);

    m_vertices[0].texCoords = sf::Vector2f(0, 0);
    m_vertices[1].texCoords = sf::Vector2f(m_texture.getSize().x, 0);
    m_vertices[2].texCoords = sf::Vector2f(0, m_texture.getSize().y);
    m_vertices[3].texCoords = sf::Vector2f(m_texture.getSize().x, m_texture.getSize().y);
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
void RenderTile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = &m_texture;
    target.draw(m_vertices, states);
}