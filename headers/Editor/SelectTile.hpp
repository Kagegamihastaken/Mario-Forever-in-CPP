#ifndef SELECTTILE_HPP
#define SELECTTILE_HPP

#include <cstdarg>

#include "Editor/TileProperty.hpp"
#include "Core/Logging.hpp"
#include "Editor/TabButton.hpp"
using TileProperty = std::variant<IntProps, BoolProps, StringProps, FloatProps, Vector2fProps>;
class CustomTileProperty {
public:
    explicit CustomTileProperty(const TileProperty &arg) {
        PushProperty(arg);
    }
    explicit CustomTileProperty() = default;
    template<typename... Args>
    explicit CustomTileProperty(const TileProperty& first_args, const Args&... args) {
        PushProperty(first_args);
        (PushProperty(args), ...);
    }
    [[nodiscard]] size_t getPropertyCount() const {
        return m_prop_size;
    }
    [[nodiscard]] const TileProperty* at(const size_t index) const {
        return &m_prop.at(index);
    }
    [[nodiscard]] TileProperty* at(const size_t index) {
        return &m_prop.at(index);
    }
    ~CustomTileProperty() = default;
private:
    void PushProperty(const TileProperty& arg) {
        if (m_prop_size >= m_prop.size()) return;
        m_prop[m_prop_size++] = arg;
    }
    std::array<TileProperty, 4> m_prop;
    size_t                      m_prop_size = 0;
};
struct SelectTileData {
    std::string name;
    CustomTileProperty prop;
    sf::Vector2f position;
    sf::Vector2i texPos = sf::Vector2i(0, 0);
    //other
    sf::Vector2f origin = sf::Vector2f(0.0f, 0.0f);
    int categoryID;
    int objectID;
    int customID1 = 0;
    int customID2 = 0;
};
extern bool EDITOR_SELECTTILE;
extern sf::Clock EDITOR_SELECTILE_CLOCK;
extern const std::array<std::vector<SelectTileData>, 5> TilePage;
extern std::array<MFCPP::TabButton, 5> TabList;

extern int CurrSelectTile;
extern int PrevSelectTile;
extern int CurrPage;
extern int PrevPage;
extern int PreviewPage;

constexpr int LevelTab = 3;
constexpr int PlatformTab = 4;

extern void SelectTileInit();
extern void SelectTilePosUpdate();
extern void SelectTileDraw();
extern void SelectTileAlphaUpdate(float dt);
extern void SelectTileDisplayUpdate();
extern int CheckExistPos();

#endif //SELECTTILE_HPP
