#ifndef __MAP_BUILDER_H__
#define __MAP_BUILDER_H__

#include <vector>

#include "GameObject.h"
#include "map.h"

/** Generates a Map object. */
class MapBuilder {
public:
	/** This will generate a map object that is the specified width
	 * and height and with the specified percentage of impassable
	 * squares.
	 */
	MapBuilder(int width, int height, float percentBlocked=25.0);
	~MapBuilder();

    /** Randomly fills the map with creatures. */
	void populate(float percent, std::vector<GameObject*>& gameUnits);

	/** Returns the generated map. */
	Map* map();

private:
	float *_costMap;
	Map *_map;
	int _width, _height;
};

#endif	// __MAP_BUILDER_H__
