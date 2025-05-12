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
	static void AddSound(std::string name, const sf::SoundBuffer &buffer);
	static sf::Sound GetSound(const std::string &name);
	static void PlaySound(const std::string &name);
	static void ClearUp();
	static void SetLoop(const std::string &name, bool loop);
	static void StopSound(const std::string &name);
	static void StopAllSound();
};

#endif