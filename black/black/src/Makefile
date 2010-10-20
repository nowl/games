CC = gcc
#CCFLAGS = -Wall -g -fpic
#CCFLAGS = -Wall -g -fpic -DENABLE_LOGGING
CCFLAGS = -Wall -g -O2 -fpic
INCLUDES = $(shell sdl-config --cflags) -I../include
LDFLAGS = -shared
LIBS = $(shell sdl-config --libs) -lSDL_image

SRCS =	font.c \
	game_state.c \
	sdl_graphics_context.c \
	sdl_sound.c \
	game_manager.c \
	mainloop.c \
	game_object.c \
	event_queue.c \
	image_loader.c \
	image_render_set.c \
	utils.c \
	collide.c \
	sdl_font.c \
	message.c

OBJS = $(SRCS:.c=.o)

MAIN = libblack.so


.SUFFIXES: .c .o

.c.o:
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: .depend clean

$(MAIN): $(OBJS)
	$(CC) $(CCFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o *~ $(MAIN)

.depend: $(SRCS)
	$(CC) -M $(CCFLAGS) $(INCLUDES) $^ > $@

include .depend
