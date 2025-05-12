#include "../headers/Core/MusicManager.hpp"
#include "../headers/Core/Loading/Loading.hpp"

std::map<std::string, sf::Music> MusicManager::m_ogg_musics;
std::map<std::string, sfmod::Mod> MusicManager::m_mod_musics;

void MusicManager::AddOGGMusic(const std::string &name, const std::string &path) {
	LoadOGG(m_ogg_musics[name], path);
}
void MusicManager::SetOGGLoop(const std::string &name, const bool loop) {
	m_ogg_musics[name].setLooping(loop);
}
void MusicManager::StopOGGMusic(const std::string &name) {
	m_ogg_musics[name].stop();
}
void MusicManager::StopAllOGGMusic() {
	for (auto&[fst, snd] : m_ogg_musics) {
		if (snd.getStatus() == sf::SoundSource::Status::Playing) snd.stop();
	}
}
void MusicManager::PlayOGGMusic(const std::string &name) {
	m_ogg_musics[name].play();
}
void MusicManager::PauseOGGMusic(const std::string &name) {
	m_ogg_musics[name].pause();
}
void MusicManager::ClearUp() {
	m_ogg_musics.clear();
	m_mod_musics.clear();
}

void MusicManager::AddMODMusic(const std::string &name, const std::string &path) {
	LoadMOD(m_mod_musics[name], path);
	//LoadMOD(m_mod_musics[name], ID, channel, 44100);
}
void MusicManager::SetMODLoop(const std::string &name, const bool loop) {
	m_mod_musics[name].setLooping(loop);
}
void MusicManager::StopMODMusic(const std::string &name) {
	m_mod_musics[name].stop();
}
void MusicManager::PlayMODMusic(const std::string &name) {
	m_mod_musics[name].play();
}
void MusicManager::PauseMODMusic(const std::string &name) {
	m_mod_musics[name].pause();
}
bool MusicManager::IsMODMusicPlaying(const std::string &name) {
	return m_mod_musics[name].getStatus() == sf::SoundSource::Status::Playing;
}
bool MusicManager::IsOGGMusicPlaying(const std::string &name) {
	return m_ogg_musics[name].getStatus() == sf::SoundSource::Status::Playing;
}
bool MusicManager::IsMODMusicStopped(const std::string &name) {
	return m_mod_musics[name].getStatus() == sf::SoundSource::Status::Stopped;
}
bool MusicManager::IsOGGMusicStopped(const std::string &name) {
	return m_ogg_musics[name].getStatus() == sf::SoundSource::Status::Stopped;
}
bool MusicManager::IsMODMusicPaused(const std::string &name) {
	return m_mod_musics[name].getStatus() == sf::SoundSource::Status::Paused;
}
bool MusicManager::IsOGGMusicPaused(const std::string &name) {
	return m_ogg_musics[name].getStatus() == sf::SoundSource::Status::Paused;
}
void MusicManager::StopAllMODMusic() {
	for (auto&[fst, snd] : m_mod_musics) {
		if (snd.getStatus() == sf::SoundSource::Status::Playing) snd.stop();
	}
}