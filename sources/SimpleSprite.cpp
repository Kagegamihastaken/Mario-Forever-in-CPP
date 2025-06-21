#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include "Core/SimpleSprite.hpp"

namespace MFCPP {
    SimpleSprite::SimpleSprite() {
        //m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
        //m_vertices.resize(4);
        m_texture = nullptr;
    }
    SimpleSprite::SimpleSprite(const sf::Texture& tex) {
        //m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
        //m_vertices.resize(4);

        m_texture = &tex;
        const sf::Vector2u texSize = tex.getSize();

        m_vertices[0].texCoords = m_vertices[0].position = sf::Vector2f(0.f, 0.f);
        m_vertices[1].texCoords = m_vertices[1].position = sf::Vector2f(texSize.x, 0.f);
        m_vertices[2].texCoords = m_vertices[2].position = sf::Vector2f(0.f, texSize.y);
        m_vertices[3].texCoords = m_vertices[3].position = sf::Vector2f(texSize.x, texSize.y);
    }
    SimpleSprite::SimpleSprite(const sf::Texture& tex, const sf::IntRect &rect) {
        //m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
        //m_vertices.resize(4);

        m_texture = &tex;

        m_vertices[0].texCoords = m_vertices[0].position = sf::Vector2f(rect.position.x, rect.position.y);
        m_vertices[1].texCoords = m_vertices[1].position = sf::Vector2f(rect.position.x + rect.size.x, rect.position.y);
        m_vertices[2].texCoords = m_vertices[2].position = sf::Vector2f(rect.position.x, rect.position.y + rect.size.y);
        m_vertices[3].texCoords = m_vertices[3].position = sf::Vector2f(rect.position.x + rect.size.x, rect.position.y + rect.size.y);
    }
    void SimpleSprite::setTexture(const sf::Texture& tex, const bool resetRect) {
        m_texture = &tex;
        if (resetRect) {
            m_vertices[0].texCoords = m_vertices[0].position = sf::Vector2f(0.f, 0.f);
            m_vertices[1].texCoords = m_vertices[1].position = sf::Vector2f(tex.getSize().x, 0.f);
            m_vertices[2].texCoords = m_vertices[2].position = sf::Vector2f(0.f, tex.getSize().y);
            m_vertices[3].texCoords = m_vertices[3].position = sf::Vector2f(tex.getSize().x, tex.getSize().y);
        }
    }
    const sf::Texture& SimpleSprite::getTexture() const {
        return *m_texture;
    }
    void SimpleSprite::setTextureRect(const sf::IntRect& rect) {
        m_vertices[0].texCoords = m_vertices[0].position = sf::Vector2f(rect.position.x, rect.position.y);
        m_vertices[1].texCoords = m_vertices[1].position = sf::Vector2f(rect.position.x + rect.size.x, rect.position.y);
        m_vertices[2].texCoords = m_vertices[2].position = sf::Vector2f(rect.position.x, rect.position.y + rect.size.y);
        m_vertices[3].texCoords = m_vertices[3].position = sf::Vector2f(rect.position.x + rect.size.x, rect.position.y + rect.size.y);
    }
    void SimpleSprite::setColor(const sf::Color& color) {
        m_vertices[0].color = m_vertices[1].color = m_vertices[2].color = m_vertices[3].color = color;
    }
    sf::Color SimpleSprite::getColor() const {
        return m_vertices[0].color;
    }
    void SimpleSprite::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        states.transform *= getTransform();
        states.texture = m_texture;
        states.coordinateType = sf::CoordinateType::Pixels;
        target.draw(m_vertices.data(), m_vertices.size(), sf::PrimitiveType::TriangleStrip, states);
    }

}