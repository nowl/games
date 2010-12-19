from pygame.sprite import Sprite, Group, Rect

class MapElement (object):
    def __init__(self, x, y, mapElementCB = None):
        self.x = x
        self.y = y

        if mapElementCB:
            mapElementCB(self)

    def __eq__(self, elem):
        return self.x == elem.x and self.y == elem.y

    def __repr__(self):
        return '(%d, %d)' % (self.x, self.y)
        
class Map (object):
    def __init__(self, width, height, meta = None, mapElementCB = None):
        self.width = width
        self.height = height
        self.meta = {'averageCost': 10}

        self.map = []
        for i in range(height):
            for j in range(width):
                self.map.append(MapElement(j, i, mapElementCB = mapElementCB))

    def getMapElement(self, x, y):
        return self.map[self.width * y + x]

    def getDiagonalAdjacencies(self, elem):
        x = elem.x
        y = elem.y
        
        results = []

        minX = max(x-1, 0)
        maxX = min(self.width-1, x+1)
        minY = max(y-1, 0)
        maxY = min(self.height-1, y+1)

        for j in range(minX, maxX + 1):
            for i in range(minY, maxY + 1):
                if j == x and i == y:
                    continue
                results.append(self.getMapElement(j, i))

        return results

    def getSquareAdjacencies(self, elem):
        x = elem.x
        y = elem.y
        
        squaresToCheck = [(x-1, y), (x+1, y), (x, y-1), (x, y+1)]
        results = []

        minX = max(x-1, 0)
        maxX = min(self.width-1, x+1)
        minY = max(y-1, 0)
        maxY = min(self.height-1, y+1)

        for point in squaresToCheck:
            if point[0] >= minX and point[0] <= maxX and point[1] >= minY and point[1] <= maxY:
                results.append(self.getMapElement(point[0], point[1]))

        return results


    #def addChangeListener(self, listener):
    #    self._changeListeners.append(listener)
    #def _updateChangeListeners(self):
    #    for listener in self._changeListeners:
    #        listener()

class MapElementSprite (Sprite):
    def __init__(self, mapElement, imageCache):
        Sprite.__init__(self)
        self.mapElement = mapElement
        self.image = imageCache.getCachedSurface(self.mapElement.meta['image'])
        self.rect = self.image.get_rect()
        pixelLocation = self.mapElement.meta['drawLocation']
        self.rect.move_ip(*pixelLocation)
    
    def update(self, *args):
        pass

class MapView (Group):
    def __init__(self, map, imageCache, rectPixelView, imWidth, imHeight):
        Group.__init__(self)
        self.map = map
        self.imageCache = imageCache
        self.imageSize = (imWidth, imHeight)
        self.setView(rectPixelView)

    def setView(self, rect):
        self.view = rect
        self._updateContainer()

    def moveView(self, x, y):
        self.view.move_ip(x, y)
        self._updateContainer()

    def _updateContainer(self):
        self.empty()
        
        sprites = []
        count =0
        for elem in self.map.map:
            w = self.imageSize[0]
            h = self.imageSize[1]
            pixelX = elem.x * w
            pixelY = elem.y * h
            if pixelX+w >= self.view.left and pixelX <= self.view.right and pixelY+h >= self.view.top and pixelY <= self.view.bottom:
                offsetX = self.view.left
                offsetY = self.view.top
                elem.meta['drawLocation'] = pixelX-offsetX, pixelY-offsetY
                sprites.append(MapElementSprite(elem, self.imageCache))
                count += 1
        print count
        self.add(sprites)

                
