#include "Core/ImageManager.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/Logging.hpp"

std::map<std::string, sf::Image> ImageManager::m_images;
std::map<std::string, sf::Texture*> ImageManager::m_textures;

sf::Texture tempTex;
void InitTempTex() {
	LoadTexture(tempTex, "data/resources/placeholder.png");
}
void ImageManager::AddImage(const std::string &name, const std::string& path) {
	m_images[name] = sf::Image();
	LoadImageFile(m_images[name], path);
	MFCPP::Log::InfoPrint(fmt::format("ImageManager: Loaded Image: {}", name));
}
sf::Image ImageManager::GetImage(const std::string &name) {
	return m_images[name];
}
void ImageManager::Cleanup() {
	MFCPP::Log::InfoPrint("ImageManager cleanup...");
	m_images.clear();
	m_textures.clear();
}

void ImageManager::AddTexture(const std::string &img_name, const sf::IntRect &tex_rect, const std::string &name_tex,
                              const bool &isRepeated, const bool &isFlipped) {
	if (!m_images.contains(img_name))
		throw std::runtime_error(fmt::format("ImageManager: {} doesnt exist while trying to create texture {}", img_name, name_tex));
	sf::Texture* tex = new sf::Texture();
	ConvertToTexture(tex, img_name, tex_rect, isFlipped);
	tex->setRepeated(isRepeated);
	m_textures[name_tex] = tex;

	MFCPP::Log::InfoPrint(fmt::format("ImageManager: Loaded Texture {}", name_tex));
	//tex->setSmooth(true);
}

void ImageManager::AddTexture(const std::string &img_name, const std::string &name_tex, const bool &isRepeated,
                              const bool &isFlipped) {
	if (!m_images.contains(img_name))
		throw std::runtime_error(fmt::format("ImageManager: {} doesnt exist while trying to create texture {}", img_name, name_tex));
	sf::Texture* tex = new sf::Texture();
	ConvertToTexture(tex, img_name, isFlipped);
	tex->setRepeated(isRepeated);
	m_textures[name_tex] = tex;
	//tex->setSmooth(true);
}
sf::Texture& ImageManager::GetTexture(const std::string& name)
{
	if (!m_textures.contains(name))
		throw std::runtime_error(fmt::format("ImageManager: cannot find {}", name));
	return *m_textures[name];
}
sf::Texture* ImageManager::GetReturnTexture(const std::string& name)
{
	if (!m_textures.contains(name))
		throw std::runtime_error(fmt::format("ImageManager: cannot find {}", name));
	return m_textures[name];
}
void ImageManager::ConvertToTexture(sf::Texture* tex, const std::string& name, const bool& isFlipped) {
	sf::Image inp = m_images[name];
	if (!m_images.contains(name)) {
		throw std::runtime_error(fmt::format("ImageManager: {} doesnt exist", name));
	}
	if (m_images[name].getSize().x == 0 || m_images[name].getSize().y == 0)
		MFCPP::Log::WarningPrint(fmt::format("ImageManager: {} has a width or height equal zero", name));
	if (isFlipped) inp.flipHorizontally();
	if (!tex->loadFromImage(inp))
		throw std::runtime_error(fmt::format("ImageManager: failed to convert {} to texture", name));
}
void ImageManager::ConvertToTexture(sf::Texture* tex, const std::string& name, const sf::IntRect& rect, const bool& isFlipped) {
	sf::Image inp = m_images[name];
	if (!m_images.contains(name)) {
		throw std::runtime_error(fmt::format("ImageManager: {} doesnt exist", name));
	}
	if (m_images[name].getSize().x == 0 || m_images[name].getSize().y == 0)
		MFCPP::Log::WarningPrint(fmt::format("ImageManager: {} has a width or height equal zero", name));
	sf::IntRect rec = rect;
	if (isFlipped) {
		inp.flipHorizontally();
		rec.position.x = inp.getSize().x - rec.position.x - rec.size.x;
	}
	if (!tex->loadFromImage(inp, false, rec))
		throw std::runtime_error(fmt::format("ImageManager: failed to convert {} to texture", name));
}
void ImageManager::CreateTestImage(const std::string& name, const std::string& new_name) {
	const int imageSizeX = static_cast<int>(m_images[name].getSize().x);
	const int imageSizeY = static_cast<int>(m_images[name].getSize().y);
	m_images[new_name] = sf::Image();
	m_images[new_name].resize({m_images[name].getSize().x + 2, m_images[name].getSize().y + 2}, sf::Color::Green);
	if (!m_images[new_name].copy(m_images[name], sf::Vector2u(1,1)))
		throw std::runtime_error(fmt::format("ImageManager: Unexpected Error when fixing {}", new_name));
	if (!m_images[new_name].copy(m_images[name], sf::Vector2u(1,0), sf::IntRect({0, 0}, {imageSizeX, 1})))
		throw std::runtime_error(fmt::format("ImageManager: Unexpected Error when fixing {}", new_name));
	if (!m_images[new_name].copy(m_images[name], sf::Vector2u(1,imageSizeY + 1), sf::IntRect({0, imageSizeY-1}, {imageSizeX, 1})))
		throw std::runtime_error(fmt::format("ImageManager: Unexpected Error when fixing {}", new_name));
	if (!m_images[new_name].copy(m_images[name], sf::Vector2u(0,1), sf::IntRect({0, 0}, {1, imageSizeY})))
		throw std::runtime_error(fmt::format("ImageManager: Unexpected Error when fixing {}", new_name));
	if (!m_images[new_name].copy(m_images[name], sf::Vector2u(imageSizeX + 1,1), sf::IntRect({imageSizeX - 1, 0}, {1, imageSizeY})))
		throw std::runtime_error(fmt::format("ImageManager: Unexpected Error when fixing {}", new_name));

	m_images[new_name].setPixel(sf::Vector2u(0, 0),m_images[name].getPixel(sf::Vector2u(0, 0)));
	m_images[new_name].setPixel(sf::Vector2u(imageSizeX + 1, 0),m_images[name].getPixel(sf::Vector2u(imageSizeX - 1, 0)));
	m_images[new_name].setPixel(sf::Vector2u(0, imageSizeY + 1), m_images[name].getPixel(sf::Vector2u(0, imageSizeY - 1)));
	m_images[new_name].setPixel(sf::Vector2u(imageSizeX + 1, imageSizeY + 1), m_images[name].getPixel(sf::Vector2u(imageSizeX - 1, imageSizeY - 1)));
}