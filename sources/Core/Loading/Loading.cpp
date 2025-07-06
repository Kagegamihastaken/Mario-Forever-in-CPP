#include "Core/Loading/Loading.hpp"
#include "Core/Loading/PhysFsStream.hpp"

#include <fmt/core.h>

void IOInit() {
	PHYSFS_init(nullptr);
	if (PHYSFS_mount("data.zip", "/", 1) == 0) throw std::runtime_error("PhysFS: Cannot mount data.zip");
}
void IODeinit() {
	PHYSFS_deinit();
}
std::vector<uint8_t> Loadbyte(const std::filesystem::path &fileName)
{
	std::vector<uint8_t> vec;
	if (PHYSFS_isInit()) {
		if (!PHYSFS_exists(fileName.string().c_str())) throw std::runtime_error(fmt::format("PhysFS: File {} doesn't exist", fileName.string()));
		if (PHYSFS_File* fp = PHYSFS_openRead(fileName.string().c_str())) {
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
void LoadTexture(sf::Texture& texture, const std::filesystem::path &path, const sf::IntRect& rect) {
	//std::vector<uint8_t> vec = Loadbyte(path);
	if (PhysFsStream loadFile(path.string().c_str()); !texture.loadFromStream(loadFile, false, rect)) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", path.string()));
	//if (!texture.loadFromMemory(vec.data(), vec.size())) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", path.string()));
	//vec.clear();
}
void LoadLvl(std::string& lvl, const std::filesystem::path &path) {
	std::vector<uint8_t> vec = Loadbyte(path);
	lvl.assign(vec.begin(), vec.end());
	vec.clear();
}
std::vector<uint8_t> GetFileDataInByte(const std::filesystem::path &path) {
	return Loadbyte(path);
	//if (!music.openFromFile(path)) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", path));;
	//vec.clear();
	//music.openFromMemory(vec.data(), vec.size());
}
void LoadImageFile(sf::Image& image, const std::filesystem::path &path) {
	//std::vector<uint8_t> vec = Loadbyte(path);
	if (PhysFsStream loadFile(path.string().c_str()); !image.loadFromStream(loadFile)) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", path.string()));;
	//if (!image.loadFromMemory(vec.data(), vec.size())) throw std::runtime_error(fmt::format("Loading: Unexpected Error when trying to load {}", path.string()));;
	//vec.clear();
}