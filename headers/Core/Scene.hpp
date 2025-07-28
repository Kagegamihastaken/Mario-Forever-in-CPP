#ifndef SCENE_HPP
#define SCENE_HPP

class SceneManager;

class Scene {
public:
    explicit Scene(SceneManager& manager) : m_sceneManager(manager) {};

    virtual void handleInput(const std::optional<sf::Event>& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void setPreviousPosition() = 0;
    virtual void interpolatePosition(float alpha) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void postUpdate() = 0;
    virtual void objectCleanup() = 0;
    virtual void HUDPositionUpdate() = 0;
    virtual void textUpdate() = 0;
    virtual void setView() = 0;

    virtual void loadResources() = 0;
    virtual void unloadResources() = 0;

    virtual ~Scene() = default;
protected:
    SceneManager& m_sceneManager;
};

#endif //SCENE_HPP
