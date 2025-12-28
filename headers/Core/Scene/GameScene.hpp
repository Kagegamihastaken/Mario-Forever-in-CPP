#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include "Core/Scene.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "Core/Object/EnemyManager.hpp"

class GameScene final : public Scene {
public:
    static EnemyManager enemyManager;

    explicit GameScene(SceneManager& manager);
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

#endif //GAMESCENE_HPP
