#include "Core/Sound.hpp"

void SoundInit() {
	SoundManager::AddSound("Jump", "data/sounds/jump.wav", 0.05f, 0.5f);
	SoundManager::AddSound("Bump", "data/sounds/bump.wav", 0.04f, 0.5f);
	SoundManager::AddSound("Break", "data/sounds/break.wav");
	SoundManager::AddSound("Coin", "data/sounds/coin.wav", 0.04f, 0.6f);
	SoundManager::AddSound("Stomp", "data/sounds/stomp.wav", 0.04f, 0.5f);
	SoundManager::AddSound("Powerup", "data/sounds/powerup.wav");
	SoundManager::AddSound("Vine", "data/sounds/vine.wav");
	SoundManager::AddSound("Kick2", "data/sounds/kick2.wav", 0.03f, 0.5f);
	SoundManager::AddSound("Pipe", "data/sounds/pipe.wav");
	SoundManager::AddSound("Kick3", "data/sounds/kick3.wav", 0.04f, 0.6f);
	SoundManager::AddSound("Kick4", "data/sounds/kick4.wav", 0.04f, 0.6f);
	SoundManager::AddSound("Kick5", "data/sounds/kick5.wav", 0.04f, 0.6f);
	SoundManager::AddSound("Kick6", "data/sounds/kick6.wav", 0.04f, 0.6f);
	SoundManager::AddSound("Kick7", "data/sounds/kick7.wav", 0.04f, 0.6f);
	SoundManager::AddSound("Kick8", "data/sounds/kick8.wav", 0.04f, 0.6f);
	SoundManager::AddSound("1UP", "data/sounds/1up.wav");
	SoundManager::AddSound("Hammer", "data/sounds/hammer.wav", 0.04f, 0.5f);
	SoundManager::AddSound("Fireball", "data/sounds/fireball.wav", 0.04f, 0.5f);
	//Editor
	SoundManager::AddSound("EDITOR_PLACE", "data/sounds/Editor/EDITOR_place.wav");
	SoundManager::AddSound("EDITOR_DELETE", "data/sounds/Editor/EDITOR_delete.wav");
	SoundManager::AddSound("EDITOR_SWITCH", "data/sounds/Editor/EDITOR_switch.wav");
	SoundManager::AddSound("EDITOR_MENU", "data/sounds/Editor/EDITOR_menu.wav");
	SoundManager::AddSound("EDITOR_CLOSE", "data/sounds/Editor/EDITOR_close.wav");
	SoundManager::AddSound("EDITOR_TAB_SELECT", "data/sounds/Editor/EDITOR_tab_select.wav");
}