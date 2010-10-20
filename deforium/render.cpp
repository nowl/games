#include <iostream>

#include <boost/foreach.hpp>

#include "Color.h"
#include "GraphicsEngine.h"
#include "GameState.h"
#include "Log.h"
#include "IRenderable.h"
#include "GameObject.h"

using namespace std;

/*
static void clearVisible(GameObject *go)
{
    go->setVisible(false);

    BOOST_FOREACH(GameObject *go, go->getChildren())
        clearVisible(go);
}
*/

void render(GraphicsEngine *ge,
            GameState &gs,
            double interpolation)
{
    //Log *log = Log::getLogger("render");

    ge->beginDraw();

/*
    for(int i=0; i<100000; i++) {
        ge->setPixel(rand()%(ge->getScreenWidth()), 
                     rand()%(ge->getScreenHeight()),
                     Color(rand()%256, rand()%256, rand()%256, 0, 256));
    }
*/

    // set all visibles to false
    /*
    BOOST_FOREACH(GameObject *gu, gs.renderList)
        clearVisible(gu);
    */
    
    // loop over all render levels
    for(int rendLayer = gs.minRenderLayer; rendLayer <= gs.maxRenderLayer; rendLayer++) {
        gs.renderLayer = rendLayer;

        // loop over all render objects in game state and render them
        BOOST_FOREACH(GameObject *gu, gs.renderList)
            gu->render(ge, &gs, interpolation);
    }

    ge->update();
}
