#include <iostream>

#include "Core/ImageManager.hpp"
#include "Core/Loading/Loading.hpp"

std::map<std::string, sf::Image> ImageManager::m_images;
std::map<std::string, sf::Texture*> ImageManager::m_textures;

sf::Texture tempTex;
void InitTempTex() {
	LoadTexture(tempTex, "data/resources/placeholder.png");
}
void ImageManager::AddImage(const std::string &name, const std::string& path) {
	m_images[name] = sf::Image();
	LoadImageFile(m_images[name], path);
}
sf::Image ImageManager::GetImage(const std::string &name) {
	return m_images[name];
}
void ImageManager::Clearup() {
	m_images.clear();
	m_textures.clear();
}

void ImageManager::AddTexture(const std::string &img_name, const sf::IntRect &tex_rect, const std::string &name_tex,
                              const bool &isRepeated, const bool &isFlipped) {
	if (!m_images.contains(img_name)) {
		std::cout << "Cannot lookup " + img_name << "\n";
		return;
	}
	sf::Texture* tex = new sf::Texture();
	ConvertToTexture(tex, img_name, tex_rect, isFlipped);
	tex->setRepeated(isRepeated);
	m_textures[name_tex] = tex;
	//tex->setSmooth(true);
}

void ImageManager::AddTexture(const std::string &img_name, const std::string &name_tex, const bool &isRepeated,
                              const bool &isFlipped) {
	if (!m_images.contains(img_name)) {
		std::cout << "Cannot lookup " + img_name << "\n";
		return;
	}
	sf::Texture* tex = new sf::Texture();
	ConvertToTexture(tex, img_name, isFlipped);
	tex->setRepeated(isRepeated);
	m_textures[name_tex] = tex;
	//tex->setSmooth(true);
}
sf::Texture& ImageManager::GetTexture(const std::string& name)
{
	if (!m_textures.contains(name)) std::cout << "Cannot find " << name << "\n";
	return *m_textures[name];
}
sf::Texture* ImageManager::GetReturnTexture(const std::string& name)
{
	if (!m_textures.contains(name)) std::cout << "Cannot find " << name << "\n";
	return m_textures[name];
}
void ImageManager::ConvertToTexture(sf::Texture* tex, const std::string& name, const bool& isFlipped) {
	sf::Image inp = m_images[name];
	if (!m_images.contains(name)) std::cout << "WARNING: " << name << " does not exist\n";
	else if (m_images[name].getSize().x == 0 || m_images[name].getSize().y == 0) std::cout << "WARNING: " << name << " have a vert / horz equal 0\n";
	if (isFlipped) inp.flipHorizontally();
	if (!tex->loadFromImage(inp)) std::cout << "Failed to convert " + name + " to texture\n";
}
void ImageManager::ConvertToTexture(sf::Texture* tex, const std::string& name, const sf::IntRect& rect, const bool& isFlipped) {
	sf::Image inp = m_images[name];
	if (!m_images.contains(name)) std::cout << "WARNING: " << name << " does not exist\n";
	else if (m_images[name].getSize().x == 0 || m_images[name].getSize().y == 0) std::cout << "WARNING: " << name << " have a vert / horz equal 0\n";
	sf::IntRect rec = rect;
	if (isFlipped) {
		inp.flipHorizontally();
		rec.position.x = inp.getSize().x - rec.position.x - rec.size.x;
	}
	if (!tex->loadFromImage(inp, false, rec)) std::cout << "Failed to convert " + name + " to texture\n";
}
void ImageManager::CreateTestImage(const std::string& name, const std::string& new_name) {
	const int imageSizeX = static_cast<int>(m_images[name].getSize().x);
	const int imageSizeY = static_cast<int>(m_images[name].getSize().y);
	m_images[new_name] = sf::Image();
	m_images[new_name].resize({m_images[name].getSize().x + 2, m_images[name].getSize().y + 2}, sf::Color::Green);
	if (!m_images[new_name].copy(m_images[name], sf::Vector2u(1,1))) {
		std::cout << "Error on creating " << new_name << "\n";
		return;
	}

	if (!m_images[new_name].copy(m_images[name], sf::Vector2u(1,0), sf::IntRect({0, 0}, {imageSizeX, 1}))) {
		std::cout << "Error on creating " << new_name << "\n";
		return;
	}
	if (!m_images[new_name].copy(m_images[name], sf::Vector2u(1,imageSizeY + 1), sf::IntRect({0, imageSizeY-1}, {imageSizeX, 1}))) {
		std::cout << "Error on creating " << new_name << "\n";
		return;
	}

	if (!m_images[new_name].copy(m_images[name], sf::Vector2u(0,1), sf::IntRect({0, 0}, {1, imageSizeY}))) {
		std::cout << "Error on creating " << new_name << "\n";
		return;
	}
	if (!m_images[new_name].copy(m_images[name], sf::Vector2u(imageSizeX + 1,1), sf::IntRect({imageSizeX - 1, 0}, {1, imageSizeY}))) {
		std::cout << "Error on creating " << new_name << "\n";
		return;
	}

	m_images[new_name].setPixel(sf::Vector2u(0, 0),m_images[name].getPixel(sf::Vector2u(0, 0)));
	m_images[new_name].setPixel(sf::Vector2u(imageSizeX + 1, 0),m_images[name].getPixel(sf::Vector2u(imageSizeX - 1, 0)));
	m_images[new_name].setPixel(sf::Vector2u(0, imageSizeY + 1), m_images[name].getPixel(sf::Vector2u(0, imageSizeY - 1)));
	m_images[new_name].setPixel(sf::Vector2u(imageSizeX + 1, imageSizeY + 1), m_images[name].getPixel(sf::Vector2u(imageSizeX - 1, imageSizeY - 1)));
}