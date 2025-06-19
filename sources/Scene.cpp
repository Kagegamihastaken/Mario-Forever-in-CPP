#include "Core/Scene.hpp"
#include "Core/Loading/enum.hpp"

SceneID CurrentScene = SceneID::SCENE_GAMEPLAY;
void SetScene(const SceneID scene) {
    CurrentScene = scene;
}