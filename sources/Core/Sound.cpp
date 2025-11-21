#include "Core/Sound.hpp"

void SoundInit() {
	SoundManager::AddSound("Jump", "data/sounds/jump.ogg");
	SoundManager::AddSound("Bump", "data/sounds/bump.ogg");
	SoundManager::AddSound("Break", "data/sounds/break.ogg");
	SoundManager::AddSound("Coin", "data/sounds/coin.ogg");
	SoundManager::AddSound("Stomp", "data/sounds/stomp.ogg");
	SoundManager::AddSound("Powerup", "data/sounds/powerup.ogg");
	SoundManager::AddSound("Vine", "data/sounds/vine.ogg");
	SoundManager::AddSound("Kick2", "data/sounds/kick2.ogg");
	SoundManager::AddSound("Pipe", "data/sounds/pipe.ogg");
	SoundManager::AddSound("Kick3", "data/sounds/kick3.ogg");
	SoundManager::AddSound("Kick4", "data/sounds/kick4.ogg");
	SoundManager::AddSound("Kick5", "data/sounds/kick5.ogg");
	SoundManager::AddSound("Kick6", "data/sounds/kick6.ogg");
	SoundManager::AddSound("Kick7", "data/sounds/kick7.ogg");
	SoundManager::AddSound("Kick8", "data/sounds/kick8.ogg");
	SoundManager::AddSound("1UP", "data/sounds/1up.ogg");
	SoundManager::AddSound("Hammer", "data/sounds/hammer.ogg");
	SoundManager::AddSound("Fireball", "data/sounds/fireball.ogg");
	SoundManager::AddSound("Bullet1", "data/sounds/bullet1.ogg");
	SoundManager::AddSound("Bullet2", "data/sounds/bullet2.ogg");
	SoundManager::AddSound("Bullet3", "data/sounds/bullet3.ogg");
	SoundManager::AddSound("Timeout", "data/sounds/timeout.ogg");
	SoundManager::AddSound("Throw", "data/sounds/throw.ogg");
	//Editor
	SoundManager::AddSound("EDITOR_PLACE", "data/sounds/Editor/EDITOR_place.ogg");
	SoundManager::AddSound("EDITOR_DELETE", "data/sounds/Editor/EDITOR_delete.ogg");
	SoundManager::AddSound("EDITOR_SWITCH", "data/sounds/Editor/EDITOR_switch.ogg");
	SoundManager::AddSound("EDITOR_MENU", "data/sounds/Editor/EDITOR_menu.ogg");
	SoundManager::AddSound("EDITOR_CLOSE", "data/sounds/Editor/EDITOR_close.ogg");
	SoundManager::AddSound("EDITOR_TAB_SELECT", "data/sounds/Editor/EDITOR_tab_select.ogg");
}