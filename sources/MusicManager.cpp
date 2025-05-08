#include "../headers/Core/MusicManager.hpp"
#include "../headers/Core/Loading/Loading.hpp"

std::map<std::string, sf::Music> MusicManager::m_ogg_musics;
std::map<std::string, sfmod::Mod> MusicManager::m_mod_musics;

void MusicManager::AddOGGMusic(std::string name, int ID) {
	LoadOGG(m_ogg_musics[name], ID);
}
void MusicManager::SetOGGLoop(std::string name, bool loop) {
	m_ogg_musics[name].setLooping(loop);
}
void MusicManager::StopOGGMusic(std::string name) {
	m_ogg_musics[name].stop();
}
void MusicManager::StopAllOGGMusic() {
	for (auto& music : m_ogg_musics) {
		music.second.stop();
	}
}
void MusicManager::PlayOGGMusic(std::string name) {
	m_ogg_musics[name].play();
}
void MusicManager::PauseOGGMusic(std::string name) {
	m_ogg_musics[name].pause();
}
void MusicManager::ClearUp() {
	m_ogg_musics.clear();
	m_mod_musics.clear();
}

void MusicManager::AddMODMusic(std::string name, int ID, int channel) {
	LoadMOD(m_mod_musics[name], ID, channel, 44100);
}
void MusicManager::SetMODLoop(std::string name, bool loop) {
	m_mod_musics[name].setLooping(loop);
}
void MusicManager::StopMODMusic(std::string name) {
	m_mod_musics[name].stop();
}
void MusicManager::PlayMODMusic(std::string name) {
	m_mod_musics[name].play();
}
void MusicManager::PauseMODMusic(std::string name) {
	m_mod_musics[name].pause();
}
bool MusicManager::IsMODMusicPlaying(std::string name) {
	return m_mod_musics[name].getStatus() == sf::SoundSource::Status::Playing;
}
bool MusicManager::IsOGGMusicPlaying(std::string name) {
	return m_ogg_musics[name].getStatus() == sf::SoundSource::Status::Playing;
}
bool MusicManager::IsMODMusicStopped(std::string name) {
	return m_mod_musics[name].getStatus() == sf::SoundSource::Status::Stopped;
}
bool MusicManager::IsOGGMusicStopped(std::string name) {
	return m_ogg_musics[name].getStatus() == sf::SoundSource::Status::Stopped;
}
bool MusicManager::IsMODMusicPaused(std::string name) {
	return m_mod_musics[name].getStatus() == sf::SoundSource::Status::Paused;
}
bool MusicManager::IsOGGMusicPaused(std::string name) {
	return m_ogg_musics[name].getStatus() == sf::SoundSource::Status::Paused;
}
void MusicManager::StopAllMODMusic() {
	for (auto& music : m_mod_musics) {
		music.second.stop();
	}
}