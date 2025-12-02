#pragma once

#include "config.hpp"
#include "renderlist.hpp"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

namespace rog
{

class Entity
{
  public:
    virtual ~Entity() = default;
    virtual void update() {};
    virtual void render_submit([[maybe_unused]] RenderList &render_list) {}
};

class Floor
{
    std::vector<int> _tile_indices;
    std::vector<SDL_Vertex> _vertices;
    static constexpr SDL_Point _map_size{30, 30};

  public:
    // In production this would be generated or loaded from a file,
    // currently we just fake it.
    void load() { _tile_indices.resize(_map_size.x * _map_size.y, 1); }

    void prepare_geometry(SDL_Point tilemap_size)
    {
        std::vector<SDL_Vertex> vertices(30 * 30 * 6);
        const SDL_FColor color{1.0f, 1.0f, 1.0f, 1.0f};

        const SDL_FPoint uv_tile_size{
            .x = 1.0f / static_cast<float>(tilemap_size.x),
            .y = 1.0f / static_cast<float>(tilemap_size.y),
        };

        for (size_t n = 0; n < _map_size.x * _map_size.y; ++n)
        {

            const SDL_FPoint pos = {static_cast<float>(n % _map_size.x) * config::tile_size<float>,
                                    static_cast<float>(n / _map_size.x) * config::tile_size<float>};

            const SDL_FPoint uv_offset{
                .x = static_cast<float>(_tile_indices[n] % tilemap_size.x) * uv_tile_size.x,
                .y = static_cast<float>(_tile_indices[n] / tilemap_size.x) * uv_tile_size.y,
            };

            const size_t i = n * 6;

            // Left-Top.
            vertices[i + 0] = {
                .position = {pos.x, pos.y},
                .color = color,
                .tex_coord = {uv_offset.x, uv_offset.y},
            };
            // Right-Top
            vertices[i + 1] = {
                .position = {pos.x + config::tile_size<float>, pos.y},
                .color = color,
                .tex_coord = {uv_offset.x + uv_tile_size.x, uv_offset.y},
            };
            // Left-Bottom.
            vertices[i + 2] = {
                .position = {pos.x, pos.y + config::tile_size<float>},
                .color = color,
                .tex_coord = {uv_offset.x, uv_offset.y + uv_tile_size.y},
            };
            // Right-Top
            vertices[i + 3] = {
                .position = {pos.x + config::tile_size<float>, pos.y},
                .color = color,
                .tex_coord = {uv_offset.x + uv_tile_size.x, uv_offset.y},
            };
            // Right-Bottom.
            vertices[i + 4] = {
                .position = {pos.x + config::tile_size<float>, pos.y + config::tile_size<float>},
                .color = color,
                .tex_coord = {uv_offset.x + uv_tile_size.x, uv_offset.y + uv_tile_size.y},
            };
            // Left-Bottom.
            vertices[i + 5] = {
                .position = {pos.x, pos.y + config::tile_size<float>},
                .color = color,
                .tex_coord = {uv_offset.x, uv_offset.y + uv_tile_size.y},
            };
        }

        _vertices.swap(vertices);
    }

    void render(SDL_Renderer *renderer, SDL_Texture *texture)
    {
        SDL_RenderGeometry(renderer, texture, _vertices.data(), static_cast<int>(_vertices.size()), nullptr, 0);
    }
};

class Player : public Entity
{
  public:
    void render_submit(RenderList &render_list) override
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

    void render_submit(RenderList &render_list) override
    {
        render_list.push_back({
            .tile_index = 2,
            .position = position,
        });
    }
};

} // namespace rog
