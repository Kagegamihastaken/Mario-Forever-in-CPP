#include "Core/ExternalHeaders/sfMod/sfMod.hpp"
#include "Core/Loading/Loading.hpp"
void IOInit() {
	PHYSFS_init(nullptr);
	PHYSFS_mount("data.zip", "/", 1);
}
void IODeinit() {
	PHYSFS_deinit();
}
std::vector<uint8_t> Loadbyte(const std::string &fileName)
{
	std::vector<uint8_t> vec;
	if (PHYSFS_isInit()) {
		if (PHYSFS_File* fp = PHYSFS_openRead(fileName.c_str())) {
			std::vector<uint8_t> buffer(1024);
			do {
				const PHYSFS_sint64 rc = PHYSFS_readBytes(fp, buffer.data(), buffer.size());
				vec.insert(vec.end(), buffer.begin(), buffer.begin() + rc);
			} while (!PHYSFS_eof(fp));
			PHYSFS_close(fp);
		}
	}
	return vec;
}
void LoadTexture(sf::Texture& texture, const std::string &path) {
	std::vector<uint8_t> vec = Loadbyte(path);
	texture.loadFromMemory(vec.data(), vec.size());
	vec.clear();
}
void LoadAudio(sf::SoundBuffer& soundBuffer, const std::string &path) {
	std::vector<uint8_t> vec = Loadbyte(path);
	soundBuffer.loadFromMemory(vec.data(), vec.size());
	vec.clear();
}
void LoadLvl(std::string& lvl, const std::string &path) {
	std::vector<uint8_t> vec = Loadbyte(path);
	lvl.assign(vec.begin(), vec.end());
	vec.clear();
}
void LoadMOD(sfmod::Mod& music, const std::string &path) {
	std::vector<uint8_t> vec = Loadbyte(path);
	music.loadFromFile(path);
	vec.clear();
	//music.loadFromMemory(vec.data(), vec.size());
}
void LoadOGG(sf::Music& music, const std::string &path) {
	std::vector<uint8_t> vec = Loadbyte(path);
	music.openFromFile(path);
	vec.clear();
	//music.openFromMemory(vec.data(), vec.size());
}
int ReadStrLine(const std::string& lvldata, std::string& out, const int resume = 0) {
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
void LoadImageFile(sf::Image& image, const std::string &path) {
	std::vector<uint8_t> vec = Loadbyte(path);
	if (!image.loadFromMemory(vec.data(), vec.size())) std::cout << "Failed to load image from: " + path + "\n";
	vec.clear();
}