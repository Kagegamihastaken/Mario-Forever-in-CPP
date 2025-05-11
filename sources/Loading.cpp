#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <string>
#include <physfs.h>
#include "../headers/Core/ExternalHeaders/sfMod/sfMod.hpp"
#include "../headers/Core/Loading/Loading.hpp"
void IOInit() {
	PHYSFS_init(nullptr);
	PHYSFS_mount("data.zip", "/", 1);
}
void IODeinit() {
	PHYSFS_deinit();
}
std::vector<uint8_t> Loadbyte(std::string fileName)
{
	std::vector<uint8_t> vec;
	if (PHYSFS_isInit()) {
		PHYSFS_File* fp = PHYSFS_openRead(fileName.c_str());
		if (fp) {
			std::vector<uint8_t> buffer(1024);
			PHYSFS_sint64 rc;
			do {
				rc = PHYSFS_readBytes(fp, buffer.data(), buffer.size());
				vec.insert(vec.end(), buffer.begin(), buffer.begin() + rc);
			} while (!PHYSFS_eof(fp));
			PHYSFS_close(fp);
		}
	}
	return vec;
}
void LoadTexture(sf::Texture& texture, std::string path) {
	std::vector<uint8_t> vec = Loadbyte(path);
	texture.loadFromMemory(vec.data(), vec.size());
	vec.clear();
}
void LoadAudio(sf::SoundBuffer& soundBuffer, std::string path) {
	std::vector<uint8_t> vec = Loadbyte(path);
	soundBuffer.loadFromMemory(vec.data(), vec.size());
	vec.clear();
}
void LoadLvl(std::string& lvl, std::string path) {
	std::vector<uint8_t> vec = Loadbyte(path);
	lvl.assign(vec.begin(), vec.end());
	vec.clear();
}
void LoadMOD(sfmod::Mod& music, std::string path) {
	std::vector<uint8_t> vec = Loadbyte(path);
	music.loadFromFile(path);
	vec.clear();
	//music.loadFromMemory(vec.data(), vec.size());
}
void LoadOGG(sf::Music& music, std::string path) {
	std::vector<uint8_t> vec = Loadbyte(path);
	music.openFromFile(path);
	vec.clear();
	//music.openFromMemory(vec.data(), vec.size());
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
	out = ou.substr(0, ou.size());
	return -1;
}
void LoadImageFile(sf::Image& image, std::string path) {
	std::vector<uint8_t> vec = Loadbyte(path);
	image.loadFromMemory(vec.data(), vec.size());
	vec.clear();
}