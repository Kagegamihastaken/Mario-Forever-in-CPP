#ifndef MFCPP_CUSTOMTILEMANAGER_HPP
#define MFCPP_CUSTOMTILEMANAGER_HPP

#include <memory>
#include <queue>
#include <vector>

#include "CustomTile.hpp"
#include "Core/Exception.hpp"
#include "CustomTile/Identity/BonusType.hpp"
#include "CustomTile/Identity/SolidType.hpp"
#include "fmt/format.h"

class CustomTileManager {
public:
    using CustomTileIntrusiveList = boost::intrusive::list<MFCPP::CustomTile, boost::intrusive::constant_time_size<false>>;
    CustomTileManager();
    template <typename T, typename... Args>
    void addCustomTile(Args&&... args) {
        auto newCustomTile = std::make_unique<T>(*this, std::forward<Args>(args)...);

        if constexpr (std::is_base_of_v<MFCPP::BonusType, T>)
            m_Bonus.push_back(*newCustomTile);
        else if constexpr (std::is_base_of_v<MFCPP::SolidType, T>)
            m_Solid.push_back(*newCustomTile);
        else throw MFCPP::Exception::WhichIdentity(fmt::format("Class {} doesn't have any identity.", typeid(*newCustomTile).name()));
        m_customTiles.push_back(std::move(newCustomTile));
    }

    [[nodiscard]] CustomTileIntrusiveList& getBonusList();
    [[nodiscard]] CustomTileIntrusiveList& getSolidList();

    void setDeletionFlag(bool val);
    [[nodiscard]] bool getDeletionFlag() const;
    void setPreviousData() const;
    void interpolateData(float alpha) const;
    void statusUpdate(float deltaTime) const;
    void setCollision(const sf::Vector2f& pos, bool val);
    void updateCollision();
    void HitEvent() const;
    void CustomTileCleanup();
    void DeleteAll();
    void Draw() const;
private:
    struct QueueCustomTileData {
        sf::Vector2f pos;
        bool val{};
    };
    CustomTileIntrusiveList m_Bonus;
    CustomTileIntrusiveList m_Solid;
    std::vector<std::unique_ptr<MFCPP::CustomTile>> m_customTiles;
    bool m_CustomTileDeletionFlag = false;
    std::queue<QueueCustomTileData> m_queue_data;
};

#endif //MFCPP_CUSTOMTILEMANAGER_HPP