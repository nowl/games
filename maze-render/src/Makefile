CC = gcc
CFLAGS = -Wall -g -DENABLE_LOGGING
INCLUDES = $(shell sdl-config --cflags)
LFLAGS =
LIBS = $(shell sdl-config --libs) -lSDL_image

SRCS =	main.c map.c globals.c sdl.c update.c render.c player.c \
		event_callback.c gamestate.c graphics_cache.c img_reader.c

OBJS = $(SRCS:.c=.o)

MAIN = dm


.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: depend clean

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)


clean:
	rm -f *.o *~ $(MAIN)

depend: $(SRCS)
	$(CC) -M $(CFLAGS) $(INCLUDES) $^ > $@

include depend
