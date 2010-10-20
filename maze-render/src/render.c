#include "dm.h"

void
render_callback_add(RenderCallback callback_func, unsigned int layer)
{
	LOG("adding render callback %p to layer %d\n", callback_func, layer);

	struct render_callback_list_t *cb = malloc(sizeof(*cb));
	cb->func = callback_func;
	cb->next = GAMESTATE.render_callback[layer];
	GAMESTATE.render_callback[layer] = cb;
}

void
render_callback_remove(RenderCallback callback_func, unsigned int layer)
{
	LOG("removing render callback %p from layer %d\n", callback_func, layer);

	/* scan through callbacks to find the one to remove */
	struct render_callback_list_t *cb, *prev_cb = NULL;
	for(cb = GAMESTATE.render_callback[layer]; cb; prev_cb = cb, cb = cb->next) {
		if(cb->func == callback_func) {

			/* fix up previous-link's next pointers */
			if(prev_cb)
				prev_cb->next = cb->next;
			else
				GAMESTATE.render_callback[layer] = cb->next;

			free(cb);
			break;
		}
	}			
}

void
render(double interpolation)
{	
	/* scan through each render layer */
	unsigned int layer;
	for(layer=0; layer<NUM_RENDER_LAYERS; layer++)
	{
		/* scan through render callbacks */
		struct render_callback_list_t* callback = GAMESTATE.render_callback[layer];
		for(; callback; callback = callback->next)
			callback->func(interpolation);
	}
	
	sdl_update_screen();
}
