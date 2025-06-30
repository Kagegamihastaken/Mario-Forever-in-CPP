#include "Core/MusicManager.hpp"
#include "Core/Logging.hpp"
#include "Core/Loading/Loading.hpp"

std::map<std::string, std::vector<uint8_t>> MusicManager::m_ogg_data;
std::map<std::string, sf::Music> MusicManager::m_ogg_musics;
std::map<std::string, sfmod::Mod> MusicManager::m_mod_musics;
std::map<std::string, bool> MusicManager::m_musics_id;
//false for OGG, true for MOD

void MusicManager::AddMusic(const std::string &name, const std::string &path) {
	//custom loading
	if (m_musics_id.contains(name)) throw MFCPP::Exception::AlreadyExistElement(fmt::format("MusicManager: Already exist music with this name {}", name));
	const std::vector<uint8_t> Data = GetFileDataInByte(path);
	bool flag = false;
	m_ogg_data[name] = std::move(Data);

	if (!m_mod_musics[name].loadFromMemory(m_ogg_data[name].data(), m_ogg_data[name].size())) {
		if (!m_ogg_musics[name].openFromMemory(m_ogg_data[name].data(), m_ogg_data[name].size()))
			throw std::format_error(fmt::format("MusicManager: Cannot detected {} is music file supported or not.", path));
		MFCPP::Log::InfoPrint(fmt::format("MusicManager: Loaded OGG File: {}", path));
	} else {
		m_ogg_data.erase(name);
		flag = true;
		MFCPP::Log::InfoPrint(fmt::format("MusicManager: Loaded MOD File: {}", path));
	}
	m_musics_id[name] = flag;
	//LoadOGG(m_ogg_musics[name], path);
}
void MusicManager::SetLoop(const std::string &name, const bool loop) {
	if (!m_musics_id.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	if (!m_musics_id[name])
		m_ogg_musics[name].setLooping(loop);
	else
		m_mod_musics[name].setLooping(loop);
}
void MusicManager::StopMusic(const std::string &name) {
	if (!m_musics_id.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	if (!m_musics_id[name])
		m_ogg_musics[name].stop();
	else
		m_mod_musics[name].stop();
}
void MusicManager::StopAllMusic() {
	for (const auto &[fst, snd] : m_musics_id)
		switch (snd) {
		case false:
			if (m_ogg_musics[fst].getStatus() == sf::SoundSource::Status::Playing) m_ogg_musics[fst].stop();
			break;
		case true:
			if (m_mod_musics[fst].getStatus() == sf::SoundSource::Status::Playing) m_mod_musics[fst].stop();
			break;
		}
}
void MusicManager::PlayMusic(const std::string &name) {
	if (!m_musics_id.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	if (!m_musics_id[name])
		m_ogg_musics[name].play();
	else
		m_mod_musics[name].play();
}
void MusicManager::PauseMusic(const std::string &name) {
	if (!m_musics_id.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	if (!m_musics_id[name])
		m_ogg_musics[name].pause();
	else
		m_mod_musics[name].pause();
}
void MusicManager::CleanUp() {
	m_ogg_data.clear();
	m_ogg_musics.clear();
	m_mod_musics.clear();
}
void MusicManager::SetMusicVolume(const std::string &name, const unsigned volume) {
	if (!m_musics_id.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	if (!m_musics_id[name])
		m_ogg_musics[name].setVolume(volume);
	else
		m_mod_musics[name].setMasterVolume(volume);
}
bool MusicManager::IsMusicPlaying(const std::string &name) {
	if (!m_musics_id.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	if (!m_musics_id[name])
		return m_mod_musics[name].getStatus() == sf::SoundSource::Status::Playing;
	return m_mod_musics[name].getStatus() == sf::SoundSource::Status::Playing;
}
bool MusicManager::IsMusicStopped(const std::string &name) {
	if (!m_musics_id.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	if (!m_musics_id[name])
		return m_ogg_musics[name].getStatus() == sf::SoundSource::Status::Stopped;
	return m_mod_musics[name].getStatus() == sf::SoundSource::Status::Stopped;
}
bool MusicManager::IsMusicPaused(const std::string &name) {
	if (!m_musics_id.contains(name)) throw MFCPP::Exception::NonExistElement(fmt::format("MusicManager: Cannot find {}", name));
	if (!m_musics_id[name])
		return m_ogg_musics[name].getStatus() == sf::SoundSource::Status::Paused;
	return m_mod_musics[name].getStatus() == sf::SoundSource::Status::Paused;
}