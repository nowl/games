#include "dm.h"

static int
quit_event(SDL_Event *event)
{
	switch(event->type) {
	case SDL_KEYDOWN:
		if(event->key.keysym.sym == SDLK_ESCAPE) {
			GAMESTATE.main_loop_running = 0;
			return 1;
		}
		break;
	}

	return 0;
}

void
gamestate_init()
{
	event_callback_add(quit_event, EM_MAP);
	GAMESTATE.event_mode = EM_MAP;
}
