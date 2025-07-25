#ifndef SIMPLESPRITE_HPP
#define SIMPLESPRITE_HPP

namespace MFCPP {
    class SimpleSprite : public sf::Drawable, public sf::Transformable {
    public:
        explicit SimpleSprite();
        explicit SimpleSprite(const sf::Texture* tex);
        explicit SimpleSprite(const sf::Texture&& tex) = delete;
        explicit SimpleSprite(const sf::Texture* tex, const sf::IntRect& rect);
        explicit SimpleSprite(const sf::Texture&& tex, const sf::IntRect& rect) = delete;
        void setTexture(const sf::Texture&& tex, bool resetRect = false) = delete;
        void setTexture(const sf::Texture* tex, bool resetRect = false);
        void setTextureRect(const sf::IntRect& rect);
        const sf::Texture& getTexture() const;
        void setColor(const sf::Color& color);
        [[nodiscard]] sf::Color getColor() const;
        [[nodiscard]] sf::IntRect getTextureRect() const;
        [[nodiscard]] sf::FloatRect getLocalBounds() const;
        [[nodiscard]] sf::FloatRect getGlobalBounds() const;
        void setRenderTexture(bool val);
        [[nodiscard]] bool getRenderTexture() const;
    private:
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
        void updateVertices();

        std::array<sf::Vertex, 6> m_vertices;
        const sf::Texture*        m_texture;
        sf::IntRect               m_rect;
        bool                      m_renderTexture;
    };
}

#endif //SIMPLESPRITE_HPP
