#ifndef TYPES_H__
#define TYPES_H__

typedef int (*SDLEventCallback)(SDL_Event *event);
struct event_callback_list_t {
	SDLEventCallback func;
	struct event_callback_list_t *next;
};

typedef void (*UpdateCallback)(void);
struct update_callback_list_t {
	UpdateCallback func;
	struct update_callback_list_t *next;
};

typedef void (*RenderCallback)(float interpolation);
struct render_callback_list_t {
	RenderCallback func;
	struct render_callback_list_t *next;
};

struct player_t {
    int map_location_x, map_location_y;

#define EAST    0
#define WEST    1
#define NORTH   2
#define SOUTH   3
    unsigned char facing;

    struct {
        /* ordering of tiles is the following:
		 * 23 24 25 26 27 28 29 30 31
         * 14 15 16 17 18 19 20 21 22
         *    07 08 09 10 11 12 13
         *       02 03 04 05 06
         *          00 xx 01
         * where the example given is x looking north
         */
        int map_tiles[32];
    } view;
};

struct map_t {
    int width, height;
    int **data;
};

struct gamestate_t {
    unsigned char main_loop_running;

    struct player_t player;
	struct map_t map;

#define NUM_EVENT_MODES 1
#define EM_MAP          0
	struct event_callback_list_t *event_callback[NUM_EVENT_MODES];
	
	unsigned int event_mode;

	struct update_callback_list_t *update_callback;

#define NUM_RENDER_LAYERS       1
#define RL_MAP                  0
	struct render_callback_list_t *render_callback[NUM_RENDER_LAYERS];
};

#endif  /* TYPES_H__ */
