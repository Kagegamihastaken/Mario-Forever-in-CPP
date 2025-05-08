#include <SFML/Audio.hpp>

#include "../headers/Core/SoundManager.hpp"
#include "../headers/Core/Sound.hpp"
#include "../headers/Core/Loading/Loading.hpp"

#include "../resource.h"

SoundManager Sounds;
void SoundAdd(std::string name, int resID) {
	sf::SoundBuffer BufferTemp;
	LoadAudio(BufferTemp, resID);
	Sounds.AddSound(name, BufferTemp);
}
void SoundInit() {
	SoundAdd("Jump", JUMP_SOUND);
	SoundAdd("Bump", BUMP_SOUND);
	SoundAdd("Break", BREAK_SOUND);
	SoundAdd("Coin", COIN_SOUND);
	SoundAdd("Stomp", STOMP_SOUND);
	SoundAdd("Powerup", POWERUP_SOUND);
	SoundAdd("Vine", VINE_SOUND);
	SoundAdd("Kick2", KICK2_SOUND);
	SoundAdd("Pipe", PIPE_SOUND);
	SoundAdd("Kick3", KICK3_SOUND);
	SoundAdd("Kick4", KICK4_SOUND);
	SoundAdd("Kick5", KICK5_SOUND);
	SoundAdd("Kick6", KICK6_SOUND);
	SoundAdd("Kick7", KICK7_SOUND);
	SoundAdd("Kick8", KICK8_SOUND);
	SoundAdd("1UP", ONEUP_SOUND);
}