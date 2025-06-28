#include "Core/ExternalHeaders/sfMod/sfMod.hpp"
#include "Core/Loading/Loading.hpp"

#include <fmt/core.h>

void IOInit() {
	PHYSFS_init(nullptr);
	if (PHYSFS_mount("data.zip", "/", 1) == 0) throw std::runtime_error("PhysFS: Cannot mount data.zip");
}
void IODeinit() {
	PHYSFS_deinit();
}
std::vector<uint8_t> Loadbyte(const std::string &fileName)
{
	std::vector<uint8_t> vec;
	if (PHYSFS_isInit()) {
		if (!PHYSFS_exists(fileName.c_str())) throw std::runtime_error(fmt::format("PhysFS: File {} doesn't exist", fileName));
		if (PHYSFS_File* fp = PHYSFS_openRead(fileName.c_str())) {
			std::vector<uint8_t> buffer(1024);
			do {
				const PHYSFS_sint64 rc = PHYSFS_readBytes(fp, buffer.data(), buffer.size());
				vec.insert(vec.end(), buffer.begin(), buffer.begin() + rc);
			} while (!PHYSFS_eof(fp));
			PHYSFS_close(fp);
		}
	}
	else throw std::runtime_error("PhysFS: Load data before initializing, call IOInit() first");
	return vec;
}
void LoadTexture(sf::Texture& texture, const std::string &path) {
	std::vector<uint8_t> vec = Loadbyte(path);
	if (!texture.loadFromMemory(vec.data(), vec.size())) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", path));
	vec.clear();
}
void LoadAudio(sf::SoundBuffer& soundBuffer, const std::string &path) {
	std::vector<uint8_t> vec = Loadbyte(path);
	if (!soundBuffer.loadFromMemory(vec.data(), vec.size())) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", path));
	vec.clear();
}
void LoadLvl(std::string& lvl, const std::string &path) {
	std::vector<uint8_t> vec = Loadbyte(path);
	lvl.assign(vec.begin(), vec.end());
	vec.clear();
}
void LoadMOD(sfmod::Mod& music, const std::string &path) {
	//std::vector<uint8_t> vec = Loadbyte(path);
	if (!music.loadFromFile(path)) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", path));
	//vec.clear();
	//music.loadFromMemory(vec.data(), vec.size());
}
void LoadOGG(sf::Music& music, const std::string &path) {
	//std::vector<uint8_t> vec = Loadbyte(path);
	if (!music.openFromFile(path)) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", path));;
	//vec.clear();
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
	if (!image.loadFromMemory(vec.data(), vec.size())) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", path));;
	vec.clear();
}