#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <Windows.h>
#include <string>
#include "../headers/Loading.hpp"
HMODULE GCM() {
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)GCM,
		&hModule);
	return hModule;
}
void LoadTexture(sf::Texture& texture, int resID, int resType) {
	HRSRC hRES = FindResource(GCM(), MAKEINTRESOURCE(resID), MAKEINTRESOURCE(resType));
	HGLOBAL hData = LoadResource(GCM(), hRES);
	DWORD hSize = SizeofResource(GCM(), hRES);
	char* hFinal = (char*)LockResource(hData);
	texture.loadFromMemory(hFinal, hSize);
}
void LoadAudio(sf::SoundBuffer& soundBuffer, int resID, int resType) {
	HRSRC hRES = FindResource(GCM(), MAKEINTRESOURCE(resID), MAKEINTRESOURCE(resType));
	HGLOBAL hData = LoadResource(GCM(), hRES);
	DWORD hSize = SizeofResource(GCM(), hRES);
	char* hFinal = (char*)LockResource(hData);
	soundBuffer.loadFromMemory(hFinal, hSize);
}
void LoadLvl(std::string& lvl, int resID, int resType) {
	HRSRC hRES = FindResource(GCM(), MAKEINTRESOURCE(resID), MAKEINTRESOURCE(resType));
	HGLOBAL hData = LoadResource(GCM(), hRES);
	DWORD hSize = SizeofResource(GCM(), hRES);
	char* hFinal = (char*)LockResource(hData);
	lvl.assign(hFinal, hSize);
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