#include <SFML/Audio.hpp>

#include "../headers/Core/Sound.hpp"
#include "../headers/Core/Loading/Loading.hpp"

#include "../resource.h"

sf::SoundBuffer jumpSoundBuffer;
sf::SoundBuffer BrickSoundBuffer;
sf::SoundBuffer BirckBreakSoundBuffer;
sf::SoundBuffer CoinSoundBuffer;
sf::SoundBuffer StompSoundBuffer;
sf::SoundBuffer PowerupSoundBuffer;
sf::SoundBuffer VineSoundBuffer;
sf::SoundBuffer Kick2SoundBuffer;
sf::SoundBuffer PipeSoundBuffer;

sf::Sound PipeSound;
sf::Sound Kick2Sound;
sf::Sound VineSound;
sf::Sound CoinSound;
sf::Sound BrickSound;
sf::Sound BrickBreakSound;
sf::Sound SoundJump;
sf::Sound StompSound;
sf::Sound Powerup;

int SoundInit() {
	LoadAudio(jumpSoundBuffer, JUMP_SOUND);
	SoundJump.setBuffer(jumpSoundBuffer);
	LoadAudio(BrickSoundBuffer, BUMP_SOUND);
	BrickSound.setBuffer(BrickSoundBuffer);
	LoadAudio(BirckBreakSoundBuffer, BREAK_SOUND);
	BrickBreakSound.setBuffer(BirckBreakSoundBuffer);
	LoadAudio(CoinSoundBuffer, COIN_SOUND);
	CoinSound.setBuffer(CoinSoundBuffer);
	LoadAudio(StompSoundBuffer, STOMP_SOUND);
	LoadAudio(PowerupSoundBuffer, POWERUP_SOUND);
	StompSound.setBuffer(StompSoundBuffer);
	Powerup.setBuffer(PowerupSoundBuffer);
	LoadAudio(VineSoundBuffer, VINE_SOUND);
	VineSound.setBuffer(VineSoundBuffer);
	LoadAudio(Kick2SoundBuffer, KICK2_SOUND);
	Kick2Sound.setBuffer(Kick2SoundBuffer);
	LoadAudio(PipeSoundBuffer, PIPE_SOUND);
	PipeSound.setBuffer(PipeSoundBuffer);
	return 6;
}
int iniSound = SoundInit();