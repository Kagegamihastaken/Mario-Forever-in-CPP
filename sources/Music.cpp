#include "Core/Music.hpp"
#include "Core/MusicManager.hpp"

MusicManager Music;

void MusicInit() {
	Music.AddMODMusic("MainMusicMOD", "musics/credits.xm");
	Music.AddMODMusic("MarioDeath", "musics/1-Music-Die.it");
	Music.AddOGGMusic("MainMusicOGG", "musics/The_Worst_Goodbye.ogg");
	Music.AddOGGMusic("RigitMedium", "musics/Rigit_Medium.ogg");
	Music.AddOGGMusic("LevelComplete", "musics/Level_Complete.ogg");
}