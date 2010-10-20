CC = gcc
CCFLAGS = -Wall -g -fpic
#CCFLAGS = -Wall -g -fpic -DENABLE_LOGGING
#CCFLAGS = -Wall -g -O2 -fpic
INCLUDES = $(shell sdl-config --cflags) -I../../black/include -I../include
LDFLAGS = -shared -llua
LIBS =

SRCS =	kern.c \
	wrap.c

OBJS = $(SRCS:.c=.o)

MAIN = libblacklua.so


.SUFFIXES: .c .o

.c.o:
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: .depend clean

$(MAIN): $(OBJS)
	$(CC) $(CCFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LDFLAGS) $(LIBS)

api_wrap.c: api.i
	swig -lua -o $@ -I/usr/include/SDL api.i

clean:
	rm -f *.o *~ $(MAIN)

.depend: $(SRCS)
	$(CC) -M $(CCFLAGS) $(INCLUDES) $^ > $@

include .depend
