#include "Core/Sound.hpp"

void SoundInit() {
	SoundManager::AddSound("Jump", "data/sounds/jump.ogg", true);
	SoundManager::AddSound("Bump", "data/sounds/bump.ogg", true);
	SoundManager::AddSound("Break", "data/sounds/break.ogg", true);
	SoundManager::AddSound("Coin", "data/sounds/coin.ogg", true);
	SoundManager::AddSound("Stomp", "data/sounds/stomp.ogg", true);
	SoundManager::AddSound("Powerup", "data/sounds/powerup.ogg", true);
	SoundManager::AddSound("Vine", "data/sounds/vine.ogg", true);
	SoundManager::AddSound("Kick2", "data/sounds/kick2.ogg", true);
	SoundManager::AddSound("Pipe", "data/sounds/pipe.ogg", true);
	SoundManager::AddSound("Kick3", "data/sounds/kick3.ogg", true);
	SoundManager::AddSound("Kick4", "data/sounds/kick4.ogg", true);
	SoundManager::AddSound("Kick5", "data/sounds/kick5.ogg", true);
	SoundManager::AddSound("Kick6", "data/sounds/kick6.ogg", true);
	SoundManager::AddSound("Kick7", "data/sounds/kick7.ogg", true);
	SoundManager::AddSound("Kick8", "data/sounds/kick8.ogg", true);
	SoundManager::AddSound("1UP", "data/sounds/1up.ogg", true);
	SoundManager::AddSound("Hammer", "data/sounds/hammer.ogg", true);
	SoundManager::AddSound("Fireball", "data/sounds/fireball.ogg", true);
	SoundManager::AddSound("Bullet1", "data/sounds/bullet1.ogg", true);
	SoundManager::AddSound("Bullet2", "data/sounds/bullet2.ogg", true);
	SoundManager::AddSound("Bullet3", "data/sounds/bullet3.ogg", true);
	SoundManager::AddSound("Timeout", "data/sounds/timeout.ogg", false);
	SoundManager::AddSound("Throw", "data/sounds/throw.ogg", false);
	SoundManager::AddSound("Stun", "data/sounds/stun.ogg", true);
	SoundManager::AddSound("Chilun", "data/sounds/chilun.ogg", true);
	SoundManager::AddSound("Volcano", "data/sounds/volcano.ogg", true);
	//Editor
	SoundManager::AddSound("EDITOR_PLACE", "data/sounds/Editor/EDITOR_place.ogg", false);
	SoundManager::AddSound("EDITOR_DELETE", "data/sounds/Editor/EDITOR_delete.ogg", false);
	SoundManager::AddSound("EDITOR_SWITCH", "data/sounds/Editor/EDITOR_switch.ogg", false);
	SoundManager::AddSound("EDITOR_MENU", "data/sounds/Editor/EDITOR_menu.ogg", false);
	SoundManager::AddSound("EDITOR_CLOSE", "data/sounds/Editor/EDITOR_close.ogg", false);
	SoundManager::AddSound("EDITOR_TAB_SELECT", "data/sounds/Editor/EDITOR_tab_select.ogg", false);
}