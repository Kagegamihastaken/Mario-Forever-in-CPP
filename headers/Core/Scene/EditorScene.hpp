#ifndef EDITORSCENE_HPP
#define EDITORSCENE_HPP

#include "Core/Scene.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class EditorScene final :  public Scene {
public:
    explicit EditorScene(SceneManager& manager);
    void handleInput(const std::optional<sf::Event>& event) override;
    void update(float deltaTime) override;
    void setPreviousPosition() override;
    void interpolatePosition(float alpha) override;
    void draw(sf::RenderWindow& window) override;
    void objectCleanup() override;
    void postUpdate() override;
    void HUDPositionUpdate() override;
    void textUpdate() override;
    void setView() override;

    void loadResources() override;
    void unloadResources() override;
};

#endif //EDITORSCENE_HPP
