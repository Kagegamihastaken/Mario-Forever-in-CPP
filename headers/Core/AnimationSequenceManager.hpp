#ifndef MFCPP_ANIMATIONSEQUENCEMANAGER_HPP
#define MFCPP_ANIMATIONSEQUENCEMANAGER_HPP

#include <string>
#include <boost/container/static_vector.hpp>

namespace MFCPP {
    using AnimationSequence = boost::container::static_vector<std::string, 32>;
    namespace AnimationSequenceManager {
        AnimationSequence* getData(std::string_view name);
        void addSingleFrame(std::string_view name, std::string_view val);
        void newData(std::string_view name);
        void clearData(std::string_view name);
    };
}

#endif //MFCPP_ANIMATIONSEQUENCEMANAGER_HPP
