#ifndef MFCPP_ENEMYMANAGER_HPP
#define MFCPP_ENEMYMANAGER_HPP

#include <memory>
#include <vector>
#include <type_traits>
#include <typeinfo>

#include "Enemy/Identity/BroAIType.hpp"
#include "Enemy/Identity/BulletBillAIType.hpp"
#include "Enemy/Identity/PiranhaAIType.hpp"
#include "Enemy/Identity/RotodiscAIType.hpp"
#include "Core/Exception.hpp"
#include "Core/Logging.hpp"
#include "Core/Object/Enemy.hpp"
#include "Enemy/Identity/GoombaAIType.hpp"
#include "Enemy/Identity/SpikeAIType.hpp"

class EnemyManager {
public:
    using EnemyIntrusiveList = boost::intrusive::list<MFCPP::Enemy, boost::intrusive::constant_time_size<false>>;
    EnemyManager();
    template <typename T, typename... Args>
    void addEnemy(Args&&... args) {
        auto newEnemy = std::make_unique<T>(*this, std::forward<Args>(args)...);

        if constexpr (std::is_base_of_v<MFCPP::GoombaAIType, T>)
            m_GoombaAI.push_back(*newEnemy);
        else if constexpr (std::is_base_of_v<MFCPP::BulletBillAIType, T>)
            m_BulletBillAI.push_back(*newEnemy);
        else if constexpr (std::is_base_of_v<MFCPP::RotodiscAIType, T>)
            m_RotodiscAI.push_back(*newEnemy);
        else if constexpr (std::is_base_of_v<MFCPP::BroAIType, T>)
            m_BroAI.push_back(*newEnemy);
        else if constexpr (std::is_base_of_v<MFCPP::PiranhaAIType, T>)
            m_PiranhaAI.push_back(*newEnemy);
        else if constexpr (std::is_base_of_v<MFCPP::SpikeAIType, T>)
            m_SpikeAI.push_back(*newEnemy);
        else throw MFCPP::Exception::WhichIdentity(fmt::format("Class {} doesn't have any identity.", typeid(*newEnemy).name()));
        m_enemies.push_back(std::move(newEnemy));
    }

    [[nodiscard]] EnemyIntrusiveList& getGoombaAIList();
    [[nodiscard]] EnemyIntrusiveList& getBroAIList();
    [[nodiscard]] EnemyIntrusiveList& getPiranhaAIList();
    [[nodiscard]] bool getDeletionFlag() const;
    void setDeletionFlag(bool val);
    void setPreviousData() const;
    void interpolateData(float alpha) const;
    void statusUpdate(float deltaTime) const;
    void MarioCollision() const;
    void XUpdate(float deltaTime) const;
    void YUpdate(float deltaTime) const;
    void EnemyCollision();
    void EnemyCleanup();
    void DeleteAll();
    void DrawPriority(int index) const;
private:
    // For EnemyType Only
    EnemyIntrusiveList m_GoombaAI;
    EnemyIntrusiveList m_BulletBillAI;
    EnemyIntrusiveList m_RotodiscAI;
    EnemyIntrusiveList m_BroAI;
    EnemyIntrusiveList m_PiranhaAI;
    EnemyIntrusiveList m_SpikeAI;
    bool m_EnemyDeletionFlag = false;
    std::vector<std::unique_ptr<MFCPP::Enemy>> m_enemies;

};

#endif //MFCPP_ENEMYMANAGER_HPP