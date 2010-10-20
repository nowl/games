#include <iostream>
#include <sstream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/any.hpp>

#include <SDL/SDL.h>

#include "GraphicsEngineManager.h"
#include "GraphicsEngine.h"
#include "UpdateEngineManager.h"
#include "UpdateEngine.h"
#include "SDLManager.h"
#include "GameState.h"
#include "Log.h"
#include "Scenegraph.h"
#include "MapBuilder.h"
#include "Event.h"
#include "EventProcessor.h"
#include "Random.h"
#include "MapGameObject.h"
#include "MapView.h"
#include "PlayerUnit.h"
#include "GraphicsCache.h"
#include "ObjectTransform.h"
#include "TextObject.h"

static const int TICKS_PER_SECOND = 50;
static const int TIME_PER_TICK = 1000 / TICKS_PER_SECOND; /* MS per TICK */
static const int MAX_FRAME_SKIP = 5;

using namespace std;
using namespace boost;

void render(GraphicsEngine *ge, GameState &gs, double interpolation);
void update(UpdateEngine *ue, GameState &gs);

void game_loop() {
    try {
        Log* logger = Log::getLogger("main");
        Log::setLevel(Log::INFO);

        Random::init();

        logger->info("Initializing Gamestate");
        GameState gameState;

        gameState.minRenderLayer = 0;
        gameState.maxRenderLayer = 5;

        logger->info("Initializing SDL");
        SDLManager sdlman;

        logger->info("Initializing graphics manager");
        GraphicsEngineManager gem(GraphicsEngineManager::SDL, &sdlman);
        //GraphicsEngineManager gem(GraphicsEngineManager::NOOP, &sdlman);
        GraphicsEngine *ge = gem.getEngine();

        logger->info("Initializing update manager");
        UpdateEngineManager uem(UpdateEngineManager::SDL, &sdlman, *ge, gameState);
        UpdateEngine *ue = uem.getEngine();
        
		logger->info("Setting up video");
        ge->setVideoMode(1024, 768, 32, GraphicsEngine::DEFAULTS, false);
        
        Scenegraph scene;

		MapBuilder generatedMap(50, 50, 25.0);
		Map *mainMap = generatedMap.map();
        //gameState["currentMap"] = mainMap;
		//MapRenderer mainMapRenderer(gameState.normalUnitSize);        
		MapView view(Extents(0,0,1024,768), 
					 &gameState,
					 mainMap);

        gameState["MapBuilder"] = &generatedMap;

        vector<GameObject*> gameUnits;
		generatedMap.populate(5.0, gameUnits); // add creatures

		MapGameObject mapObj("map game object",
							 1,
							 Position(0, 0),
							 1024,
							 768,
							 mainMap,
							 &view,
							 32);

        // add all of these objects as children of the map object
        BOOST_FOREACH(GameObject* goptr, gameUnits)
            mapObj.addObject(goptr);
        
        gameState["MapGameObject"] = &mapObj;

		scene.addChild(&mapObj);
		gameState.renderList.push_back(&scene);

        PlayerUnit *player = new PlayerUnit("player", 3, 
                                            Position(0, 0), 
											4,
                                            150);
        mapObj.addObject(player);

        gameState["PlayerObject"] = player;

        TextObject livesText("lives text display", 4, Position(150, 10), "VeraMono.ttf", 24, 0, 0, 250);
        livesText.setText("Lives: 4");
		gameState["LivesText"] = &livesText;

		TextObject deforiumText("deforium text display", 4, Position(300, 10), "VeraMono.ttf", 24, 0, 0, 250);
        deforiumText.setText("Deforium: 0");
		gameState["DeforiumText"] = &deforiumText;
		gameState["Deforium"] = 0.0f;

        TextObject scoreText("score text display", 4, Position(500, 10), "VeraMono.ttf", 24, 0, 0, 250);
        scoreText.setText("Score: 0");
		gameState["ScoreText"] = &scoreText;
        gameState["Score"] = 0.0f;

        TextObject fpsText("fps text display", 4, Position(700, 10), "VeraMono.ttf", 24, 0, 0, 250);
        fpsText.setText("FPS: ");
		fpsText.setVisible(false);
		gameState["FPSText"] = &fpsText;

        scene.addChild(&livesText);
        scene.addChild(&deforiumText);
        scene.addChild(&scoreText);
        scene.addChild(&fpsText);

        int startTick = SDL_GetTicks();
        int frames=0;
        gameState.tickCount = 0;

        gameState["Level"] = 1;

        unsigned long next_game_tick = SDL_GetTicks();
        logger->info("Entering game loop");
        while(gameState.mainLoopRunning) {
            int loops = 0;

            while(SDL_GetTicks() > next_game_tick && loops < MAX_FRAME_SKIP)
			{
                /*
                stringstream log;
                log << "tick: " << next_game_tick;
                logger->info(log.str());
                */

                update(ue, gameState);
                
                ++gameState.tickCount;
                
                next_game_tick += TIME_PER_TICK;
                loops++;
            }
            
            double interpolation = (double)(SDL_GetTicks() + TIME_PER_TICK - next_game_tick)/TIME_PER_TICK;
            render(ge, gameState, interpolation);

            frames++;
            if(frames%10 == 0) {
                float fps = frames / ((SDL_GetTicks() - startTick) / 1000.0);
                //cout << "fps = " << fps << endl;
                stringstream fpsMessage;
                fpsMessage << "FPS: " << fps;
                fpsText.setText(fpsMessage.str());
            }
        }
    } catch(const DefException& e) {
        cout << "DefException: " << e.what() << endl;
    }
}

int main(int argc, char *argv[])
{
    game_loop();

    return 0;
}
