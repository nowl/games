#ifndef __TYPES_H__
#define __TYPES_H__

typedef struct game_state game_state;
typedef struct game_object game_object;
typedef struct game_manager game_manager;
typedef struct sdl_graphics_context sdl_graphics_context;

typedef void (*message_callback_func)(game_object *sender, game_object *receiver);
typedef void (*game_object_update_fn)(game_manager *manager, game_object *obj, unsigned int ticks);
typedef void (*game_object_render_fn)(game_manager *manager, game_object *obj, float interpolation);

#endif	/* __TYPES_H__ */
