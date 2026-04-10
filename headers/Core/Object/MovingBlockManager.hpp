#ifndef MFCPP_MOVINGBLOCKMANAGER_HPP
#define MFCPP_MOVINGBLOCKMANAGER_HPP

#include <memory>
#include <vector>
#include <boost/intrusive/list.hpp>

#include "MovingBlock.hpp"
#include "Core/Exception.hpp"
#include "fmt/color.h"
#include "MovingBlock/Identity/PlatformType.hpp"

class MovingBlockManager {
public:
    using MovingBlockIntrusiveList = boost::intrusive::list<MFCPP::MovingBlock, boost::intrusive::constant_time_size<false>>;
    MovingBlockManager();
    template<typename T, typename... Args>
    void addMovingBlock(Args&&... args) {
        auto newMovingBlock = std::make_unique<T>(*this, std::forward<Args>(args)...);

        if constexpr (std::is_base_of_v<MFCPP::PlatformType, T>)
            m_platforms.push_back(*newMovingBlock);
        else throw MFCPP::Exception::WhichIdentity(fmt::format("Class {} doesn't have any identity.", typeid(*newMovingBlock).name()));
        m_movingblocks.push_back(std::move(newMovingBlock));
    }
    [[nodiscard]] MovingBlockIntrusiveList& getPlatformList();
    [[nodiscard]] bool getDeletionFlag() const;
    void setDeletionFlag(bool val);
    void setPreviousData() const;
    void interpolateData(float alpha) const;
    void statusUpdate(float deltaTime) const;
    void draw() const;
    void DeleteAll();
    void MovingBlockCleanup();
private:
    MovingBlockIntrusiveList m_platforms;
    bool m_MovingBLockDeletionFlag = false;
    std::vector<std::unique_ptr<MFCPP::MovingBlock>> m_movingblocks;
};

#endif //MFCPP_MOVINGBLOCKMANAGER_HPP