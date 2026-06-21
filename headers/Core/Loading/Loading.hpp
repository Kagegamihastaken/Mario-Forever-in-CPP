#pragma once

#ifndef LOADING_HPP
#define LOADING_HPP
namespace MFCPP {
    class IO final{
        static std::vector<uint8_t> Loadbyte(const std::filesystem::path &fileName);
    public:
        IO() = delete;
        IO(const IO&) = delete;
        IO(IO&&) = delete;
        IO& operator=(const IO&) = delete;
        IO& operator=(IO&&) = delete;

        static void Init();
        static void Deinit();
        static void LoadTexture(sf::Texture& texture, const std::filesystem::path &path, const sf::IntRect& rect = {});
        static void LoadLvl(std::string& lvl, const std::filesystem::path &path);
        static std::vector<uint8_t> GetFileDataInByte(const std::filesystem::path &path);
        static void LoadImageFile(sf::Image& image, const std::filesystem::path &path);
    };
}
#endif