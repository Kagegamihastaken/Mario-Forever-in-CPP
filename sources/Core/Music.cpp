#include "Core/Music.hpp"
#include "Core/MusicManager.hpp"

void MusicInit() {
	MusicManager::AddMusic(MusicID::NIGHT, "data/musics/night.ogg");
	MusicManager::AddMusic(MusicID::YKY, "data/musics/yky.ogg");
	MusicManager::AddMusic(MusicID::UNDERGROUND, "data/musics/underground1.ogg");
	MusicManager::AddMusic(MusicID::APOPLEXY, "data/musics/apoplexy.ogg");
	MusicManager::AddMusic(MusicID::MARIO_DEATH, "data/musics/1-Music-Die.ogg");
	//MusicManager::AddMusic("MainMusicOGG", "data/musics/The_Worst_Goodbye.ogg");
	MusicManager::AddMusic(MusicID::DEFAULT, "data/musics/Rigit_Medium.ogg");
	MusicManager::AddMusic(MusicID::LEVEL_COMPLETE, "data/musics/Level_Complete.ogg");
}