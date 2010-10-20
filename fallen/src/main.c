#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "blacklua.h"

int main ( int argc, char** argv )
{
    int fsflag = 0;
    char *cvalue = NULL;
    int index;
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "f")) != -1)
        switch (c)
        {
        case 'f':
            fsflag = 1;
            break;
        case '?':
            if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Unknown option character `\\x%x'.\n",
                         optopt);
            return 1;
            break;
        default:
            abort ();
        }

    struct game_manager *manager = new_game_manager();

    black_sdl_set_video_mode(manager->sdl_driver,
                             800, 600,
                             fsflag);
    blacklua_init(manager);
    blacklua_load("init.lua");

    SDL_ShowCursor(SDL_DISABLE);
	SDL_EnableKeyRepeat(50, 50);

    mainloop(manager);

    blacklua_shutdown();

    del_game_manager(manager);

    image_render_set_cleanup();

    image_loader_cleanup();

    return 0;
}
