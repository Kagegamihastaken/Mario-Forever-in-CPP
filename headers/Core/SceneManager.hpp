#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include "Core/Scene.hpp"
#include <boost/unordered/unordered_flat_map.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class SceneManager {
public:
    enum class SceneState {
        GAMEPLAY,
        EDITOR
    };
    explicit SceneManager();
    template <typename T, typename... Args>
    void addScene(SceneState state, Args&&... args) {
        m_scenes[state] = std::make_unique<T>(*this, std::forward<Args>(args)...);
    }
    void changeScene(SceneState state);
    void handleInput(const std::optional<sf::Event>& event) const;
    void update(float deltaTime) const;
    void setPreviousPosition() const;
    void interpolatePosition(float alpha) const;
    void draw(sf::RenderWindow& window, float alpha) const;
    void objectCleanup() const;
    void postUpdate() const;
    void HUDPositionUpdate() const;
    void textUpdate() const;
    void setView() const;
    void unload();
private:
    boost::unordered::unordered_flat_map<SceneState, std::unique_ptr<Scene>> m_scenes;
    Scene* m_currentScene;
};

#endif //SCENEMANAGER_HPP
