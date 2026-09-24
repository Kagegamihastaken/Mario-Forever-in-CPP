#pragma once

#ifndef LOADING_HPP
#define LOADING_HPP
namespace MFCPP {
    namespace IO {
        static std::vector<uint8_t> Loadbyte(const std::filesystem::path &fileName);
        void Init();
        void Deinit();
        void LoadTexture(sf::Texture& texture, const std::filesystem::path &path, const sf::IntRect& rect = {});
        void LoadRaw(std::string& lvl, const std::filesystem::path &path);
        std::vector<uint8_t> GetFileDataInByte(const std::filesystem::path &path);
        void LoadImageFile(sf::Image& image, const std::filesystem::path &path);
    }
}
#endif