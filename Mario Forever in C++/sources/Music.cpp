#include "../headers/Core/Music.hpp"
#include "../headers/Core/MusicManager.hpp"

#include "../resource.h"

MusicManager Music;

void MusicInit() {
	Music.AddMODMusic("MainMusicMOD", TEST_MUSIC, 32);
	Music.AddOGGMusic("MainMusicOGG", TEST_OGG);
	Music.AddMODMusic("MarioDeath", DEATH_MUSIC, 31);
}