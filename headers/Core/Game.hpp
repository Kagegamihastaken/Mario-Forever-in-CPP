#ifndef GAME_HPP
#define GAME_HPP
#include "SceneManager.hpp"

class Game {
public:
    Game() = delete;
    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = delete;

    static void Init();
    static void EditText();
    static void SetPrev();
    static void DeltaMovement(float dt);
    static void InterpolateMovement(float alpha);
    static void Collision();
    static void MiscUpdate();
    static void Draw();
    static void RetrieveEvent(const std::optional<sf::Event>& event);
    static void Cleanup();
    static void UpdateView();
private:
    static SceneManager sceneManager;
};

#endif //GAME_HPP
