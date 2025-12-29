#ifndef MFCPP_ENEMYMANAGER_HPP
#define MFCPP_ENEMYMANAGER_HPP

#include <memory>
#include <vector>
#include <type_traits>
#include <typeinfo>

#include "Core/Exception.hpp"
#include "Core/Logging.hpp"
#include "Core/Object/Enemy.hpp"
#include "Core/Object/GoombaAIType.hpp"

class EnemyManager {
public:
    using EnemyIntrusiveList = boost::intrusive::list<MFCPP::Enemy, boost::intrusive::constant_time_size<false>>;
    EnemyManager();
    template <typename T, typename... Args>
    void addEnemy(Args&&... args) {
        auto newEnemy = std::make_unique<T>(*this, std::forward<Args>(args)...);

        if constexpr (std::is_base_of_v<MFCPP::GoombaAIType, T>)
            m_GoombaAI.push_back(*newEnemy);
        else throw MFCPP::Exception::WhichIdentity(fmt::format("Class {} doesn't have any identity.", typeid(*newEnemy).name()));
        m_enemies.push_back(std::move(newEnemy));
    }

    [[nodiscard]] EnemyIntrusiveList& getGoombaAIList();
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
    void DrawHighPriority() const;
    void DrawLowPriority() const;
private:
    // For EnemyType Only
    EnemyIntrusiveList m_GoombaAI;
    bool m_EnemyDeletionFlag = false;
    std::vector<std::unique_ptr<MFCPP::Enemy>> m_enemies;

};

#endif //MFCPP_ENEMYMANAGER_HPP