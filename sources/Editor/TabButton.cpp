#include "Editor/TabButton.hpp"

namespace MFCPP {
    TabButton::TabButton() {
        m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
        m_vertices.resize(4);
    }
    bool TabButton::isMouseHovered(const sf::Vector2f& pos, const sf::Vector2f& mouse) const {
        if (mouse.x >= getPosition().x - pos.x && mouse.x <= getPosition().x + m_texture.getSize().x - pos.x
            && mouse.y >= getPosition().y - pos.y && mouse.y <= getPosition().y + m_texture.getSize().y - pos.y) {
            return true;
            }
        return false;
    }
    void TabButton::setTexture(const sf::Texture& tex) {
        m_vertices[0].texCoords = m_vertices[0].position = sf::Vector2f(0.f, 0.f);
        m_vertices[1].texCoords = m_vertices[1].position = sf::Vector2f(tex.getSize().x, 0.f);
        m_vertices[2].texCoords = m_vertices[2].position = sf::Vector2f(0.f, tex.getSize().y);
        m_vertices[3].texCoords = m_vertices[3].position = sf::Vector2f(tex.getSize().x, tex.getSize().y);

        m_texture = tex;
    }
    void TabButton::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();
        states.texture = &m_texture;
        target.draw(m_vertices, states);
    }

}
