def getCost(map, elem):
    x = elem.x
    y = elem.y
    return map.map[map.width * y + x].meta['cost']

def getHeuristicCost(map, elem1, elem2):
    x1 = elem1.x
    x2 = elem2.x
    y1 = elem1.y
    y2 = elem2.y
    return (abs(x2 - x1) + abs(y2 - y1)) * map.meta['averageCost']

def setCost(map, x, y, cost):
    map.map[map.width * y + x].meta['cost'] = cost

def isImpassable(elem):
    return elem.meta['type'] == 'impassable'
