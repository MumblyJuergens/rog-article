#pragma once

#include "config.hpp"
#include "entities.hpp"
#include "renderlist.hpp"
#include "timer.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cstdint>
#include <fstream>
#include <memory>
#include <vector>

namespace rog
{

class Game
{
    bool _should_quit = false;
    bool _is_fps_testing = false;
    RenderList _render_items;
    std::vector<std::unique_ptr<Entity>> _entities;
    Floor _floor;

    SDL_Window *_window{};
    SDL_Renderer *_renderer{};

    SDL_Texture *_tilemap{};
    SDL_FPoint _tmap_size{};
    SDL_Point _tmap_size_int{};

    // I know this seems like a lot to get good averaged data,
    // it isn't necessary for production code.
    FPSTimer _fps_timer;
    DurationTimer _microsecs_work;
    Averager<int> _fps_averager;
    Averager<double> _microsec_averager;
    uint64_t _frames;

  public:
    Game()
    {
        SDL_CreateWindowAndRenderer("Rog", 800, 600, SDL_WINDOW_RESIZABLE, &_window, &_renderer);

        SDL_SetRenderScale(_renderer, 2.0f, 2.0f);

        std::ifstream file{"res/tilemap.png", std::ios::binary};
        std::vector<char> tmap_data{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
        SDL_Surface *tmap_surface = IMG_Load("res/tilemap.png");
        _tilemap = SDL_CreateTextureFromSurface(_renderer, tmap_surface);

        SDL_SetTextureScaleMode(_tilemap, SDL_SCALEMODE_NEAREST);
        SDL_GetTextureSize(_tilemap, &_tmap_size.x, &_tmap_size.y);
        _tmap_size_int = {static_cast<int>(_tmap_size.x) / config::tile_size<int>,
                          static_cast<int>(_tmap_size.y) / config::tile_size<int>};

        // Application should print a bunch of 1's on startup, if you get 0's something has gone bad.
        SDL_Log("Verify load: %d %d %d %d %d %d %d %d", _window != nullptr, _renderer != nullptr, file.good(),
                tmap_data.size() > 0, tmap_surface != nullptr, _tilemap != nullptr, _tmap_size.x > 0, _tmap_size.y > 0);

        SDL_DestroySurface(tmap_surface);
    }

    ~Game()
    {
        SDL_DestroyTexture(_tilemap);
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
    }

    void on_event(SDL_Event *const event)
    {
        if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE)
        {
            _should_quit = true;
        }
    }

    void setup_level()
    {
        _floor.load();
        _floor.prepare_geometry(_tmap_size_int);
        _entities.push_back(std::make_unique<Player>());
        _entities.push_back(std::make_unique<Goblin>(4, 2));
    }

    void update()
    {
        _fps_timer.fps_tick();
        if (_is_fps_testing) _microsecs_work.begin_activity();

        for (auto &entity : _entities)
        {
            entity->update();
        }
    }

    void render()
    {
        for (auto &entity : _entities)
        {
            entity->render_submit(_render_items);
        }

        std::sort(_render_items.begin(), _render_items.end());

        SDL_RenderClear(_renderer);
        _floor.render(_renderer, _tilemap);

        for (auto &item : _render_items)
        {
            SDL_FRect src{
                .x = static_cast<float>(item.tile_index % _tmap_size_int.x) * config::tile_size<float>,
                .y = static_cast<float>(item.tile_index / _tmap_size_int.x) * config::tile_size<float>,
                .w = config::tile_size<float> * static_cast<float>(item.tile_extent.x),
                .h = config::tile_size<float> * static_cast<float>(item.tile_extent.y),
            };
            SDL_FRect dst{
                .x = static_cast<float>(item.screen_x()),
                .y = static_cast<float>(item.screen_y()),
                .w = config::tile_size<float> * static_cast<float>(item.tile_extent.x),
                .h = config::tile_size<float> * static_cast<float>(item.tile_extent.y),
            };

            // Actually adds to SDL internal batch renderer.
            SDL_RenderTexture(_renderer, _tilemap, &src, &dst);
        }
        _render_items.clear();

        if (_is_fps_testing) _microsecs_work.end_activity();

        SDL_RenderPresent(_renderer);

        if (!_is_fps_testing) return;

        if (_frames++ % 500 == 0)
        {
            const int fpsavg = _fps_timer.average_fps();
            const double microavg = _microsecs_work.average();
            _fps_averager.add(fpsavg);
            _microsec_averager.add(microavg);
            SDL_Log("FPS: %d | µs: %f", fpsavg, microavg);
        }
        if (_frames == 70'000)
        {
            SDL_Log("Hot Averages: FPS: %d | µs: %f", _fps_averager.get(), _microsec_averager.get());
            _should_quit = true;
        }
    }

    [[nodiscard]] bool should_quit() const noexcept { return _should_quit; }
    void set_fps_testing(bool value) noexcept { _is_fps_testing = value; }
};

} // namespace rog
