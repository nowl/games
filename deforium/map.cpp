#include <cmath>
#include <ostream>
#include <sstream>

#include "map.h"

using namespace std;
using namespace boost;

Map::Map(int width, int height, float *costs)
	: _width(width), _height(height)
{
  // allocate memory
  mElements = new MapElement*[width];

  for(int i=0; i<width; i++)
    mElements[i] = new MapElement[height];

  // fill in costs
  int i=0;
  for(int y=0; y<height; y++) 
      for(int x=0; x<width; x++, i++) {
          mElements[x][y].cost = costs[i];
          mElements[x][y].x = x;
          mElements[x][y].y = y;
      }

}

Map::~Map() {
  for(int i=0; i<_width; i++)
    delete [] mElements[i];

  delete [] mElements;
}

void
Map::getAdjacencies(const MapElement& m1, vector<MapElement>& results) const
{
/*
    for(int x = m1.x-1; x <= m1.x+1; x++)
        for(int y = m1.y-1; y <= m1.y+1; y++)
            if(x >= 0 && x < _width && y >= 0 && y < _height)
                if(x != m1.x || y != m1.y)
                    results.push_back(mElements[x][y]);
*/

    int x, y;
    x = m1.x-1; y = m1.y;
    if(x >= 0) results.push_back(mElements[x][y]);
    x = m1.x+1; y = m1.y;
    if(x < _width) results.push_back(mElements[x][y]);
    x = m1.x; y = m1.y-1;
    if(y >= 0) results.push_back(mElements[x][y]);
    x = m1.x; y = m1.y+1;
    if(y < _height) results.push_back(mElements[x][y]);
}

float
Map::getHeuristicCost(const MapElement& m1, const MapElement& m2) const
{
    return fabs(m2.x - m1.x) + fabs(m2.y - m1.y);
}

float
Map::getCost(const MapElement& me) const {
	// check for out of bounds
	if(me.x < 0 || me.x >= _width || me.y < 0 || me.y >= _height) {
		stringstream error;
		error << "element (" << me.x << ", "
			  << me.y << ") is out of bounds";
		throw MapOutOfBoundsException(error.str());
	}
	
	return mElements[me.x][me.y].cost;
}

int
Map::getWidth() const
{
	return _width;
}

int
Map::getHeight() const
{
	return _height;
}

signal<void (const Position&)>&
Map::getChangeSignal()
{
	return _changeSignal;
}
