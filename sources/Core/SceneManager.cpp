#include "Core/SceneManager.hpp"

#include "Core/Logging.hpp"

SceneManager::SceneManager() : m_currentScene(nullptr) {}
void SceneManager::changeScene(SceneState state) {
    if (m_currentScene) {
        m_currentScene->unloadResources();
    }

    if (const auto it = m_scenes.find(state); it != m_scenes.end()) {
        m_currentScene = it->second.get();
        m_currentScene->loadResources();
    }
    else {
        m_currentScene = nullptr;
        MFCPP::Log::ErrorPrint("SceneManager: An Error Occurred While Changing Scene");
    }
}
void SceneManager::handleInput(const std::optional<sf::Event>& event) const {
    if (m_currentScene) {
        m_currentScene->handleInput(event);
    }
}
void SceneManager::update(float deltaTime) const {
    if (m_currentScene) {
        m_currentScene->update(deltaTime);
    }
}
void SceneManager::draw(sf::RenderWindow &window, float alpha) const {
    if (m_currentScene) {
        m_currentScene->draw(window, alpha);
    }
}
void SceneManager::setPreviousPosition() const {
    if (m_currentScene) {
        m_currentScene->setPreviousPosition();
    }
}
void SceneManager::interpolatePosition(float alpha) const {
    if (m_currentScene) {
        m_currentScene->interpolatePosition(alpha);
    }
}
void SceneManager::objectCleanup() const {
    if (m_currentScene) {
        m_currentScene->objectCleanup();
    }
}
void SceneManager::postUpdate() const {
    if (m_currentScene) {
        m_currentScene->postUpdate();
    }
}
void SceneManager::HUDPositionUpdate() const {
    if (m_currentScene) {
        m_currentScene->HUDPositionUpdate();
    }
}
void SceneManager::textUpdate() const {
    if (m_currentScene) {
        m_currentScene->textUpdate();
    }
}
void SceneManager::setView() const {
    if (m_currentScene) {
        m_currentScene->setView();
    }
}

void SceneManager::unload() {
    // Later
}







