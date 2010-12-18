from priorityqueueset import PriorityQueueSet
#import logging

class Node (object):
    def __init__(self, elem, parent = None, totalCost = 0, costFromStart = 0):
        self.elem = elem
        self.costFromStart = costFromStart
        self.totalCost = totalCost
        self.parent = parent

    def __eq__(self, elem):
        return self.elem == elem.elem

    def __hash__(self):
        return hash(self.elem)

    def __repr__(self):
        return repr(self.elem)

class AStar (object):

    def __init__(self, adjacenciesFunc, costFunc, heuristicCostFunc, impassablePred):
        self.adjacenciesFunc = adjacenciesFunc
        self.heuristicCostFunc = heuristicCostFunc
        self.costFunc = costFunc
        self.impassablePred = impassablePred

    def best_path(self, posA, posB):
        results = []
        closedList = []
        openList = PriorityQueueSet()

        startNode = Node(posA)
        endNode = Node(posB)

        # if start node or end node is impassable then this can not
        # happen
        if self.impassablePred(posA) or self.impassablePred(posB):
            return []

        startNode.costFromStart = self.costFunc(posA)
        startNode.totalCost = startNode.costFromStart

        reachedEndNode = False
        pathEndNode = None

        # add start node to the open list        
        openList.add(startNode, (startNode.totalCost, startNode.costFromStart))

        while len(openList) > 0:
            #logging.debug('------------------------------')
            #logging.debug('open: %s' % str(openList))
            #logging.debug('closed: %s' % str(closedList))

            node = openList.pop()

            # check if rNode is the end point, if so then we are done
            if node == endNode:
                reachedEndNode = True
                pathEndNode = node
                break
            
            closedList.append(node)

            #logging.debug('node: %s' % str(node))
            
            adjacencies = self.adjacenciesFunc(node.elem)

            for r in adjacencies:

                # first check if impassable, if so then skip
                if self.impassablePred(r):
                    continue
                
                h = self.heuristicCostFunc(r, posB)
                g = node.costFromStart + self.costFunc(r)
                f = g + h
                rNode = Node(r,
                             parent = node,
                             totalCost = f,
                             costFromStart = g)
                                
                # check if rNode is already in the closed list and if
                # so then ignore it
                if rNode in closedList:
                    continue

                # check if rNode is already in the open list and if so
                # then if it's G cost is better or the same then skip
                # to next node
                if rNode in openList:
                    olNodeCost = openList.get(rNode)[1]
                    if olNodeCost <= rNode.costFromStart:
                        continue
            
                # add rNode to the open list
                openList.add(rNode, (rNode.totalCost, rNode.costFromStart))

        # backtrack and copy results into the results buffer
        if reachedEndNode:
            n = pathEndNode
            while n:
                results.append(n.elem)
                n = n.parent

        # return it in the forward order
        results.reverse()
        return results
    
