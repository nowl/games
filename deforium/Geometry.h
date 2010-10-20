#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <vector>

#include "Extents.h"

class Polygon {
public:
	Polygon(float *pairs, int numPairs) { _buildPairs(pairs, numPairs); }

	const std::vector< std::pair<float, float> >& getPairs() const
	{
		return _pairs;
	}

private:
	std::vector< std::pair<float, float> > _pairs;

	void _buildPairs(float *pairs, int numPairs) {
		int index = 0;
		for(int i=0; i<numPairs; i++) {
			_pairs.push_back(std::pair<float, float>(pairs[index],
													 pairs[index+1]));
			index+=2;						  
		}		
	}	
};
	
// Returns t if point (x, y) is inside the bounding box.
bool pointInBB(float x, float y, const Extents& bbox);

// Returns t if two line segments intersect.
// Line one is from point (x1,y1) to point (x2,y2), Line 2 is from
// point (x3,y3) to point (x4,y4)."
bool linesIntersect(float x1, float y1, float x2, 
					float y2, float x3, float y3,
					float x4, float y4);

// Returns t if a point (x,y) is within a polygon.
bool pointInPolygon(float x, float y, const Polygon& poly);

// Returns true if the two bounding boxes intersect.
bool coarseBBIntersect(const Extents& bb1, const Extents& bb2);

// Returns t if the intersection occurs between two arbitrary polygons
// based on line intersections.
bool polygonIntersect(const Polygon& poly1, const Polygon &poly2);

#endif	// __GEOMETRY_H__
