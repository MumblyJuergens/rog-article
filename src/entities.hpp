#pragma once

#include "config.hpp"
#include "renderlist.hpp"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

namespace rog
{

class Floor
{
    std::vector<int> _tile_indices;

  public:
    // In production this would be generated or loaded from a file,
    // currently we just fake it.
    void load() { _tile_indices.resize(config::map_size<size_t> * config::map_size<size_t>, 1); }

    void render_submit(RenderList &render_list)
    {
        for (size_t y = 0; y < config::map_size<size_t>; ++y)
        {
            for (size_t x = 0; x < config::map_size<size_t>; ++x)
            {
                render_list.push_back({
                    .tile_index = _tile_indices[y * config::map_size<size_t> + x],
                    .position = {static_cast<int>(x), static_cast<int>(y)},
                    .layer = 0,
                });
            }
        }
    }
};


class Player
{
    int _tile_index{0};
    SDL_Point _starting_position{2, 2};
    SDL_Point _tile_extent{1, 2};

  public:
    void render_submit(RenderList &render_list)
    {
        render_list.push_back({
            .tile_index = _tile_index,
            .position = _starting_position,
            .tile_extent = _tile_extent,
        });
    }
};

class Goblin
{
    SDL_Point _position;

  public:
    Goblin(int x, int y) : _position{x, y} {}
    void render_submit(RenderList &render_list)
    {
        render_list.push_back({
            .tile_index = 2,
            .position = _position,
        });
    }
};

} // namespace rog
