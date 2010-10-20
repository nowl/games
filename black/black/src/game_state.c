#include "black.h"

SLL_GENERATE_STATIC(game_object_list, game_object_list_entry, _sll_entry);

game_state *
make_game_state(int id)
{
    game_state *gs = malloc(sizeof(*gs));
    gs->id = id;
    SLL_INIT(&gs->objects);
    return gs;
}

void
game_state_append_object(game_state *gs,
                         game_object *obj)
{
    struct game_object_list_entry *entry = malloc(sizeof(*entry));
    entry->object = obj;
    SLL_ENTRY_INIT(entry, _sll_entry);
    game_object_list_SLL_APPEND(&gs->objects, entry);
}

static int game_object_list_entry_cmp(void *obj, struct game_object_list_entry *entry)
{
    /* TODO: check if these is correct, before it was using strcmp() */
    return obj == entry->object;
}

struct game_object_list_entry *
game_state_remove_object(game_state *gs,
                         game_object *obj)
{
    return game_object_list_SLL_REMOVE(&gs->objects, obj, game_object_list_entry_cmp);
}

void
game_state_render(game_manager *gm, float interpolation)
{
    game_state *gs = gm->state;

    black_sdl_free_font_surfaces();

    struct game_object_list_entry * entry = SLL_HEAD(&gs->objects);
    while(entry) {
        game_object *object = entry->object;

        struct render_callback *callback = render_callback_list_SLL_NEXT(&object->render_callbacks, NULL);
        while(callback) {

			switch(callback->type)
            {
            case C_FUNC:
                callback->cb.c_func(gm, object, interpolation);
                break;
            case SCRIPT_FUNC:
                gm->script_render_call(callback->cb.script_func, gm, object, interpolation);
                break;
            default:
                assert(FALSE);
			}

            // go to the next one
            callback = render_callback_list_SLL_NEXT(&object->render_callbacks, callback);
        }

		/*
		if(object->image)
			black_sdl_draw_image(gm, object->image, object->screenx, object->screeny);
		*/

        entry = game_object_list_SLL_NEXT(&gs->objects, entry);
    }

    black_sdl_flip(gm);
}

void
game_state_update(game_manager *gm, unsigned int ticks)
{
    game_state *gs = gm->state;

    struct game_object_list_entry * entry = SLL_HEAD(&gs->objects);
    while(entry) {
        game_object *object = entry->object;

        // process message list
        struct message *message = message_list_SLL_NEXT(&object->message_list, NULL);
        while(message) {
            message->callback_func(message->sender, message->receiver);

            message = message_list_SLL_NEXT(&object->message_list, message);
        }

        // clear message list
        game_object_clear_messages(object);

        // process update callbacks

        struct update_callback *callback = update_callback_list_SLL_NEXT(&object->update_callbacks, NULL);
        while(callback) {

            // call the update function, it is the responsibility of the update function to continue popping events
            // as it needs them and to even exhaust them all if they are applicable to that update function

            switch(callback->type)
            {
            case C_FUNC:
                callback->cb.c_func(gm, object, ticks);
                break;
            case SCRIPT_FUNC:
                gm->script_update_call(callback->cb.script_func, gm, object, ticks);
                break;
            default:
                assert(FALSE);
            }

            // go to the next one
            callback = update_callback_list_SLL_NEXT(&object->update_callbacks, callback);
        }

        entry = game_object_list_SLL_NEXT(&gs->objects, entry);
    }
}
