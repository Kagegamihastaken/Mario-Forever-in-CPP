#ifndef MFCPP_SCENERYTYPE_HPP
#define MFCPP_SCENERYTYPE_HPP

namespace MFCPP {
    enum SceneryTypeEnum {GENERIC_SCENERY};

    class SceneryType {
    public:
        SceneryType() = default;
        virtual SceneryTypeEnum getSceneryType() = 0;
        virtual ~SceneryType() = default;
    };
}

#endif //MFCPP_SCENERYTYPE_HPP
