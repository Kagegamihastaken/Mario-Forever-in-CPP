#include "Core/TextureConfig.hpp"
#include <toml++/toml.hpp>

#include "Core/AnimationSequenceManager.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Logging.hpp"
#include "Core/Loading/Loading.hpp"

namespace MFCPP {
    void TextureConfig::loadFile(const std::filesystem::path& path) {
        std::string TOMLFileRaw;
        IO::LoadRaw(TOMLFileRaw, path);
        toml::table TOMLFile = toml::parse(TOMLFileRaw);

        toml::array* textureArray = TOMLFile["textures"].as_array();
        for (auto& textureNode : *textureArray) {
            toml::table* CurrentTexture = textureNode.as_table();

            //ID Texture & Path Texture
            const toml::node* idNode = CurrentTexture->get("id");
            const toml::node* pathNode = CurrentTexture->get("path");

            if (!idNode || !pathNode)
                throw std::runtime_error("TextureConfig: Missing 'id' or 'path' in texture configuration in TOML file: " + path.string());

            std::string name = idNode->value_or("__NULL");
            std::string filePath = pathNode->value_or("");

            if (name == "__NULL" || filePath.empty())
                throw std::runtime_error("TextureConfig: Invalid texture configuration in TOML file: " + path.string());

            //Check valid size & is Animated
            const toml::node* animatedNode = CurrentTexture->get("animated");
            const toml::node* widthNode = CurrentTexture->get("width");
            const toml::node* heightNode = CurrentTexture->get("height");

            if (!animatedNode || !widthNode || !heightNode)
                throw std::runtime_error("TextureConfig: Missing 'animated', 'width', or 'height' in texture configuration for texture '" + name + "' in TOML file: " + path.string());

            bool animated = animatedNode->value_or(false);
            int32_t width = widthNode->value_or(0);
            int32_t height = heightNode->value_or(0);

            // x, y is optional, default to 0 if not provided
            int32_t x = CurrentTexture->get("x") ? CurrentTexture->get("x")->value_or(0) : 0;
            int32_t y = CurrentTexture->get("y") ? CurrentTexture->get("y")->value_or(0) : 0;
            bool isRepeated = CurrentTexture->get("is_repeated") ? CurrentTexture->get("is_repeated")->value_or(false) : false;
            if (animated) {
                AnimationSequenceManager::newData(name);
                bool customAnimated = false;
                const toml::node* customAnimatedNode = CurrentTexture->get("custom_animated");
                if (customAnimatedNode) {
                    const toml::array* arr = customAnimatedNode->as_array();
                    if (arr && !arr->empty()) {
                        customAnimated = true;
                        for (const auto& frame : *arr) {
                            if (frame.is_integer())
                                AnimationSequenceManager::addSingleFrame(name, fmt::format("{}_{}", name, frame.value_or(0)));
                            else
                                Log::WarningPrint(fmt::format("TextureConfig: Invalid frame in animation sequence for texture '{}'. Frame should be an integer.", name));
                        }
                    }
                }
                // If animated, check image_width and create frames based on width
                const toml::node* imageWidthNode = CurrentTexture->get("image_width");
                if (!imageWidthNode)
                    throw std::runtime_error("TextureConfig: Missing 'image_width' in texture configuration for texture with animated enabled '" + name + "' in TOML file: " + path.string());
                int32_t image_width = imageWidthNode->value_or(0);
                for (int i = 0; i < image_width / width; ++i) {
                    ImageManager::AddTexture(fmt::format("{}_{}", name, i), filePath, sf::IntRect({i * width + x, y}, {width, height}), isRepeated);
                    if (!customAnimated) AnimationSequenceManager::addSingleFrame(name, fmt::format("{}_{}", name, i));
                }
            } else {
                ImageManager::AddTexture(name, filePath, sf::IntRect({x, y}, {width, height}), isRepeated);
            }
        }
    }
}
