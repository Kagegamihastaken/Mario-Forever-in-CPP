#include "Core/SimpleSprite.hpp"

namespace MFCPP {
    SimpleSprite::SimpleSprite() {
        m_texture = nullptr;
    }
    SimpleSprite::SimpleSprite(const sf::Texture* tex) : SimpleSprite(tex, sf::IntRect({0, 0}, sf::Vector2i(tex->getSize()))) {}
    SimpleSprite::SimpleSprite(const sf::Texture* tex, const sf::IntRect &rect) : m_texture(tex), m_rect(rect) {
        updateVertices();
    }
    void SimpleSprite::setTexture(const sf::Texture* tex, const bool resetRect) {
        if (resetRect)
            setTextureRect(sf::IntRect({0, 0}, sf::Vector2i(tex->getSize())));
        m_texture = tex;
    }
    const sf::Texture& SimpleSprite::getTexture() const {
        return *m_texture;
    }
    void SimpleSprite::setTextureRect(const sf::IntRect& rect) {
        if (rect != m_rect) {
            m_rect = rect;
            updateVertices();
        }
    }
    void SimpleSprite::setColor(const sf::Color& color) {
        for (auto &i : m_vertices)
            i.color = color;
    }
    sf::Color SimpleSprite::getColor() const {
        return m_vertices[0].color;
    }
    sf::FloatRect SimpleSprite::getLocalBounds() const {
        return {{0.f, 0.f}, m_vertices[5].position};
    }
    sf::FloatRect SimpleSprite::getGlobalBounds() const {
        return getTransform().transformRect(getLocalBounds());
    }
    void SimpleSprite::draw(sf::RenderTarget &target, sf::RenderStates states) const {

        states.transform *= getTransform();
        if (m_renderTexture) states.texture = m_texture;
        states.coordinateType = sf::CoordinateType::Pixels;
        //states.stencilMode = sf::StencilMode(sf::StencilComparison::Always, sf::StencilUpdateOperation::Replace, 1, 0xFF);
        target.draw(m_vertices.data(), m_vertices.size(), sf::PrimitiveType::Triangles, states);
    }
    sf::IntRect SimpleSprite::getTextureRect() const {
        return m_rect;
    }
    void SimpleSprite::updateVertices() {
        m_renderTexture = true;
        const auto [position, size] = sf::FloatRect(m_rect);

        const sf::Vector2f absSize(std::abs(size.x), std::abs(size.y));

        m_vertices[0].position = {0.f, 0.f};
        m_vertices[1].position = {0.f, absSize.y};
        m_vertices[2].position = {absSize.x, 0.f};
        m_vertices[3].position = {0.f, absSize.y};
        m_vertices[4].position = {absSize.x, 0.f};
        m_vertices[5].position = absSize;

        m_vertices[0].texCoords = position;
        m_vertices[1].texCoords = position + sf::Vector2f(0.f, size.y);
        m_vertices[2].texCoords = position + sf::Vector2f(size.x, 0.f);
        m_vertices[3].texCoords = position + sf::Vector2f(0.f, size.y);
        m_vertices[4].texCoords = position + sf::Vector2f(size.x, 0.f);
        m_vertices[5].texCoords = position + size;
    }
    void SimpleSprite::setRenderTexture(const bool val) {
        m_renderTexture = val;
    }
    bool SimpleSprite::getRenderTexture() const {
        return m_renderTexture;
    }


}