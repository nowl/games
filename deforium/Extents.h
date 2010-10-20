#ifndef __EXTENTS_H__
#define __EXTENTS_H__

#include "Position.h"

/** This is a map boxing class. This represents a position with a
 * width and height.
 */
class Extents {
public:
	/** Constructor. Sets zero for position, width, and height
	 */
    Extents() : ulx(0), uly(0),
                width(0), height(0)
		{}
    
	/** Constructor.
	 * @param ulx upper left x value
	 * @param uly upper left y value
	 * @param width width of box
	 * @param height height of box
	 */
    explicit Extents(float ulx, float uly, float width, float height)
        : ulx(ulx), uly(uly),
		width(width), height(height)
	{}
	
	/** Upper left position. */
	Position getUL() const { return Position(ulx, uly); }
	/** Minimum x value. */
	float getMinX() const { return ulx; }
	/** Maximum x value. */
	float getMaxX() const { return ulx+width; }
	/** Minimum y value. */
	float getMinY() const { return uly; }
	/** Maximum y value. */
	float getMaxY() const { return uly+height; }
	/** The width of the box. */
	float getWidth() const { return width; }
	/** The height of the box. */
	float getHeight() const { return height; }

	/** Sets minimum x value. */
	void setMinX(float x) { ulx = x; }
	/** Sets maximum x value. */
	void setMaxX(float x) { width = x - ulx; }
	/** Sets minimum y value. */
	void setMinY(float y) { uly = y; }
	/** Sets maximum y value. */
	void setMaxY(float y) { height = y - uly; }
	/** Sets width. */
	void setWidth(float w) { width = w; }
	/** Sets height. */
	void setHeight(float h) { height = h; }
	

private:        
    float ulx, uly;
    float width, height;
};

#endif  // __EXTENTS_H__
