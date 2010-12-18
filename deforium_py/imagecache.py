import pygame

class ImageCache (object):
    def __init__(self):
        self.cache = {}

    def getSurface(self, filename, name = None):
        if name == None:
            name = filename

        if name in self.cache:
            return self.cache[name]
        else:
            image = pygame.image.load(filename)
            self.cache[name] = image
            return image

    def getCachedSurface(self, name):
        return self.cache[name]
