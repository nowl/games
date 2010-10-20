#include "black.h"

static unsigned long global_id_counter = 0;

static RB_HEAD(game_object_tree, game_object) game_objects = RB_INITIALIZER(game_objects);

static int game_object_cmp(struct game_object *a, struct game_object *b) {
    return a->id - b->id;
}

RB_GENERATE_STATIC(game_object_tree, game_object, _rb_tree_entry, game_object_cmp);

game_object *
make_game_object(unsigned int type, void *data)
{
    struct game_object *obj = malloc(sizeof(*obj));
    obj->id = global_id_counter++;
    obj->type = type;
    obj->data = data;
	obj->image = NULL;
	obj->screenx = 0;
	obj->screeny = 0;
    SLL_INIT(&obj->update_callbacks);
    SLL_INIT(&obj->render_callbacks);
    SLL_INIT(&obj->message_list);

    /* store in rb tree for easier retrieval */
    game_object_tree_RB_INSERT(&game_objects, obj);

    return obj;
}

void
destroy_game_object(game_manager *gm, game_object *go)
{
    // TODO: ensure object is removed from all states
    game_state_remove_object(gm->state, go);

    game_object_clear_update_callbacks(go);
    game_object_clear_render_callbacks(go);
    game_object_tree_RB_REMOVE(&game_objects, go);
    free(go);
}

game_object *
game_object_get(int id)
{
    struct game_object obj;
    obj.id = id;

    /* attempt to retrieve from rb tree */
    return game_object_tree_RB_FIND(&game_objects, &obj);
}

game_object *
game_object_remove(game_object *obj)
{
    /* attempt to remove from rb tree */
    return game_object_tree_RB_REMOVE(&game_objects, obj);
}

game_object *
game_object_remove_by_id(int id)
{
    struct game_object obj;
    obj.id = id;

    return game_object_remove(&obj);
}

SLL_GENERATE(update_callback_list, update_callback, _sll_entry);

void
game_object_append_update_callback_c_func(game_object *obj,
                                          const game_object_update_fn callback)
{
    struct update_callback *cb = malloc(sizeof(*cb));
    cb->type = C_FUNC;
    cb->cb.c_func = callback;
    SLL_ENTRY_INIT(cb, _sll_entry);
    update_callback_list_SLL_APPEND(&obj->update_callbacks, cb);
}

static int update_callback_cmp_c_func(void *name, struct update_callback *cb)
{
    return name == cb->cb.c_func;
}

struct update_callback *
game_object_remove_update_callback_c_func(game_object *obj,
                                          const game_object_update_fn callback)
{
    return update_callback_list_SLL_REMOVE(&obj->update_callbacks, callback, update_callback_cmp_c_func);
}

void
game_object_append_update_callback_script_func(game_object *obj,
                                          char *callback)
{
    struct update_callback *cb = malloc(sizeof(*cb));
    cb->type = SCRIPT_FUNC;
    cb->cb.script_func = strdup(callback);
    SLL_ENTRY_INIT(cb, _sll_entry);
    update_callback_list_SLL_APPEND(&obj->update_callbacks, cb);
}

static int update_callback_cmp_script_func(void *name, struct update_callback *cb)
{
    return strcmp(name, cb->cb.script_func) == 0;
}

struct update_callback *
game_object_remove_update_callback_script_func(game_object *obj,
                                               char *callback)
{
    return update_callback_list_SLL_REMOVE(&obj->update_callbacks, callback, update_callback_cmp_script_func);
}

static void
free_updates(struct update_callback *cb)
{
    switch(cb->type)
    {
    case C_FUNC:
        break;
    case SCRIPT_FUNC:
        free(cb->cb.script_func);
        break;
    default:
        assert(FALSE);
    }

    free(cb);
}

static void
free_renders(struct render_callback *cb)
{
    switch(cb->type)
    {
    case C_FUNC:
        break;
    case SCRIPT_FUNC:
        free(cb->cb.script_func);
        break;
    default:
        assert(FALSE);
    }

    free(cb);
}


void
game_object_clear_update_callbacks(game_object *obj)
{
    update_callback_list_SLL_CLEAR(&obj->update_callbacks, free_updates);
}

struct update_callback *
game_object_update_callback_next(game_object *obj,
                                 struct update_callback *cur)
{
    return update_callback_list_SLL_NEXT(&obj->update_callbacks, cur);
}

SLL_GENERATE(render_callback_list, render_callback, _sll_entry);

void
game_object_append_render_callback_c_func(game_object *obj,
                                   const game_object_render_fn callback)
{
    struct render_callback *cb = malloc(sizeof(*cb));
    cb->type = C_FUNC;
    cb->cb.c_func = callback;
    SLL_ENTRY_INIT(cb, _sll_entry);
    render_callback_list_SLL_APPEND(&obj->render_callbacks, cb);
}

static int render_callback_cmp_c_func(void *name, struct render_callback *cb)
{
    return name == cb->cb.c_func;
}

struct render_callback *
game_object_remove_render_callback_c_func(game_object *obj,
                                   const game_object_render_fn callback)
{
    return render_callback_list_SLL_REMOVE(&obj->render_callbacks, callback, render_callback_cmp_c_func);
}

void
game_object_append_render_callback_script_func(game_object *obj,
                                   char * callback)
{
    struct render_callback *cb = malloc(sizeof(*cb));
    cb->type = SCRIPT_FUNC;
    cb->cb.script_func = strdup(callback);
    SLL_ENTRY_INIT(cb, _sll_entry);
    render_callback_list_SLL_APPEND(&obj->render_callbacks, cb);
}

static int render_callback_cmp_script_func(void *name, struct render_callback *cb)
{
    return strcmp(name, cb->cb.script_func) == 0;
}

struct render_callback *
game_object_remove_render_callback_script_func(game_object *obj,
                                               char * callback)
{
    return render_callback_list_SLL_REMOVE(&obj->render_callbacks, callback, render_callback_cmp_script_func);
}

void
game_object_clear_render_callbacks(game_object *obj)
{
    render_callback_list_SLL_CLEAR(&obj->render_callbacks, free_renders);
}

struct render_callback *
game_object_render_callback_next(game_object *obj,
                                 struct render_callback *cur)
{
    return render_callback_list_SLL_NEXT(&obj->render_callbacks, cur);
}


SLL_GENERATE(message_list, message, _sll_entry);

void
game_object_append_message(game_object *obj,
                           game_object *sender,
                           message_callback_func callback)
{
    struct message *mes = message_create(sender, obj, callback);
    message_list_SLL_APPEND(&obj->message_list, mes);
}

void
game_object_clear_messages(game_object *obj)
{
    message_list_SLL_CLEAR(&obj->message_list, (void (*)(void *))message_destroy);
}

struct message *
game_object_message_next(game_object *obj,
                         struct message *cur)
{
    return message_list_SLL_NEXT(&obj->message_list, cur);
}
