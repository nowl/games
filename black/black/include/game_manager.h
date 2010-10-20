#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

struct game_manager {
    sdl_graphics_context *sdl_driver;
    // font *font_driver;

#define MAX_GAME_STATES 100

    game_state* state;
    unsigned char is_running;
    unsigned int tick;

	int fps;

    void *script_controller;
    int (*script_update_call)(char *, game_manager *, game_object *, int);
    int (*script_render_call)(char *, game_manager *, game_object *, float);
};

struct game_manager *new_game_manager();
void del_game_manager(game_manager *gm);

void game_manager_switch_state(game_manager *gm, game_state* state);

void game_manager_handle_events(game_manager *gm);
void game_manager_update(game_manager *gm, unsigned int ticks);
void game_manager_render(game_manager *gm, float interpolation);

void game_manager_start(game_manager *gm);
void game_manager_quit(game_manager *gm);

#endif /* __GAMEMANAGER_H__ */
