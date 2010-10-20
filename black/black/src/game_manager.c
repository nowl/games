#include "black.h"

game_manager *
new_game_manager()
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        WARN( "Unable to init SDL: %s\n", SDL_GetError() );
        exit(1);
    }

    int r = IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
    LOG("result of IMG_Init: %d\n", r);

    r = TTF_Init();
    LOG("result of TTF_Init: %d\n", r);

    r = Mix_Init(MIX_INIT_OGG);
    LOG("result of MIX_Init: %d\n", r);

    Mix_AllocateChannels(16);
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        LOG("Mix_OpenAudio error: %s\n", Mix_GetError());
        exit(2);
    }

    struct game_manager* gm = malloc(sizeof(*gm));
    gm->sdl_driver = malloc(sizeof(*gm->sdl_driver));
    gm->is_running = TRUE;
    gm->state = NULL;
	gm->fps = 0;
    return gm;
}

void
del_game_manager(game_manager *gm)
{
    free(gm->sdl_driver);
    free(gm);

    sdl_font_cleanup();
    image_loader_cleanup();
    sound_loader_cleanup();

    Mix_CloseAudio();

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void
game_manager_switch_state(game_manager *gm, game_state* state)
{
    gm->state = state;
}

void game_manager_handle_events(game_manager *gm)
{
    // message processing loop
    SDL_Event event;
    while (SDL_PollEvent(&event))
        event_queue_push(&event);
}

void game_manager_update(game_manager *gm, unsigned int ticks)
{
    gm->tick = ticks;

    if(gm->state)
        game_state_update(gm, ticks);
}

void game_manager_render(game_manager *gm, float interpolation)
{
    if(gm->state)
        game_state_render(gm, interpolation);
}

void game_manager_start(game_manager *gm) {}
void game_manager_quit(game_manager *gm) {gm->is_running = FALSE;}
