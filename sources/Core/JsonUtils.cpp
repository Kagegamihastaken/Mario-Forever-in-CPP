#include "Core/JsonUtils.hpp"

namespace sf {
    void to_json(nlohmann::json& j, const Vector2f& v) {
        j = {{"x", v.x}, {"y", v.y}};
    }
    void from_json(const nlohmann::json& j, Vector2f& v) {
        j.at("x").get_to(v.x);
        j.at("y").get_to(v.y);
    }
}
namespace MFCPP {
    void to_json(nlohmann::json& j, const Color& v) {
        j = {{"r", v.r}, {"g", v.g}, {"b", v.b}, {"a", v.a}};
    }
    void from_json(const nlohmann::json& j, Color& v) {
        j.at("r").get_to(v.r);
        j.at("g").get_to(v.g);
        j.at("b").get_to(v.b);
        j.at("a").get_to(v.a);
    }
}
void to_json(nlohmann::json& j, const TileProperty& prop) {
    std::visit([&]<typename T0>(T0&& arg) {
        using T = std::decay_t<T0>;
        if constexpr (std::is_same_v<T, BoolProps>)
            j[arg.name] = arg.val;
        else if constexpr (std::is_same_v<T, IntProps>)
            j[arg.name] = arg.val;
        else if constexpr (std::is_same_v<T, StringProps>)
            j[arg.name] = arg.val;
        else if constexpr (std::is_same_v<T, FloatProps>)
            j[arg.name] = arg.val;
        else if constexpr (std::is_same_v<T, Vector2fProps>)
            j[arg.name] = arg.val;
        else if constexpr (std::is_same_v<T, ColorProps>)
            j[arg.name] = arg.val;
    }, prop);
}
void from_json(const nlohmann::json& j, TileProperty& prop) {
    std::visit([&]<typename T0>(T0&& arg) {
        using T = std::decay_t<T0>;
        if (j.contains(arg.name)) {
            if constexpr (std::is_same_v<T, BoolProps>) {
                j.at(arg.name).get_to(arg.val);
            } else if constexpr (std::is_same_v<T, IntProps>) {
                j.at(arg.name).get_to(arg.val);
            } else if constexpr (std::is_same_v<T, StringProps>) {
                const std::string s = j.at(arg.name).template get<std::string>();
                strncpy(arg.val, s.c_str(), sizeof(arg.val) - 1);
                arg.val[sizeof(arg.val) - 1] = '\0';
            } else if constexpr (std::is_same_v<T, FloatProps>) {
                j.at(arg.name).get_to(arg.val);
            } else if constexpr (std::is_same_v<T, Vector2fProps>) {
                j.at(arg.name).get_to(arg.val);
            } else if constexpr (std::is_same_v<T, ColorProps>) {
                j.at(arg.name).get_to(arg.val);
            }
        }
    }, prop);
}