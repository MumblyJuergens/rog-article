#include "game.hpp"
#include <SDL3/SDL_stdinc.h>
#include <memory>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_AppResult SDL_AppInit(void **appstate, [[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    auto game = std::make_unique<rog::Game>();

    if (argc == 2 && SDL_strcmp(argv[1], "fpstest") == 0)
    {
        game->set_fps_testing(true);
    }

    game->setup_level();

    *appstate = game.release();
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    rog::Game &game = *static_cast<rog::Game *>(appstate);

    game.on_event(event);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    rog::Game &game = *static_cast<rog::Game *>(appstate);

    game.update();
    game.render();

    if (game.should_quit()) return SDL_APP_SUCCESS;

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    rog::Game *const game = static_cast<rog::Game *>(appstate);
    delete game;

    if (result == SDL_APP_SUCCESS)
    {
        SDL_Log("Quitting with happy state :)");
    } else if (result == SDL_APP_FAILURE)
    {
        SDL_Log("Quitting with failure state :(");
    }
}
