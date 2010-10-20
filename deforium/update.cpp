#include <iostream>

#include <boost/foreach.hpp>

#include "UpdateEngine.h"
#include "GameState.h"
#include "Log.h"
#include "GameObject.h"

using namespace std;

void update(UpdateEngine *ue,
            GameState &gs)
{
    //Log *log = Log::getLogger("update");

	// This will pull and process any pending input from the user.
    ue->update();

	// This will do the updates on each game object in the render
	// list.
	BOOST_FOREACH(GameObject *go, gs.renderList)
		go->update(&gs);
}
