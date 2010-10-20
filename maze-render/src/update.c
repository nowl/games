#include "dm.h"

void
update_callback_add(UpdateCallback callback_func)
{
	LOG("adding update callback %p\n", callback_func);

	struct update_callback_list_t *cb = malloc(sizeof(*cb));
	cb->func = callback_func;
	cb->next = GAMESTATE.update_callback;
	GAMESTATE.update_callback = cb;
}

void
update_callback_remove(UpdateCallback callback_func)
{
	LOG("removing update callback %p\n", callback_func);

	/* scan through callbacks to find the one to remove */
	struct update_callback_list_t *cb, *prev_cb = NULL;
	for(cb = GAMESTATE.update_callback; cb; prev_cb = cb, cb = cb->next) {
		if(cb->func == callback_func) {

			/* fix up previous-link's next pointers */
			if(prev_cb)
				prev_cb->next = cb->next;
			else
				GAMESTATE.update_callback = cb->next;

			free(cb);
			break;
		}
	}			
}

void
update()
{
	LOG("calling update\n");

    SDL_Event event;

    while(SDL_PollEvent(&event)) {
		/* scan through event handlers */
		struct event_callback_list_t* callback = GAMESTATE.event_callback[GAMESTATE.event_mode];
		for(; callback; callback = callback->next)
			if( callback->func(&event) )
				break;
    }

	/* scan through update handlers */
	struct update_callback_list_t* callback = GAMESTATE.update_callback;
	for(; callback; callback = callback->next)
		callback->func();
}
