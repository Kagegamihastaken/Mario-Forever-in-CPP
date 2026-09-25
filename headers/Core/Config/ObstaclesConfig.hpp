#ifndef MFCPP_OBSTACLESCONFIG_HPP
#define MFCPP_OBSTACLESCONFIG_HPP

namespace MFCPP {
    namespace ObstacleConfig {
        struct ObstacleData {
            int32_t tex_x, tex_y, collision_id, y_low, y_high;
        };
        void loadFile(const std::filesystem::path& path, bool force_preload = true);
        ObstacleData getData(int32_t id);
    }
}

#endif //MFCPP_OBSTACLESCONFIG_HPP
