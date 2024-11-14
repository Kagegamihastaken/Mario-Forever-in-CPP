#include "../headers/Core/SoundManager.hpp"

std::map<std::string, sf::Sound> SoundManager::m_sounds;
std::map<std::string, sf::SoundBuffer> SoundManager::m_buffers;

void SoundManager::AddSound(std::string name, sf::SoundBuffer buffer) {
	m_buffers[name] = buffer;
	m_sounds[name] = sf::Sound();
	m_sounds[name].setBuffer(m_buffers[name]);
}
sf::Sound SoundManager::GetSound(std::string name) {
	return m_sounds[name];
}
void SoundManager::PlaySound(std::string name) {
	m_sounds[name].play();
}
void SoundManager::ClearUp() {
	m_sounds.clear();
	m_buffers.clear();
}