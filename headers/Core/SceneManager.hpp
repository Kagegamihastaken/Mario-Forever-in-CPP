#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include "Core/Scene.hpp"
#include <string>
#include <unordered_map>
#include <SFML/Graphics/RenderWindow.hpp>

class SceneManager {
public:
    SceneManager();
    template <typename T, typename... Args>
    void addScene(const std::string& name, Args&&... args) {
        m_scenes[name] = std::make_unique<T>(*this, std::forward<Args>(args)...);
    }
    void changeScene(const std::string& name);
    void handleInput(const std::optional<sf::Event>& event) const;
    void update(float deltaTime) const;
    //Interpolate position
    void setPreviousPosition() const;
    void interpolatePosition(float alpha) const;
    //Draw Scene
    void draw(sf::RenderWindow& window) const;
    void objectCleanup() const;
    void postUpdate() const;
    void HUDPositionUpdate() const;
    void textUpdate() const;
    void setView() const;
private:
    std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;
    Scene* m_currentScene;
};

#endif //SCENEMANAGER_HPP
