#include "MapBuilder.h"
#include "Random.h"
#include "GraphicsCache.h"
#include "CreatureUnit.h"

using namespace boost;
using namespace std;

MapBuilder::MapBuilder(int width, int height, float percentBlocked)
	: _width(width), _height(height)
{	
	_costMap = new float [width * height];
	
	// fill with normal values
	for(int h=0; h<height; h++)
		for(int w=0; w<width; w++)
			_costMap[h*width+w] = 0;
	
    // sprinkle a certain amount of impassable terrains
    for(int h=0; h<height; h++)
		for(int w=0; w<width; w++)
            if(Random::floatVal() < (percentBlocked/100.0))
                _costMap[h*width+w] = 1000;
	
    // add start and end points
    _costMap[0] = 98;                // begin
    _costMap[width*height - 1] = 99; // end
	
	_map = new Map(width, height, _costMap);
}

MapBuilder::~MapBuilder() {
	delete _costMap;
	delete _map;
}

void
MapBuilder::populate(float percent, vector<GameObject*>& gameUnits)
{
	// sprinkle a certain amount of creatures
    for(int h=0; h<_height; h++)
		for(int w=0; w<_width; w++)
            if((Random::floatVal() < (percent/100.0)) && (_costMap[h*_width+w] != 1)) {
				// TODO: get rid of these references to "32"
				gameUnits.push_back(new CreatureUnit("creature",
                                                     2,
                                                     Position(w*32, h*32),
                                                     "units/KillerFrog.png",
                                                     _map));
			}
}


Map*
MapBuilder::map()
{
	return _map;
}
