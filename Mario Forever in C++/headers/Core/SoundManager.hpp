#pragma once

#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include <map>
#include <string>
#include <SFML/Audio.hpp>
#include <vector>

class SoundManager {
private:
	static std::map<std::string, sf::Sound> m_sounds;
	static std::map<std::string, sf::SoundBuffer> m_buffers;
public:
	static void AddSound(std::string name, sf::SoundBuffer buffer);
	static sf::Sound GetSound(std::string name);
	static void PlaySound(std::string name);
	static void ClearUp();
};

#endif