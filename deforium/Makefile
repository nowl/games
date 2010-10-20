CXX = g++
CXXFLAGS = -Wall -g -O2
INCLUDES = $(shell sdl-config --cflags)
LIBS = $(shell sdl-config --libs) -lSDL_image -lSDL_ttf -lboost_signals -lboost_system -lboost_date_time -lboost_filesystem
LDFLAGS =

SRCS = CreatureUnit.cpp \
	ImageBackedGameObject.cpp \
	PlayerUnit.cpp \
	Event.cpp \
	Log.cpp \
	Random.cpp \
	EventProcessor.cpp \
	main.cpp \
	render.cpp \
	FontCache.cpp \
	MapBuilder.cpp \
	RIBGameObject.cpp \
	GameObject.cpp \
	map.cpp \
	Scenegraph.cpp \
	GameState.cpp \
	MapGameObject.cpp \
	SDLGraphicsEngine.cpp \
	Geometry.cpp \
	MapView.cpp \
	SDLUpdateEngine.cpp \
	GraphicsCache.cpp \
	MUGameObject.cpp \
	update.cpp

OBJS = $(SRCS:.cpp=.o)

MAIN = deforium

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: .depend clean

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIBS) $(LDFLAGS)

clean:
	rm -f *.o *~ $(MAIN)

.depend: $(SRCS)
	$(CXX) -M $(CXXFLAGS) $(INCLUDES) $^ > $@

include .depend
