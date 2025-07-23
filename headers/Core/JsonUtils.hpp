#ifndef JSONUTILS_HPP
#define JSONUTILS_HPP

#include <variant>
#include <nlohmann/json.hpp>
#include <SFML/System/Vector2.hpp>

#include "Common.hpp"
#include "Editor/SelectTile.hpp"

namespace sf {
    extern void to_json(nlohmann::json& j, const Vector2f& v);
    extern void from_json(const nlohmann::json& j, Vector2f& v);
}
namespace MFCPP {
    extern void to_json(nlohmann::json& j, const Color& v);
    extern void from_json(const nlohmann::json& j, Color& v);
}
extern void to_json(nlohmann::json& j, const TileProperty& prop);
extern void from_json(const nlohmann::json& j, TileProperty& prop);

#endif //JSONUTILS_HPP
