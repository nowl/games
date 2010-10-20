#ifndef __ASTAR_H__
#define __ASTAR_H__

#include <iostream>
#include <ostream>
#include <vector>

#include "PriQueueMutableHeap.h"

namespace astar {

template<class MapType, class ElemType>
class Node {
public:
    Node() {}

    Node(const MapType &m, const ElemType &a) : _elem(a), _map(&m), mParent(NULL), mCostFromStart(0), mTotalCost(0) {}

    bool operator>(const Node& node) const {
        return mTotalCost > node.mTotalCost;
    }

    bool operator==(const Node& node) const {
        return _map->elementsEqual(node._elem, _elem);
    }

    const ElemType getElem() const { return _elem; }

private:
  ElemType _elem;
  const MapType* _map;

public:
  Node *mParent;
  float mCostFromStart;
  float mTotalCost;
};

template<class A, class B>
struct openListGtrCompare : public std::binary_function<A, B, bool> {
    bool operator()(const A& a, const B& b) {
        return *a > *b;
    }
};

template<class A, class B>
struct openListEqualCompare : public std::binary_function<A, B, bool> {
    bool operator()(const A& a, const B& b) {
        return *a == *b;
    }
};

// MapType must support the following operations:
//   - getCost(ElemType) -> float
//   - getHeuristicDistance(ElemType, ElemType) -> float
//   - getAdjacencies(ElemType) -> vector<ElemType>
template<class MapType, class ElemType>
std::vector<ElemType>
best_path(const MapType& map, const ElemType& start, const ElemType& end)
{
  typedef Node<MapType, ElemType> LocalNode;
  typedef IMutableHeap< boost::shared_ptr<LocalNode>,
    std::vector< boost::shared_ptr<LocalNode> >,
    openListGtrCompare< boost::shared_ptr<LocalNode>, boost::shared_ptr<LocalNode> >,
    openListEqualCompare< boost::shared_ptr<LocalNode>, boost::shared_ptr<LocalNode> > >
  OpenListPriorityQueue;

  typedef PriQueueMutableHeap< boost::shared_ptr<LocalNode>,
    std::vector< boost::shared_ptr<LocalNode> >,
    openListGtrCompare< boost::shared_ptr<LocalNode>, boost::shared_ptr<LocalNode> >,
    openListEqualCompare< boost::shared_ptr<LocalNode>, boost::shared_ptr<LocalNode> > >
  OpenListPriorityQueueImpl;

  std::vector<ElemType> results;
  boost::shared_ptr<OpenListPriorityQueue> openListPtr(new OpenListPriorityQueueImpl());
  OpenListPriorityQueue &openList = *openListPtr;

  std::vector< boost::shared_ptr<LocalNode> > closedList;

  LocalNode startNode(map, start), endNode(map, end);

  // fill in the start location cost since this was passed in as a location
  // with no cost associated with it
  startNode.mCostFromStart = map.getCost(start);

  bool reachedEndNode = false;
  LocalNode pathEndNode;

  // add start node to the open list

  openList.push(boost::shared_ptr<LocalNode>(new LocalNode(startNode)));

  // while the open list is not empty

  while(!openList.empty()) {

    //cout << "--- next open ---" << endl;

    // debug
    /*
    cout << "open list: " << endl;
    cout << "  " << openList << endl;
    cout << "closed list: " << endl;
    BOOST_FOREACH(boost::shared_ptr<LocalNode> ln, closedList) {
        const LocalNode &ln1 = *ln;
        cout << "  " << ln1 << endl;
    }
    */

    // grab current node with lowest F value
        
    boost::shared_ptr<LocalNode> node = openList.pop();
    
    // add node to the closed list

    closedList.push_back(node);

    // if node is the endNode then we're done
    if(*node == endNode) {
      reachedEndNode = true;
      pathEndNode = *node;
      break;
    }

    // get adjacencies
    std::vector<ElemType> adjacencies;
    map.getAdjacencies(node->getElem(), adjacencies);

    // debug
    /*
    cout << "adjacancies: " << endl;
    BOOST_FOREACH(ElemType r, adjacencies) {
        cout << "  " << r << endl;
    }
    */

    // loop through each element
    BOOST_FOREACH(ElemType r, adjacencies) {
      //cout << "-------- loop start ------------" << endl;

        boost::shared_ptr<LocalNode> n1(new LocalNode(map, r));

        // set parent to node
        n1->mParent = node.get();

        // find heuristic cost to goal
        float h = map.getHeuristicCost(n1->getElem(), endNode.getElem());

        // get estimated cost from start node
        float g = node->mCostFromStart + n1->getElem().cost;

        // get estimated total cost from start to end through this node
        float f = g + h;
        n1->mTotalCost = f;
        n1->mCostFromStart = g;

        // check if n1 is already in the open list and if so then if
        // it's better or the same then continue
	boost::shared_ptr<LocalNode> tmp;
	if(openList.find(n1, tmp))
	  if(tmp->mTotalCost <= n1->mTotalCost)
	    continue;
	
        // check if n1 is already in the closed list and if so then if
        // it's better or the same then continue
	bool found = false;
        typename std::vector< boost::shared_ptr<LocalNode> >::iterator i2 
            = closedList.begin();
        while(i2 != closedList.end()) {
            LocalNode *nn = i2->get();
            if(*nn == *n1) {
	      if(nn->mTotalCost <= n1->mTotalCost)
                found = true;
	      break;
            }
            i2++;
        }
        if(found)
            continue;

	// debug
	//cout << "open list (before erase): " << endl;
	//cout << "  " << openList << endl;

        // remove occurances of n1 from both open and closed lists if it exists
        // in either list
	openList.remove(n1);

        i2 = closedList.begin();
        while(i2 != closedList.end()) {
            LocalNode *nn = i2->get();
            if(*nn == *n1) {
	      //cout << "erasing: " << *nn << endl;
                closedList.erase(i2);
                break;
            }
            i2++;
        }

	// debug
	//cout << "open list (after erase): " << endl;
	//cout << "  " << openList << endl;

        // add n1 to the open list
        //cout << "n1 parent: " << hex << (int)(n1->mParent->mParent) << endl;
	//cout << "adding to open list: " << *n1 << endl;
        openList.push(n1);

        //cout << "openlist size = " << openListNodes.size() << endl;
        //cout << "closedlist size = " << closedList.size() << endl;
    }
  }

  // backtrack and copy results into the results buffer
  if(reachedEndNode) {
      LocalNode *n = &pathEndNode;
      while(n) {
	//cout << "here: " << hex << (int)n << endl;
          results.push_back(n->getElem());
          n = n->mParent;
      }
  }

  // return it in the forward order
  reverse(results.begin(), results.end());

  return results;
}


}; // namespace astar

#endif // __ASTAR_H__
