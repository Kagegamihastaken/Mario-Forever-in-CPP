#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_set>
#include "Editor/Editor.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Hash.hpp"
#include "Core/ImageManager.hpp"
#include "Block/Obstacles.hpp"
#include "Core/Interpolation.hpp"

float lastPlaceX = -1.0f;
float lastPlaceY = -1.0f;
float lastDeleteX = -1.0f;
float lastDeleteY = -1.0f;

sf::Vector2f EditorPos;
sf::Vector2f EditorPrevPos;
sf::Vector2f EditorInterpolatedPos;

std::unordered_set<RenderTile, RenderTileHash, RenderTileEqual> Tile;
void SetPrevEditor() {
    EditorPrevPos = EditorPos;
}
void InterpolateEditorPos(const float alpha) {
    EditorInterpolatedPos = linearInterpolation(EditorPrevPos, EditorPos, alpha);
}
void PlaceTile() {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && window.hasFocus()) {
            if (((lastDeleteX != std::floor((MouseX + EditorPos.x) / 32.0f) * 32.0f) || (lastDeleteY != std::floor(MouseY/ 32.0f) * 32.0f)) || ((lastPlaceX == std::floor((MouseX + EditorPos.x) / 32.0f) * 32.0f) && (lastPlaceY == std::floor(MouseY / 32.0f) * 32.0f))) {
                if (Tile.contains(sf::Vector2f(std::floor((MouseX + EditorPos.x) / 32.0f) * 32.0f, std::floor(MouseY / 32.0f) * 32.0f))) {
                    std::cout << "Found!\n";
                    Tile.erase(sf::Vector2f(std::floor((MouseX + EditorPos.x) / 32.0f) * 32.0f, std::floor(MouseY / 32.0f) * 32.0f));
                }
                else std::cout << "Not Found!\n";
                lastDeleteX = std::floor((MouseX + EditorPos.x) / 32.0f) * 32.0f;
                lastDeleteY = std::floor(MouseY / 32.0f) * 32.0f;
                lastPlaceX = -1.0f;
                lastPlaceY = -1.0f;
            }
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)&& window.hasFocus()) {
            if (((lastPlaceX != std::floor((MouseX + EditorPos.x) / 32.0f) * 32.0f) || (lastPlaceY != std::floor(MouseY / 32.0f) * 32.0f)) || ((lastDeleteX == std::floor((MouseX + EditorPos.x) / 32.0f) * 32.0f) && (lastDeleteY == std::floor(MouseY / 32.0f) * 32.0f))) {
                if (!Tile.contains(sf::Vector2f(std::floor((MouseX + EditorPos.x) / 32.0f) * 32.0f, std::floor(MouseY / 32.0f) * 32.0f))) {
                    std::cout << "Placed\n";
                    Tile.insert(RenderTile(ImageManager::GetTexture("Tile_6"), sf::Vector2f({std::floor((MouseX + EditorPos.x) / 32.0f) * 32.0f, std::floor(MouseY / 32.0f) * 32.0f})));
                }
                else std::cout << "Already Placed\n";
                lastPlaceX = std::floor((MouseX + EditorPos.x) / 32.0f) * 32.0f;
                lastPlaceY = std::floor(MouseY / 32.0f) * 32.0f;
                lastDeleteX = -1.0f;
                lastDeleteY = -1.0f;
            }
        }
}

void EditorScreenMove(const float dt) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) EditorPos.x += 7.5f * dt;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) EditorPos.x -= 7.5f * dt;
    if (EditorPos.x <= 0.0f) EditorPos.x = 0.0f;
}

void DrawTile() {
    for (const auto &i : Tile) {
        window.draw(i);
    }
}
