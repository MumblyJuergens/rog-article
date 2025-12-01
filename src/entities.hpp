#pragma once

#include "renderlist.hpp"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

namespace rog
{

class Entity
{
  public:
    virtual ~Entity() = default;
    virtual void update() {};
    virtual void render([[maybe_unused]] RenderList &render_list) {}
};

class Floor : public Entity
{
  public:
    void render(RenderList &render_list) override
    {
        for (int y{}; y < 30; ++y)
        {
            for (int x{}; x < 30; ++x)
            {
                render_list.push_back({
                    .tile_index = 1,
                    .position = {x, y},
                    .layer = 0,
                });
            }
        }
    }
};

class Player : public Entity
{
  public:
    void render(RenderList &render_list) override
    {
        render_list.push_back({
            .tile_index = 0,
            .position = {2, 2},
            .tile_extent = {1, 2},
        });
    }
};

class Goblin : public Entity
{
    SDL_Point position;

  public:
    Goblin(int x, int y) : position{x, y} {}

    void render(RenderList &render_list) override
    {
        render_list.push_back({
            .tile_index = 2,
            .position = position,
        });
    }
};

} // namespace rog
