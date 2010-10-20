#ifndef __BLACK_H__
#define __BLACK_H__

#include <assert.h>

#include "types.h"

#include "tree.h"
#include "list.h"

#include "sdl_graphics_context.h"
#include "sdl_font.h"
#include "sdl_sound.h"
#include "event_queue.h"
#include "game_object.h"
#include "game_state.h"
#include "game_manager.h"
#include "font.h"
#include "image_loader.h"
#include "image_render_set.h"
#include "utils.h"
#include "collide.h"
#include "message.h"

#ifndef __unused
# define __unused
#endif


#ifndef FALSE
# define FALSE 0
#endif
#ifndef TRUE
# define TRUE (!FALSE)
#endif

#ifndef SUCCESS
# define SUCCESS 0
#endif
#ifndef FAIL
# define FAIL (!SUCCESS)
#endif

void set_ticks_per_second(unsigned int num);
void mainloop(struct game_manager* manager);

#endif /* __BLACK_H__ */
