#include "dm.h"

void
event_callback_add(SDLEventCallback callback_func, unsigned int mode)
{
	LOG("adding event callback %p to mode %d\n", callback_func, mode);

	struct event_callback_list_t *cb = malloc(sizeof(*cb));
	cb->func = callback_func;
	cb->next = GAMESTATE.event_callback[mode];
	GAMESTATE.event_callback[mode] = cb;
}

void
event_callback_remove(SDLEventCallback callback_func, unsigned int mode)
{
	LOG("removing event callback %p from mode %d\n", callback_func, mode);

	/* scan through callbacks to find the one to remove */
	struct event_callback_list_t *cb, *prev_cb = NULL;
	for(cb = GAMESTATE.event_callback[mode]; cb; prev_cb = cb, cb = cb->next) {
		if(cb->func == callback_func) {

			/* fix up previous-link's next pointers */
			if(prev_cb)
				prev_cb->next = cb->next;
			else
				GAMESTATE.event_callback[mode] = cb->next;

			free(cb);
			break;
		}
	}			
}
