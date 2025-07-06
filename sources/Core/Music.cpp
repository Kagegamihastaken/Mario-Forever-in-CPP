#include "Core/Music.hpp"
#include "Core/MusicManager.hpp"

void MusicInit() {
	//MusicManager::AddMusic("DansLaRue", "data/musics/Rigit Medium.mp3");
	MusicManager::AddMusic("DansLaRue", "data/musics/xyce-dans_la_rue.ogg");
	MusicManager::AddMusic("MarioDeath", "data/musics/1-Music-Die.it");
	//MusicManager::AddMusic("MainMusicOGG", "data/musics/The_Worst_Goodbye.ogg");
	//MusicManager::AddMusic("RigitMedium", "data/musics/Rigit_Medium.ogg");
	MusicManager::AddMusic("LevelComplete", "data/musics/Level_Complete.ogg");
}