#include "Core/Music.hpp"
#include "Core/MusicManager.hpp"

MusicManager Music;

void MusicInit() {
	MusicManager::AddMODMusic("DansLaRue", "musics/xyce-dans_la_rue.xm");
	MusicManager::SetMODMusicVolume("DansLaRue", 1000);
	MusicManager::AddOGGMusic("DansLaRueOGG", "musics/xyce-dans_la_rue.ogg");

	MusicManager::AddMODMusic("MarioDeath", "musics/1-Music-Die.it");
	MusicManager::AddOGGMusic("MainMusicOGG", "musics/The_Worst_Goodbye.ogg");
	MusicManager::AddOGGMusic("RigitMedium", "musics/Rigit_Medium.ogg");
	MusicManager::AddOGGMusic("LevelComplete", "musics/Level_Complete.ogg");
}