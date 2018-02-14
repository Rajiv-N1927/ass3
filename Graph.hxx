//Public functions
template <typename N, typename E>
Graph<N, E>::Graph() : noEdges(0), noNodes(0) {}
template <typename N, typename E>
Graph<N, E>::Graph(const Graph<N, E>& g) :
  noEdges (g.noEdges), noNodes (g.noNodes),
  gNodes (g.gNodes), gEdges (g.gEdges) {}

template <typename N, typename E>
Graph<N, E>::Graph(const Graph<N, E>&& g) :
  Graph(g) {
  g.noEdges = 0;
  g.noNodes = 0;
  g.gEdges.erase(gEdges.begin(), gEdges.end());
  g.gNodes.erase(gNodes.begin(), gNodes.end());
}

template <typename N, typename E>
Graph<N, E>& Graph<N, E>::operator=(const Graph<N, E>& g) {
  if ( &g != this ) {
    this->noNodes = g.noNodes;
    this->noEdges = g.noEdges;
    this->gNodes = g.nNodes;
    this->gEdges = g.gEdges;
  }
  return *this;
}

template <typename N, typename E>
Graph<N, E>& Graph<N, E>::operator=(const Graph<N, E>&& g) {
  if ( &g != this ) {
    this->noNodes = g.noNodes;
    this->noEdges = g.noEdges;
    this->gNodes = g.nNodes;
    this->gEdges = g.gEdges;

    g.noEdges = 0;
    g.noNodes = 0;
    g.gEdges.erase(gEdges.begin(), gEdges.end());
    g.gNodes.erase(gNodes.begin(), gNodes.end());
  }
  return *this;
}
//Adding the Nodes
template <typename N, typename E>
bool Graph<N, E>::addNode(const N& val) {
  bool created = true;
  if ( findNVal(val) ) {
    created = false;
    std::cout << "Not created" << std::endl;
  } else {
    std::shared_ptr<Node> newNode
      = std::make_shared<Node>(val);
    gNodes.push_back(newNode);
    noNodes++;
    std::cout << "Created" << std::endl;
  }
  return created;
}
//Adding the edges
template <typename N, typename E>
bool Graph<N, E>::addEdge(const N& src, const N& dst, const E& w) {
  bool toAdd = true;
  int srcPos, dstPos;
  if ( findNVal(src, srcPos) && findNVal(dst, dstPos) ) {
    //Get the src and dest nodes
    auto srcIt = gNodes.begin() + srcPos;
    auto dstIt = gNodes.begin() + dstPos;
    Edge toCheck(*srcIt, *dstIt, w);

    for ( auto Eit = gEdges.begin(); Eit != gEdges.end(); Eit++ ) {
      if ( auto edptr = Eit->lock() ) {
        //std::cout << edptr->getSrc() << " "
        //  << edptr->getDest() << " " << edptr->getWeight() << std::endl;
        if ( *edptr ==  toCheck ) {
          std::cout << "Edge not created" << std::endl;
          toAdd = false;
          break;
        }
      } else {
        std::cout << "Not lockable" << std::endl;
      }
    }
    if ( toAdd ) {
      //Create a ptr
      std::shared_ptr<Edge> newEdge = std::make_shared<Edge>(*srcIt, *dstIt, w);
      std::weak_ptr<Edge> wp(newEdge);
      (*srcIt)->addEdge(newEdge);
      gEdges.push_back(wp);
      noEdges++;
      std::cout << "Edge created, Num of edges: " << gEdges.size() << std::endl;
      //std::cout << "Printing Edges for " << (*srcIt)->getVal() << std::endl;
      //(*srcIt)->printEdges();
    }
  } else {
    std::cout << "Edge error 1 or both nodes don't exist" << std::endl;
  }
  return toAdd;
}
//--------------Deleting Edges and Nodes------------//
template <typename N, typename E>
void Graph<N, E>::deleteNode(const N& nToDel) noexcept {
  int srcPos;
  if ( findNVal(nToDel, srcPos) ) {
    auto srcIt = gNodes.begin() + srcPos;
    std::cout << "Node " << (*srcIt)->getVal() << " Deleted" << std::endl;
    (*srcIt)->resetEdges();
    gNodes.erase(srcIt);

    std::cout << "Num of edges after node deletion: "
      << gEdges.size() << std::endl;
    printAllEdges();
  } else {
    std::cout << "Node doesn't exist" << std::endl;
  }
}
//------------------Replacing-------------------//

template <typename N, typename E>
bool Graph<N, E>::replace(const N& oldData, const N& newData) {
  bool replaced = true;
  int srcPos, dstPos;
  if ( findNVal(newData) ) replaced = false;
  else if ( findNVal(oldData, srcPos) ) {
    //std::cout << "Replaced" << std::endl;
    auto srcIt = gNodes.begin() + srcPos;
    (*srcIt)->setVal(newData);
    printAllEdges();
  } else {
    std::cout << "error thrown oldData not found in replace" << std::endl;
  }
  return replaced;
}

//----------------------------------------------//
template <typename N, typename E>
void Graph<N, E>::deleteEdge(const N& src, const N& dst, const E& w) noexcept {
  int srcPos, dstPos;
  if ( findNVal(src, srcPos) && findNVal(dst, dstPos) ) {
    auto srcIt = gNodes.begin() + srcPos;
    auto dstIt = gNodes.begin() + dstPos;
    (*srcIt)->deleteNEdge(**dstIt, w);
  } else {
    std::cout << "one or two nodes dont exist delEdge" << std::endl;
  }
}
//--------------------------------------------------//
//-------------Graph Checking Operations------------//
//Clearing the Graph
template <typename N, typename E>
void Graph<N, E>::clear() noexcept {
  for ( auto it = gNodes.begin(); it != gNodes.end(); it++ ) {
    (*it)->resetEdges();
  }
  gNodes.erase(gNodes.begin(), gNodes.end());
  gEdges.erase(gEdges.begin(), gEdges.end());
}

template <typename N, typename E>
bool Graph<N, E>::isNode(const N& val) const {
  return findNVal(val);
}

template <typename N, typename E>
bool Graph<N, E>::isConnected(const N& src, const N& dst) const {
  bool isConn = false;
  int srcPos;
  if ( findNVal(src, srcPos) && findNVal(dst) ) {
    //Get the src and dest nodes
    auto srcIt = gNodes.begin() + srcPos;
    isConn = (*srcIt)->isConnected(dst);
  } else {
    std::cout << "Runtime error in isConnected()" << std::endl;
  }
  return isConn;
}

//--------------------------------------------------//

//Printing functions (unsorted for now)
template <typename N, typename E>
void Graph<N, E>::printNodes() const {
  for ( auto it = gNodes.begin(); it != gNodes.end(); it++ ) {
    std::cout << (*it)->getVal() << std::endl;
  }
}
template <typename N, typename E>
void Graph<N, E>::printEdges(const N& val) const {
  int srcPos;
  if ( findNVal(val, srcPos) ) {
    auto it = gNodes.begin() + srcPos;
    (*it)->printEdges();
  } else {
    std::cout << "Value doesn't exist to print edges" << '\n';
  }
}
//private functions
template <typename N, typename E>
void Graph<N, E>::printAllEdges() const {
  for ( auto Eit = gEdges.begin(); Eit != gEdges.end(); Eit++ ) {
    if ( auto edptr = Eit->lock() ) {
      std::cout << edptr->getSrc() << " "
        << edptr->getDest() << " " << edptr->getWeight() << std::endl;
    } else {
      std::cout << "Not lockable" << std::endl;
    }
  }
}
template <typename N, typename E>
bool Graph<N, E>::findNVal(const N& val) const {
  bool valFound = false;
  for ( auto it = gNodes.begin(); it != gNodes.end(); it++ ) {
    if ( (*it)->getVal() == val ) {
      valFound = true;
      break;
    }
  }
  return valFound;
}

template <typename N, typename E>
bool Graph<N, E>::findNVal(const N& val, int& pos) const {
  bool valFound = false;
  pos = 0;
  for ( auto it = gNodes.begin(); it != gNodes.end(); it++, pos++ ) {
    if ( (*it)->getVal() == val ) {
      valFound = true;
      break;
    }
  }
  return valFound;
}
