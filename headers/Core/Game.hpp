#ifndef GAME_HPP
#define GAME_HPP
#include "SceneManager.hpp"

namespace Game {
    void Init();
    void EditText();
    void SetPrev();
    void DeltaMovement(float dt);
    void InterpolateMovement(float alpha);
    void Collision();
    void MiscUpdate();
    void Draw(float alpha);
    void RetrieveEvent(const std::optional<sf::Event>& event);
    void Cleanup();
    void UpdateView();
    void Deinit();
};

#endif //GAME_HPP
