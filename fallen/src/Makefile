CC = gcc
CCFLAGS = -Wall -g
#CCFLAGS = -Wall -g -DENABLE_LOGGING
#CCFLAGS = -Wall -g -O2
INCLUDES = $(shell sdl-config --cflags) -I../Black/black/include -I../Black/blacklua/include
LDFLAGS = -Wl,-rpath,'$$ORIGIN'
LIBS = $(shell sdl-config --libs) -lSDL_image -L../win -lSDL_ttf -lSDL_mixer -L../ -lblack -lblacklua

SRCS = main.c

OBJS = $(SRCS:.c=.o)

MAIN = fallen


.SUFFIXES: .c .o

.c.o:
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: .depend clean

$(MAIN): $(OBJS)
	$(CC) $(CCFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIBS) $(LDFLAGS)

clean:
	rm -f *.o *~ $(MAIN)

.depend: $(SRCS)
	$(CC) -M $(CCFLAGS) $(INCLUDES) $^ > $@

include .depend
