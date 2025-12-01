#pragma once

#include "config.hpp"
#include <SDL3/SDL_rect.h>
#include <set>

namespace rog
{

struct RenderItem
{
    int tile_index;
    SDL_Point position;
    int layer{10};
    SDL_Point tile_extent{1, 1};

    [[nodiscard]] constexpr int screen_x() const noexcept
    {
        return position.x * config::tile_size<int> - tile_extent.x * config::tile_size<int>;
    }
    
    [[nodiscard]] constexpr int screen_y() const noexcept
    {
        return position.y * config::tile_size<int> - tile_extent.y * config::tile_size<int>;
    }

    [[nodiscard]] constexpr bool operator<(const RenderItem &other) const noexcept
    {
        return layer < other.layer || (layer == other.layer && screen_y() < other.screen_y());
    }
};

using RenderList = std::multiset<RenderItem>;

} // namespace rog
