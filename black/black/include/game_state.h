#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

struct game_object_list_entry {
    game_object *object;

    SLL_ENTRY(game_object_list_entry) _sll_entry;
};

struct game_state {
    int id;

    SLL_LIST(game_object_list, game_object_list_entry) objects;

    //game_state_update_fn update_fn;
    //game_state_event_handle_fn event_handle_fn;
    //game_state_render_fn render_fn;
};

game_state * make_game_state(int id);

void game_state_update(game_manager *, unsigned int ticks);
void game_state_render(game_manager *, float interpolation);

void game_state_append_object(game_state *, game_object *);
struct game_object_list_entry * game_state_remove_object(game_state *, game_object *);
/* TODO: add game_state remove, etc */

#endif /* __GAMESTATE_H__ */
