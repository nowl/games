#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "tree.h"
#include "list.h"

enum callback_types
{
    C_FUNC,
    SCRIPT_FUNC
};

struct update_callback {
    enum callback_types type;

    union {
        game_object_update_fn c_func;
        char *script_func;
    } cb;

    SLL_ENTRY(update_callback) _sll_entry;
};

struct render_callback {
    enum callback_types type;

    union {
        game_object_render_fn c_func;
        char *script_func
    } cb;

    SLL_ENTRY(render_callback) _sll_entry;
};

struct game_object
{
    unsigned long id;
    unsigned int type;
    void *data;
	SDL_Surface *image;
	int screenx;
	int screeny;

    /* the structure used to include this in an rb
     * tree for easy retreival */
    RB_ENTRY(game_object) _rb_tree_entry;

    SLL_LIST(update_callback_list, update_callback) update_callbacks;
    SLL_LIST(render_callback_list, render_callback) render_callbacks;
    SLL_LIST(message_list, message) message_list;
};

SLL_PROTOTYPE(update_callback_list, update_callback, _sll_entry);
SLL_PROTOTYPE(render_callback_list, render_callback, _sll_entry);
SLL_PROTOTYPE(message_list, message, _sll_entry);

/* game object creation, destruction, retrieval */
game_object * make_game_object(unsigned int type, void *data);
void destroy_game_object(struct game_manager *gm, game_object *go);
game_object * game_object_get(int id);
game_object * game_object_remove(game_object *obj);
game_object * game_object_remove_by_id(int id);

/* update callback functions */
void
game_object_append_update_callback_c_func(game_object *obj,
                                          const game_object_update_fn callback);

struct update_callback *
game_object_remove_update_callback_c_func(game_object *obj,
                                          const game_object_update_fn callback);

void
game_object_append_update_callback_script_func(game_object *obj,
                                          char *callback);

struct update_callback *
game_object_remove_update_callback_script_func(game_object *obj,
                                          char *callback);


void
game_object_clear_update_callbacks(game_object *obj);

struct update_callback *
game_object_update_callback_next(game_object *obj,
                                 struct update_callback *cur);

/* render callback functions */
void
game_object_append_render_callback_c_func(game_object *obj,
                                   const game_object_render_fn callback);

struct render_callback *
game_object_remove_render_callback_c_func(game_object *obj,
                                   const game_object_render_fn callback);

void
game_object_append_render_callback_script_func(game_object *obj,
                                   char * callback);

struct render_callback *
game_object_remove_render_callback_script_func(game_object *obj,
                                   char * callback);

void
game_object_clear_render_callbacks(game_object *obj);

struct render_callback *
game_object_render_callback_next(game_object *obj,
                                 struct render_callback *cur);

/* message list functions */
void
game_object_append_message(game_object *obj,
                           game_object *sender,
                           message_callback_func callback);

void
game_object_clear_messages(game_object *obj);

struct message *
game_object_message_next(game_object *obj,
                         struct message *cur);


#endif /* __GAMEOBJECT_H__ */
