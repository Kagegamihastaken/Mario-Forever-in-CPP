#ifndef FLOATHASH_HPP
#define FLOATHASH_HPP

#include <SFML/System/Vector2.hpp>
#include <boost/container_hash/hash.hpp>
#include "Editor/RenderTile.hpp"
#include "Class/PiranhaAIClass.hpp"

struct RenderTileHash {
    std::size_t operator()(const RenderTile& p) const {
        std::size_t seed = 0;
        boost::hash_combine(seed, p.getPosition().x);
        boost::hash_combine(seed, p.getPosition().y);
        return seed;
    }
};
struct RenderTileEqual {
    bool operator()(const RenderTile& a, const RenderTile& b) const {
        constexpr float epsilon = 1e-6f;
        return std::abs(a.getPosition().x - b.getPosition().x) < epsilon &&
               std::abs(a.getPosition().y - b.getPosition().y) < epsilon;
    }
};

#endif //FLOATHASH_HPP
