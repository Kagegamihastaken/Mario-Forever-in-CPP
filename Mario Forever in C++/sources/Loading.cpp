#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <Windows.h>
#include <string>
#include <modplug.h>
#include "../headers/sfMod.hpp"
#include "../headers/Loading.hpp"
#include "../resource.h"
HMODULE GCM() {
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)GCM,
		&hModule);
	return hModule;
}
void LoadTexture(sf::Texture& texture, int resID) {
	HRSRC hRES = FindResource(GCM(), MAKEINTRESOURCE(resID), MAKEINTRESOURCE(IMAGEFILE));
	HGLOBAL hData = LoadResource(GCM(), hRES);
	DWORD hSize = SizeofResource(GCM(), hRES);
	char* hFinal = (char*)LockResource(hData);
	texture.loadFromMemory(hFinal, hSize);
}
void LoadAudio(sf::SoundBuffer& soundBuffer, int resID) {
	HRSRC hRES = FindResource(GCM(), MAKEINTRESOURCE(resID), MAKEINTRESOURCE(SOUNDFILE));
	HGLOBAL hData = LoadResource(GCM(), hRES);
	DWORD hSize = SizeofResource(GCM(), hRES);
	char* hFinal = (char*)LockResource(hData);
	soundBuffer.loadFromMemory(hFinal, hSize);
}
void LoadLvl(std::string& lvl, int resID) {
	HRSRC hRES = FindResource(GCM(), MAKEINTRESOURCE(resID), MAKEINTRESOURCE(LVLFILE));
	HGLOBAL hData = LoadResource(GCM(), hRES);
	DWORD hSize = SizeofResource(GCM(), hRES);
	char* hFinal = (char*)LockResource(hData);
	lvl.assign(hFinal, hSize);
}
void LoadMOD(sfmod::Mod& music, int resID, int channel, int samplerate) {
	HRSRC hRES = FindResource(GCM(), MAKEINTRESOURCE(resID), MAKEINTRESOURCE(MODFILE));
	HGLOBAL hData = LoadResource(GCM(), hRES);
	DWORD hSize = SizeofResource(GCM(), hRES);
	char* hFinal = (char*)LockResource(hData);
	music.loadFromMemory(hFinal, hSize);
}
void LoadOGG(sf::SoundBuffer& music, int resID) {
	HRSRC hRES = FindResource(GCM(), MAKEINTRESOURCE(resID), MAKEINTRESOURCE(OGGFILE));
	HGLOBAL hData = LoadResource(GCM(), hRES);
	DWORD hSize = SizeofResource(GCM(), hRES);
	char* hFinal = (char*)LockResource(hData);
	music.loadFromMemory(hFinal, hSize);
}
int ReadStrLine(std::string& lvldata, std::string& out, int resume = 0) {
	std::string ou = "";
	for (int i = resume; i < lvldata.size(); ++i) {
		if (lvldata[i] == '\n') {
			out = ou.substr(0, ou.size() - 1);
			return i + 1;
		}
		else ou += lvldata[i];
	}
	return -1;
}