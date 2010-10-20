#include <SDL.h>

#include "black.h"

static unsigned int ticks_per_second;
static float time_per_tick;
static unsigned int max_frame_skip = 5;

void
set_ticks_per_second(unsigned int num)
{
    ticks_per_second = num;
    time_per_tick = 1000.0 / num;
}

void
mainloop(game_manager* manager)
{
    event_queue_init();

    game_manager_start(manager);

    unsigned long next_game_tick = black_sdl_get_tick();

	unsigned long fps_counter = 0;
	unsigned long game_tick = 0;
	unsigned long fps_start_time = black_sdl_get_tick();

    while (manager->is_running)
    {
        game_manager_handle_events(manager);

        int loops = 0;
        unsigned int tick = black_sdl_get_tick();
        while(tick > next_game_tick && loops < max_frame_skip)
        {
            game_manager_update(manager, game_tick++);

            next_game_tick += time_per_tick;
            loops++;

            tick = black_sdl_get_tick();
        }

        float interpolation = (tick + time_per_tick - next_game_tick)/time_per_tick;
        game_manager_render(manager, interpolation);

		++fps_counter;
		if(black_sdl_get_tick() - fps_start_time > 1000)
		{
            manager->fps = fps_counter;
			fps_counter = 0;
			fps_start_time = black_sdl_get_tick();
		}
    }

    event_queue_destroy();
}
