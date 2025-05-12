#include "../headers/Core/SoundManager.hpp"

std::map<std::string, sf::Sound> SoundManager::m_sounds;
std::map<std::string, sf::SoundBuffer> SoundManager::m_buffers;

void SoundManager::AddSound(std::string name, const sf::SoundBuffer &buffer) {
	m_buffers[name] = buffer;
	m_sounds.insert({ name, sf::Sound(m_buffers[name]) });
}
sf::Sound SoundManager::GetSound(const std::string &name) {
	return m_sounds.at(name);
}
void SoundManager::SetLoop(const std::string &name, const bool loop) {
	m_sounds.at(name).setLooping(loop);
}
void SoundManager::StopSound(const std::string &name) {
	m_sounds.at(name).stop();
}
void SoundManager::StopAllSound() {
	for (auto&[fst, snd] : m_sounds) {
		snd.stop();
	}
}
void SoundManager::PlaySound(const std::string &name) {
	m_sounds.at(name).play();
}
void SoundManager::ClearUp() {
	m_sounds.clear();
	m_buffers.clear();
}