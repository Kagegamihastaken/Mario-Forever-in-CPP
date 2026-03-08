#ifndef MFCPP_CUSTOMTILETYPE_HPP
#define MFCPP_CUSTOMTILETYPE_HPP

namespace MFCPP {
    enum CustomTileTypeEnum {BONUS, SOLID};

    class CustomTileType {
    public:
        CustomTileType() = default;
        virtual CustomTileTypeEnum getCustomTileType() = 0;
        virtual ~CustomTileType() = default;
    };
}
#endif //MFCPP_CUSTOMTILETYPE_HPP