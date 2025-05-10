#include <SFML/Audio.hpp>

#include "../headers/Core/SoundManager.hpp"
#include "../headers/Core/Sound.hpp"
#include "../headers/Core/Loading/Loading.hpp"

#include "../resource.h"

SoundManager Sounds;
void SoundAdd(std::string name, std::string path) {
	sf::SoundBuffer BufferTemp;
	LoadAudio(BufferTemp, path);
	Sounds.AddSound(name, BufferTemp);
}
void SoundInit() {
	SoundAdd("Jump", "data/sounds/jump.wav");
	SoundAdd("Bump", "data/sounds/bump.wav");
	SoundAdd("Break", "data/sounds/break.wav");
	SoundAdd("Coin", "data/sounds/coin.wav");
	SoundAdd("Stomp", "data/sounds/stomp.wav");
	SoundAdd("Powerup", "data/sounds/powerup.wav");
	SoundAdd("Vine", "data/sounds/vine.wav");
	SoundAdd("Kick2", "data/sounds/kick2.wav");
	SoundAdd("Pipe", "data/sounds/pipe.wav");
	SoundAdd("Kick3", "data/sounds/kick3.wav");
	SoundAdd("Kick4", "data/sounds/kick4.wav");
	SoundAdd("Kick5", "data/sounds/kick5.wav");
	SoundAdd("Kick6", "data/sounds/kick6.wav");
	SoundAdd("Kick7", "data/sounds/kick7.wav");
	SoundAdd("Kick8", "data/sounds/kick8.wav");
	SoundAdd("1UP", "data/sounds/1up.wav");
}