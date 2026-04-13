#ifndef MFCPP_SCENERYMANAGER_HPP
#define MFCPP_SCENERYMANAGER_HPP
#include <memory>
#include <vector>
#include <boost/intrusive/list.hpp>

#include "Scenery.hpp"
#include "Core/Exception.hpp"
#include "fmt/format.h"
#include "Scenery/Identity/GenericSceneryType.hpp"

class SceneryManager {
public:
    using SceneryIntrusiveList = boost::intrusive::list<MFCPP::Scenery, boost::intrusive::constant_time_size<false>>;
    SceneryManager();
    template<typename T, typename... Args>
    void addScenery(Args&&... args) {
        auto newScenery = std::make_unique<T>(*this, std::forward<Args>(args)...);

        if constexpr (std::is_base_of_v<MFCPP::GenericSceneryType, T>)
            m_generic_scenery.push_back(*newScenery);
        else throw MFCPP::Exception::WhichIdentity(fmt::format("Class {} doesn't have any identity.", typeid(*newScenery).name()));
        m_scenery.push_back(std::move(newScenery));
    }
    [[nodiscard]] SceneryIntrusiveList& getGenericScenery();
    [[nodiscard]] bool getDeletionFlag() const;
    void setDeletionFlag(bool val);
    void setPreviousData() const;
    void interpolateData(float alpha) const;
    void draw() const;
    void DeleteAll();
    void SceneryCleanup();
private:
    SceneryIntrusiveList m_generic_scenery;
    bool m_SceneryDeletionFlag = false;
    std::vector<std::unique_ptr<MFCPP::Scenery>> m_scenery;
};

#endif //MFCPP_SCENERYMANAGER_HPP
