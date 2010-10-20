#ifndef __MAP_H__
#define __MAP_H__

#include <vector>
#include <stdexcept>
#include <boost/signal.hpp>

#include "Position.h"

/** Exception thrown when an element is requested from the map that is
 * out of bounds.
 */
class MapOutOfBoundsException : public std::runtime_error {
public:
    MapOutOfBoundsException(const std::string& msg) 
        : std::runtime_error(msg) {};
};

/** An object used internally by the map model representing a location
 * on the map along with a cost associated with the map location.
 */
struct MapElement {
    MapElement() {}
    MapElement(int x, int y) : x(x), y(y) {}
    MapElement(int x, int y, float cost) : x(x), y(y), cost(cost) {}
	
    bool operator==(const MapElement& m) const {
		return (x == m.x) && (y == m.y);
    }

    int x, y;
    float cost;
};

/** The main map model. This is the underlying map model for the array
 * of game terrains.
 */
class Map {
public:
	/** Constructor. Takes the width and the height of the map in
	 * integer units. Also a cost array is required in row order of
	 * the costs of the map squares.
	 */
	Map(int width, int height, float *costs);
	~Map();

	/** Returns the cost of a given location. */
	float getCost(const MapElement& me) const;

	/** Returns the map width in units. */
	int getWidth() const;

	/** Returns the map height in units. */
	int getHeight() const;

	/** Returns a vector of adjacent MapElements given a location on
	 * the map. */
	void getAdjacencies(const MapElement& m1, 
						std::vector<MapElement>& results) const;

	/** Returns the best guess cost for moving between the two passed
	 * in points on the map.
	 */
	float getHeuristicCost(const MapElement& m1,
						   const MapElement& m2) const;

    bool elementsEqual(const MapElement& m1,
                       const MapElement& m2) const {
        return m1 == m2;
    }    
	
	/** This allows slots to listen for changes to the underlying map
	 * model, i.e. changes to the terrain.
	 */
	boost::signal<void (const Position&)>& getChangeSignal();

private:
	int _width, _height;
	MapElement **mElements;
	boost::signal<void (const Position&)> _changeSignal;
};

#endif // __MAP_H__
