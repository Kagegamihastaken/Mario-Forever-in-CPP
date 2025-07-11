#ifndef TABBUTTON_HPP
#define TABBUTTON_HPP

namespace MFCPP {
    class TabButton final : public sf::Drawable, public sf::Transformable {
    public:
        TabButton();
        void setTexture(const sf::Texture& tex);
        bool isMouseHovered(const sf::Vector2f& pos, const sf::Vector2f& mouse) const;
    private:
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        sf::VertexArray m_vertices;
        sf::Texture     m_texture;
    };
}

#endif //TABBUTTON_HPP
