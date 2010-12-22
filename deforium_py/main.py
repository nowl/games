import astar
from map import Map, MapElement, MapView
from astar import AStar
from imagecache import ImageCache
from maputils import getCost, getHeuristicCost, setCost, isImpassable
from random import random, randint

import sys, pygame

def initMapElement(elem):
    if random() > 0.1:
        i = 'normal'
        t = 'normal'
    else:
        i = 'impassable-1'
        t = 'impassable'
        
    elem.meta = {'image': i,
                 'type': t,
                 'cost': 10}

class MapSprite (pygame.sprite.Sprite):
    def __init__(self, screenRect):
        pygame.sprite.Sprite.__init__(self)
        self.location = [0, 0]
        self.mapView = None
        self.screenRect = screenRect

    def _modifyLocation(self, x, y):
        self.location[0] += x
        self.location[1] += y
        self.rect.left = self.location[0]
        self.rect.top = self.location[1]
        self.rect.clamp_ip(self.mapView.worldRect)
        self.location[0] = self.rect.left
        self.location[1] = self.rect.top
        self.rect.move_ip(-self.mapView.offsetX, -self.mapView.offsetY)

    def setMapView(self, mapView):
        self.mapView = mapView
        self.mapView.addChangeListener(self._mapViewChangeListener)

    def _mapViewChangeListener(self, x, y):
        raise NotImplementedError

class PlayerSprite (MapSprite):
    def __init__(self, img, screenRect):
        MapSprite.__init__(self, screenRect)
        self.image = img
        self.rect = self.image.get_rect()
        self.dx = 0
        self.dy = 0
        self.collideFunc = pygame.sprite.collide_rect_ratio(0.8)

    def update(self, *args):
        needsUpdate = False
        
        if self.dx != 0:
            needsUpdate = True
            self._modifyLocation(self.dx, 0)
            
            for sprite in self.mapView:
                if self.collideFunc(self, sprite) and isImpassable(sprite.mapElement):
                    self._modifyLocation(-self.dx, 0)

        if self.dy != 0:
            needsUpdate = True
            self._modifyLocation(0, self.dy)
            
            for sprite in self.mapView:
                if self.collideFunc(self, sprite) and isImpassable(sprite.mapElement):
                    self._modifyLocation(0, -self.dy)
            
        if needsUpdate:
            imWidth, imHeight = self.mapView.imageSize
            if self.mapView.view.right < self.mapView.map.width * imWidth and self.rect.right > self.screenRect.width-100:
                self.mapView.moveView(imWidth * 6, 0)
            if self.mapView.view.bottom < self.mapView.map.height * imHeight and self.rect.bottom > self.screenRect.height-100:
                self.mapView.moveView(0, imHeight * 4)
            if self.mapView.offsetX > 0 and self.rect.left < 100:
                self.mapView.moveView(-imWidth * 6, 0)
            if self.mapView.offsetY > 0 and self.rect.top < 100:
                self.mapView.moveView(0, -imHeight * 4)

    def _mapViewChangeListener(self, x, y):
        self.rect.move_ip(x, y)     

class MobSprite (MapSprite):
    def __init__(self, x, y, img, screenRect, astar, speed, player):
        MapSprite.__init__(self, screenRect)
        self.image = img
        self.rect = self.image.get_rect()
        self.dx = 0
        self.dy = 0
        self.path = []
        self.curPathElement = None
        self.collideFunc = pygame.sprite.collide_rect_ratio(0.8)
        self.location = [x, y]
        self.rect.move_ip(x, y)
        self.astar = astar
        self.speed = speed
        self.player = player

    def update(self, *args):
        if self.curPathElement:
            x, y = self.location
            dx, dy = self.curPathElement
            if abs(x-dx) < self.speed and abs(y-dy) < self.speed:
                self.curPathElement = None
                self.dx, self.dy = 0, 0
                
            else:
                if x < dx:
                    self.dx = self.speed
                elif x > dx:
                    self.dx = -self.speed
                if y < dy:
                    self.dy = self.speed
                elif y > dy:
                    self.dy = -self.speed
            
        elif self.path:
            imWidth, imHeight = self.mapView.imageSize
            self.curPathElement = self.path.pop(0)
            self.curPathElement = [self.curPathElement.x * imWidth, self.curPathElement.y * imHeight]
        else:
            imWidth, imHeight = self.mapView.imageSize
            levelMap = self.mapView.map
            x = self.location[0] / imWidth
            y = self.location[1] / imHeight
            playerX = self.player.location[0] / imWidth
            playerY = self.player.location[1] / imHeight
            self.path = self.astar.best_path(levelMap.getMapElement(x, y),
                                             levelMap.getMapElement(playerX, playerY))

        if self.dx != 0 or self.dy != 0:
            self._modifyLocation(self.dx, self.dy)
            
    
    def _mapViewChangeListener(self, x, y):
        self.rect.left = self.location[0]
        self.rect.top = self.location[1]
        self.rect.move_ip(-x, -y)        

def sprinkleMobs(num, maxSpeed, sprites, mapView, imageCache, screenRect, astar, player):
    imWidth, imHeight = mapView.imageSize
    for x in xrange(num):
        mob = MobSprite(randint(0, mapView.map.width * imWidth),
                        randint(0, mapView.map.height * imHeight),
                        imageCache.getCachedSurface("mob"),
                        screenRect,
                        astar,
                        random() * maxSpeed/2 + maxSpeed/2,
                        player)

        mob.setMapView(mapView)
    
        sprites.add(mob)    


class GameLoop (object):
    def __init__(self, ticks_per_second, max_frame_skip):
        self.TicksPerSecond = ticks_per_second
        self.TimePerTick = 1000.0 / self.TicksPerSecond # MS per tick
        self.MaxFrameSkip = max_frame_skip
        
        pygame.init()

        size = width, height = 800, 600
        self.black = 0,0,0
        self.screen = pygame.display.set_mode(size)

        self.imageCache = ImageCache()
        self.imageCache.getSurface("units/Player1.png", "ball")
        self.imageCache.getSurface("units/Creature1.png", "mob")
        self.imageCache.getSurface("terrains/Normal.jpg", "normal")
        self.imageCache.getSurface("terrains/Impassable1.jpg", "impassable-1")

        ball = self.imageCache.getCachedSurface("ball")
        self.screenRect = pygame.Rect(0, 0, width, height)
        self.playerSprite = PlayerSprite(ball, self.screenRect)
        self.sprites = pygame.sprite.Group(self.playerSprite)
        self.mapView = None

    def render(self):
        self.screen.fill(self.black)
        self.mapView.draw(self.screen)
        self.sprites.draw(self.screen)
        pygame.display.flip()

    def update(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT: sys.exit()
            
            speed = 5

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    sys.exit()
                elif event.key == pygame.K_a:
                    self.playerSprite.dx -= speed
                elif event.key == pygame.K_d:
                    self.playerSprite.dx += speed
                elif event.key == pygame.K_w:
                    self.playerSprite.dy -= speed
                elif event.key == pygame.K_s:
                    self.playerSprite.dy += speed
            if event.type == pygame.KEYUP:
                if event.key == pygame.K_a:
                    self.playerSprite.dx += speed
                elif event.key == pygame.K_d:
                    self.playerSprite.dx -= speed
                elif event.key == pygame.K_w:
                    self.playerSprite.dy += speed
                elif event.key == pygame.K_s:
                    self.playerSprite.dy -= speed

        self.mapView.update()
        self.sprites.update()

    def loop(self):
        nextGameTick = pygame.time.get_ticks()

        renderClock = pygame.time.Clock()
        updateTick = 0

        levelMap = Map(30, 30, mapElementCB = initMapElement)

        astar = AStar(levelMap.getSquareAdjacencies,
                      lambda elem: getCost(levelMap, elem),
                      lambda x, y: getHeuristicCost(levelMap, x, y),
                      isImpassable)

        self.mapView = MapView(levelMap, self.imageCache, pygame.Rect(0, 0, 800, 600), 32, 32)
        self.playerSprite.setMapView(self.mapView)

        sprinkleMobs(10, 8, self.sprites, self.mapView, self.imageCache, self.screenRect, astar, self.playerSprite)

        while 1:
            loops = 0

            while pygame.time.get_ticks() > nextGameTick and loops < self.MaxFrameSkip:
                self.update()
                nextGameTick += self.TimePerTick
                loops += 1
                updateTick += 1
                
                if updateTick % 100 == 0:
                    print renderClock.get_fps()

            renderClock.tick()    
            self.render()

if __name__ == '__main__':
    game = GameLoop(30, 5)
    game.loop()
