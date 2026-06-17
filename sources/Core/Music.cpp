#include "Core/Music.hpp"
#include "Core/MusicManager.hpp"

void MusicInit() {
	MusicManager::AddMusic("night", "data/musics/night.ogg");
	MusicManager::AddMusic("yky", "data/musics/yky.ogg");
	MusicManager::AddMusic("underground", "data/musics/underground1.ogg");
	MusicManager::AddMusic("apoplexy", "data/musics/apoplexy.ogg");
	MusicManager::AddMusic("MarioDeath", "data/musics/1-Music-Die.ogg");
	//MusicManager::AddMusic("MainMusicOGG", "data/musics/The_Worst_Goodbye.ogg");
	MusicManager::AddMusic("default", "data/musics/Rigit_Medium.ogg");
	MusicManager::AddMusic("LevelComplete", "data/musics/Level_Complete.ogg");
}