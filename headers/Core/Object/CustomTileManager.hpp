#ifndef MFCPP_CUSTOMTILEMANAGER_HPP
#define MFCPP_CUSTOMTILEMANAGER_HPP

#include <memory>
#include <queue>
#include <boost/container/small_vector.hpp>

#include "CustomTile.hpp"
#include "Core/Exception.hpp"
#include "CustomTile/Identity/BonusType.hpp"
#include "CustomTile/Identity/SolidType.hpp"
#include "fmt/format.h"

class CustomTileManager {
public:
    using CustomTileIntrusiveList = boost::intrusive::list<MFCPP::CustomTile, boost::intrusive::constant_time_size<false>>;
    CustomTileManager() = default;
    template <typename T, typename... Args>
    void addCustomTile(Args&&... args) {
        auto newCustomTile = std::make_unique<T>(*this, std::forward<Args>(args)...);

        if constexpr (std::is_base_of_v<MFCPP::BonusType, T>)
            m_Bonus.push_back(*newCustomTile);
        else if constexpr (std::is_base_of_v<MFCPP::SolidType, T>)
            m_Solid.push_back(*newCustomTile);
        else throw MFCPP::Exception::WhichIdentity(fmt::format("Class {} doesn't have any identity.", typeid(*newCustomTile).name()));
        m_customTiles.emplace_back(std::move(newCustomTile));
    }

    [[nodiscard]] CustomTileIntrusiveList& getBonusList();
    [[nodiscard]] CustomTileIntrusiveList& getSolidList();

    void setDeletionFlag(bool val);
    [[nodiscard]] bool getDeletionFlag() const;
    void updatePreviousData() const;
    void statusUpdate(float deltaTime) const;
    void setCollision(const sf::Vector2f& pos, bool val);
    void animationUpdate(float deltaTime) const;
    void updateCollision();
    void HitEvent() const;
    void CustomTileCleanup();
    void DeleteAll();
    void DrawPriority(int index, float alpha) const;
private:
    struct QueueCustomTileData {
        sf::Vector2f pos;
        bool val{};
    };
    CustomTileIntrusiveList m_Bonus;
    CustomTileIntrusiveList m_Solid;
    boost::container::small_vector<std::unique_ptr<MFCPP::CustomTile>, 256> m_customTiles;
    bool m_CustomTileDeletionFlag = false;
    std::queue<QueueCustomTileData> m_queue_data;
};

#endif //MFCPP_CUSTOMTILEMANAGER_HPP