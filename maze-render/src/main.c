#include "dm.h"

#define TICKS_PER_SECOND    25
#define TIME_PER_TICK       (1000 / TICKS_PER_SECOND) /* MS per Tick */
#define MAX_FRAME_SKIP      5

static void
main_game_loop()
{
    unsigned long next_game_tick = sdl_get_tick();

    GAMESTATE.main_loop_running = 1;

    while(GAMESTATE.main_loop_running) {
        int loops = 0;
        double interpolation;
        
        while(sdl_get_tick() > next_game_tick && loops < MAX_FRAME_SKIP)
        {
            update();
            
            next_game_tick += TIME_PER_TICK;
            loops++;
        }
        
        interpolation = (sdl_get_tick() + TIME_PER_TICK - next_game_tick)/TIME_PER_TICK;
        render(interpolation);
    }
}

int
main(int argc, char **argv)
{
	gamestate_init();
    player_init();
	
    read_map("data/test.map");
	print_map();
    
    sdl_init();
    sdl_set_video_mode(1024, 768, 0);

	img_reader_init("data/images.txt");
	map_read_tileset("data/tileset.txt");

    main_game_loop();

	img_reader_destroy();
    sdl_close();
    destroy_map();

    return 0;
}
