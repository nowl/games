#include <cassert>

#include <boost/foreach.hpp>

#include "Geometry.h"

using namespace std;

bool pointInBB(float x, float y, const Extents& bbox)
{
	return (x >= bbox.getMinX()) && (x < bbox.getMaxX())
		&& (y >= bbox.getMinY()) && (y < bbox.getMaxY());
}


bool linesIntersect(float x1, float y1, float x2, 
					float y2, float x3, float y3,
					float x4, float y4)
{
	float den = (y2-y1)*(x4-x3) + (y3-y4)*(x2-x1);
	if(den == 0)
		return false;

	float a = ((y3-y1)*(x2-x1) + (y1-y2)*(x3-x1)) / den;
	if((a <= 0) || (a >= 1))
		return false;
    
	den = (y4-y3)*(x2-x1) + (y2-y1)*(x3-x4);
	if(den == 0)
		return false;

	a = ((x4-x3)*(y1-y3) + (y4-y3)*(x3-x1)) / den;
	if((a <= 0) || (a >= 1))
		return false;

	return true;
}

bool pointInPolygon(float x, float y, const Polygon& poly)
{
	float xEnd = 1e6;
	float yEnd = 1e6 + 1.1;
	int intersections = 0;
	
	// get copy of polygon
	vector< pair<float,float> > polyCopy(poly.getPairs());

	if(polyCopy.front() != polyCopy.back())
		polyCopy.push_back(polyCopy.front());

	pair<float,float> prevPair(polyCopy.front());
	vector< pair<float,float> >::const_iterator iter;
	iter = polyCopy.begin();
	iter++;

	for(; iter!=polyCopy.end(); iter++) {
		const pair<float,float>& currentPair = *iter;
		if(linesIntersect(x, y, xEnd, yEnd, currentPair.first, 
						  currentPair.second, prevPair.first,
						  prevPair.second))
			++intersections;
		prevPair = currentPair;
	}
	
	if((intersections % 2) == 1)
		return true;

	return false;
}

template <class T>
T abs(const T& a) {
	if(a < 0)
		return -a;
	return a;
}

bool coarseBBIntersect(const Extents& bb1, const Extents& bb2)
{
	assert(bb1.getMaxX() > bb1.getMinX());
	assert(bb2.getMaxX() > bb2.getMinX());
	assert(bb1.getMaxY() > bb1.getMinY());
	assert(bb2.getMaxY() > bb2.getMinY());

	float midpoint_1x = (bb1.getMaxX() + bb1.getMinX()) / 2;
	float midpoint_2x = (bb2.getMaxX() + bb2.getMinX()) / 2;
	float span_1x = bb1.getMaxX() - midpoint_1x;
	float span_2x = bb2.getMaxX() - midpoint_2x;

	if(abs(midpoint_2x - midpoint_1x) > (span_1x + span_2x))
		return false;

	midpoint_1x = (bb1.getMaxY() + bb1.getMinY()) / 2;
	midpoint_2x = (bb2.getMaxY() + bb2.getMinY()) / 2;
	span_1x = bb1.getMaxY() - midpoint_1x;
	span_2x = bb2.getMaxY() - midpoint_2x;

	if(abs(midpoint_2x - midpoint_1x) > (span_1x + span_2x))
		return false;

	return true;
}

struct LineHolder {
	LineHolder(float a, float b, float c, float d)
		: x1(a), y1(b), x2(a), y2(b) {}
	float x1, y1, x2, y2;
};

bool polygonIntersect(const Polygon& poly1, const Polygon &poly2)
{
	// first test if any of the points are within the other polygon
	pair<float,float> point;
	BOOST_FOREACH(point, poly1.getPairs())
		if(pointInPolygon(point.first, point.second, poly2))
			return true;
	BOOST_FOREACH(point, poly2.getPairs())
		if(pointInPolygon(point.first, point.second, poly1))
			return true;
	
	// now do a brute force line check

	vector< pair<float,float> > poly1Copy(poly1.getPairs());
	vector< pair<float,float> > poly2Copy(poly2.getPairs());

	if(poly1Copy.front() != poly1Copy.back())
		poly1Copy.push_back(poly1Copy.front());
	if(poly2Copy.front() != poly2Copy.back())
		poly2Copy.push_back(poly2Copy.front());

	vector< LineHolder > lines1, lines2;

	// gather up points for line 1
	pair<float,float> prevPair(poly1Copy.front());
	vector< pair<float,float> >::const_iterator iter;
	iter = poly1Copy.begin();
	iter++;

	for(; iter!=poly1Copy.end(); iter++) {
		const pair<float,float>& currentPair = *iter;
		lines1.push_back(LineHolder(currentPair.first, currentPair.second,
									prevPair.first, prevPair.second));
		prevPair = currentPair;
	}

	// gather up points for line 2
	prevPair = (poly2Copy.front());
	iter = poly2Copy.begin();
	iter++;

	for(; iter!=poly2Copy.end(); iter++) {
		const pair<float,float>& currentPair = *iter;
		lines2.push_back(LineHolder(currentPair.first, currentPair.second,
									prevPair.first, prevPair.second));
		prevPair = currentPair;
	}

	BOOST_FOREACH(LineHolder l1, lines1) {
		BOOST_FOREACH(LineHolder l2, lines2) {
			if(linesIntersect(l1.x1,l1.y1,l1.x2,l1.y2,
							  l2.x1,l2.y1,l2.x2,l2.y2))
				return true;
		}
	}

	return false;
}
