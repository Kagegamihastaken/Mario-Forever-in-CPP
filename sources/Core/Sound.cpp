#include "Core/Sound.hpp"
#include "Core/SoundManager.hpp"

void SoundInit() {
	SoundManager::AddSound(SoundID::GAME_JUMP, "data/sounds/jump.ogg", true);
	SoundManager::AddSound(SoundID::GAME_BUMP, "data/sounds/bump.ogg", true);
	SoundManager::AddSound(SoundID::GAME_BREAK, "data/sounds/break.ogg", true);
	SoundManager::AddSound(SoundID::GAME_COIN, "data/sounds/coin.ogg", true);
	SoundManager::AddSound(SoundID::GAME_STOMP, "data/sounds/stomp.ogg", true);
	SoundManager::AddSound(SoundID::GAME_POWERUP, "data/sounds/powerup.ogg", true);
	SoundManager::AddSound(SoundID::GAME_VINE, "data/sounds/vine.ogg", true);
	SoundManager::AddSound(SoundID::GAME_KICK2, "data/sounds/kick2.ogg", true);
	SoundManager::AddSound(SoundID::GAME_PIPE, "data/sounds/pipe.ogg", true);
	SoundManager::AddSound(SoundID::GAME_KICK3, "data/sounds/kick3.ogg", true);
	SoundManager::AddSound(SoundID::GAME_KICK4, "data/sounds/kick4.ogg", true);
	SoundManager::AddSound(SoundID::GAME_KICK5, "data/sounds/kick5.ogg", true);
	SoundManager::AddSound(SoundID::GAME_KICK6, "data/sounds/kick6.ogg", true);
	SoundManager::AddSound(SoundID::GAME_KICK7, "data/sounds/kick7.ogg", true);
	SoundManager::AddSound(SoundID::GAME_KICK8, "data/sounds/kick8.ogg", true);
	SoundManager::AddSound(SoundID::GAME_1UP, "data/sounds/1up.ogg", true);
	SoundManager::AddSound(SoundID::GAME_HAMMER, "data/sounds/hammer.ogg", true);
	SoundManager::AddSound(SoundID::GAME_FIREBALL, "data/sounds/fireball.ogg", true);
	SoundManager::AddSound(SoundID::GAME_BULLET1, "data/sounds/bullet1.ogg", true);
	SoundManager::AddSound(SoundID::GAME_BULLET2, "data/sounds/bullet2.ogg", true);
	SoundManager::AddSound(SoundID::GAME_BULLET3, "data/sounds/bullet3.ogg", true);
	SoundManager::AddSound(SoundID::GAME_TIMEOUT, "data/sounds/timeout.ogg", false);
	SoundManager::AddSound(SoundID::GAME_THROW, "data/sounds/throw.ogg", false);
	SoundManager::AddSound(SoundID::GAME_STUN, "data/sounds/stun.ogg", true);
	SoundManager::AddSound(SoundID::GAME_CHILUN, "data/sounds/chilun.ogg", true);
	SoundManager::AddSound(SoundID::GAME_VOLCANO, "data/sounds/volcano.ogg", true);
	//Editor
	SoundManager::AddSound(SoundID::EDITOR_PLACE, "data/sounds/Editor/EDITOR_place.ogg", false);
	SoundManager::AddSound(SoundID::EDITOR_DELETE, "data/sounds/Editor/EDITOR_delete.ogg", false);
	SoundManager::AddSound(SoundID::EDITOR_SWITCH, "data/sounds/Editor/EDITOR_switch.ogg", false);
	SoundManager::AddSound(SoundID::EDITOR_MENU, "data/sounds/Editor/EDITOR_menu.ogg", false);
	SoundManager::AddSound(SoundID::EDITOR_CLOSE, "data/sounds/Editor/EDITOR_close.ogg", false);
	SoundManager::AddSound(SoundID::EDITOR_TAB_SELECT, "data/sounds/Editor/EDITOR_tab_select.ogg", false);
}
