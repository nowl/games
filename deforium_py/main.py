import astar
from map import Map, MapElement, MapView
from astar import AStar
from imagecache import ImageCache
from maputils import getCost, getHeuristicCost, setCost, isImpassable
from random import random

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

class PlayerSprite (pygame.sprite.Sprite):
    def __init__(self, ballimg, screenRect):
        pygame.sprite.Sprite.__init__(self)
        self.image = ballimg
        self.rect = self.image.get_rect()
        self.dx = 0
        self.dy = 0
        self.location = [0, 0]
        self.mapView = None
        self.screenRect = screenRect
        self.collideFunc = pygame.sprite.collide_rect_ratio(0.8)

    def _modifyLocation(self, x, y):
        self.location[0] += x
        self.location[1] += y
        self.rect.left = self.location[0]
        self.rect.top = self.location[1]
        self.rect.clamp_ip(self.mapView.worldRect)
        self.location[0] = self.rect.left
        self.location[1] = self.rect.top
        self.rect.move_ip(-self.mapView.offsetX, -self.mapView.offsetY)
            
    def update(self, *args):
        #needsUpdate = False
        
        if self.dx != 0 or self.dy != 0:
            self._modifyLocation(self.dx, self.dy)
            
            for sprite in self.mapView:
                if self.collideFunc(self, sprite) and isImpassable(sprite.mapElement):
                    self._modifyLocation(-self.dx, -self.dy)
        
            imWidth, imHeight = self.mapView.imageSize
            if self.mapView.view.right < self.mapView.map.width * imWidth and self.rect.right > self.screenRect.width-100:
                self.mapView.moveView(imWidth * 6, 0)
                self.rect.move_ip(imWidth * 6, 0)
            if self.mapView.view.bottom < self.mapView.map.height * imHeight and self.rect.bottom > self.screenRect.height-100:
                self.mapView.moveView(0, imHeight * 4)
                self.rect.move_ip(0, imHeight * 4)
            if self.mapView.offsetX > 0 and self.rect.left < 100:
                self.mapView.moveView(-imWidth * 6, 0)
                self.rect.move_ip(-imWidth * 6, 0)
            if self.mapView.offsetY > 0 and self.rect.top < 100:
                self.mapView.moveView(0, -imHeight * 4)
                self.rect.move_ip(0, -imHeight * 4)


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
        self.imageCache.getSurface("terrains/Normal.jpg", "normal")
        self.imageCache.getSurface("terrains/Impassable1.jpg", "impassable-1")
        ball = self.imageCache.getCachedSurface("ball")

        self.playerSprite = PlayerSprite(ball, pygame.Rect(0, 0, width, height))
        self.sprites = pygame.sprite.Group(self.playerSprite)
        self.mapView = None

    def render(self):
        self.screen.fill(self.black)
        self.mapView.draw(self.screen)
        self.sprites.draw(self.screen)
        #screen.blit(ball, ballrect)
        pygame.display.flip()

    def update(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT: sys.exit()
            
            speed = 5

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    sys.exit()
                elif event.key == pygame.K_a:
                    self.playerSprite.dx = -speed
                elif event.key == pygame.K_d:
                    self.playerSprite.dx = speed
                elif event.key == pygame.K_w:
                    self.playerSprite.dy = -speed
                elif event.key == pygame.K_s:
                    self.playerSprite.dy = speed
            if event.type == pygame.KEYUP:
                if event.key == pygame.K_a:
                    self.playerSprite.dx = 0
                elif event.key == pygame.K_d:
                    self.playerSprite.dx = 0
                elif event.key == pygame.K_w:
                    self.playerSprite.dy = 0
                elif event.key == pygame.K_s:
                    self.playerSprite.dy = 0

        self.mapView.update()
        self.sprites.update()

    def loop(self):
        nextGameTick = pygame.time.get_ticks()

        renderClock = pygame.time.Clock()
        updateTick = 0

        levelMap = Map(50, 50, mapElementCB = initMapElement)

        astar = AStar(levelMap.getSquareAdjacencies,
                      lambda elem: getCost(levelMap, elem),
                      lambda x, y: getHeuristicCost(levelMap, x, y),
                      isImpassable)
        print astar.best_path(levelMap.getMapElement(0, 0),
                              levelMap.getMapElement(20, 20))

        self.mapView = MapView(levelMap, self.imageCache, pygame.Rect(0, 0, 800, 600), 32, 32)
        self.playerSprite.mapView = self.mapView

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
