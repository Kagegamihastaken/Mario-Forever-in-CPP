#pragma once

#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP
#include <queue>

extern sf::Texture tempTex;
extern void InitTempTex();
struct PreTexture {
	std::filesystem::path path;
	sf::IntRect rect;
	bool isRepeat;
};
class ImageManager {
	static std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_textures;
	static std::unordered_map<std::string, PreTexture> m_pre_textures;
	static std::unordered_map<std::string, sf::VertexArray> m_vertex_array;
	//Drawing only
	static std::unordered_set<std::string> m_vertex_set;
	static std::queue<std::string> m_vertex_queue;
	static size_t draw_count;
public:
	ImageManager() = delete;
	static void ClearAllVertex();
	static void AddToVertex(const std::string& name_tex, const sf::IntRect& texRect, const sf::Transform& trans, const sf::Color &color);
	static void DrawAllVertex();
	static void AddTexture(const std::string &name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect = {}, const bool &isRepeated = false);
	static void PreloadTexture(const std::string &name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect, const bool &isRepeated = false);
	static void LoadTexture(const std::string &name_tex, const std::filesystem::path &path, const sf::IntRect &tex_rect = {}, const bool &isRepeated = false);
	static void LoadTexture(const std::string& name_tex);
	static sf::Texture& GetTexture(const std::string &name);
	static sf::Texture* GetReturnTexture(const std::string& name);
	static bool isExist(const std::string& name_tex);
	static void Cleanup();
	static void printDrawCount();
};

#endif