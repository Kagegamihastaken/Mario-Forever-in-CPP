#ifndef JSONUTILS_HPP
#define JSONUTILS_HPP

#include <variant>
#include <nlohmann/json.hpp>
#include <SFML/System/Vector2.hpp>

#include "Common.hpp"
#include "Editor/SelectTile.hpp"

namespace sf {
    void to_json(nlohmann::json& j, const Vector2f& v);
    void from_json(const nlohmann::json& j, Vector2f& v);
}
namespace MFCPP {
    void to_json(nlohmann::json& j, const Color& v);
    void from_json(const nlohmann::json& j, Color& v);
}
void to_json(nlohmann::json& j, const TileProperty& prop);
void from_json(const nlohmann::json& j, TileProperty& prop);

#endif //JSONUTILS_HPP
