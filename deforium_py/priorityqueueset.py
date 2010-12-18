class PriorityQueueSet ():
    """
    Basic implementation of a priority queue set using dicts. The
    insertion is O(1) but the popping of next lowest is O(n).
    """

    def __init__(self, *args):
        self.queue = {}
        
    def add(self, x, priority=0):
        self.queue[x] = priority

    def get(self, x):
        return self.queue[x]

    def pop(self):
        lowest, key = None, None
        for (k,v) in self.queue.iteritems():
            if lowest is None or v < lowest:
                lowest = v
                key = k
        if lowest is not None:
            del self.queue[key]
            return key

        return None

    def __contains__(self, x):
        return self.queue.__contains__(x)

    def __len__(self):
        return self.queue.__len__()

    def __repr__(self):
        return self.queue.__str__()
