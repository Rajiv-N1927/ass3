#include <iostream>
#include <map>
#include <vector>

template <typename N, typename E> class Graph {
public:
  //Constructors and Assignment operators
Graph();
Graph(const Graph& g); //Copy Constructor
Graph(const Graph&& g); //Move Constructor
Graph& operator=(const Graph& g); //Copy Assignment operator
Graph& operator=(const Graph&& g); //Move Assignment operator
//Edge and Node operations
bool addNode(const N& val);
bool addEdge(const N& src, const N& dst, const E& w);
bool replace(const N& oldData, const N& newData);
void mergeReplace(const N& oldData, const N& newData);
void deleteNode(const N&) noexcept;
void deleteEdge(const N& src, const N& dst, const E& w) noexcept;
//Higher level (graph) operations
void clear() noexcept;
bool isNode(const N& val) const;
bool isConnected(const N& src, const N& dst) const;
void printNodes() const;
void printEdges(const N& val) const;
//Iterator functions
void begin() const;
bool end() const;
void next() const;
const N& value() const;
private:
  class Edge; class Node;

  class Node {
  public:
    Node(const N& Nval) : val(Nval), noEdgeOut(0) {}
    ~Node() {
      //std::cout << "Node destructor called "
      //  << this->getVal() << std::endl;
    }
    //Add the edge to the vector
    void addEdge(std::shared_ptr<Edge> e) {
      this->RefEdge.push_back(e);
      noEdgeOut++;
    }
    void deleteNEdge(const Node& n, const E& w) {
      Edge toRem(*this, n, w);
      for ( auto it = RefEdge.begin(); it != RefEdge.end(); it++ ) {
        if ( **it == toRem ) {
          RefEdge.erase(it);
          noEdgeOut--;
        }
      }
    }
    //Check if edge is is connected
    bool isConnected(const N& val) {
      bool isConn = false;
      for ( auto it = RefEdge.begin(); it != RefEdge.end(); it++ ) {
        if ( (*it)->getSrc() == this->getVal()
          && (*it)->getDest() == val ) {
          isConn = true;
        }
      }
      return isConn;
    }
    //Reset the Edges
    void resetEdges() {
    //RefEdge.erase(RefEdge.begin(), RefEdge.end());
      auto it = RefEdge.begin();
      while ( it != RefEdge.end() ) {
        it->reset();
        it = RefEdge.erase(it);
        it++;
      }
    }
    //Print the edges
    void printEdges() {
      std::cout << "Edges attached to Node " << val << std::endl;
      if ( noEdgeOut == 0 ) { std::cout << "null" << std::endl; }
      else {
        for ( auto it = RefEdge.begin(); it != RefEdge.end(); it++ ) {
          std::cout << (*it)->getDest() << " "
            << (*it)->getWeight() << std::endl;
        }
      }
    }
    //Get and Set value
    void setVal(const N& val) { this->val = val; }
    const N& getVal() const { return this->val; }
  private:
    mutable N val;
    std::vector<std::shared_ptr<Edge>> RefEdge;
    int noEdgeOut = 0;
  };

  class Edge {
  public:
    Edge(std::shared_ptr<Node> start,
        std::shared_ptr<Node> dst, const E& newton) :
      src(start), dest(dst), weight(newton) {}
    ~Edge() {
      //std::cout << "Edge destructor called" << std::endl;
      //std::cout << this->getSrc() << " "
      //  << this->getDest() << " " << this->getWeight() << std::endl;
    }
    //Overload the == operator
    bool operator==(const Edge& toComp) {
    bool comp = false;
    if ( this->src->getVal() == toComp.src->getVal()
      && this->dest->getVal() == toComp.dest->getVal()
      && this->weight == toComp.weight ) {
        comp = true;
      }
    return comp;
    }
    //Getters
    const N& getSrc() { return this->src->getVal(); }
    const N& getDest() { return this->dest->getVal(); }
    const E& getWeight() { return this->weight; }

  private:
    std::shared_ptr<Node> src, dest;
    const E weight;
  };

  //Private graph variables
  int noNodes, noEdges;
  std::vector< std::shared_ptr<Node> > gNodes;
  std::vector< std::weak_ptr<Edge> > gEdges;
  void printAllEdges() const;
  //Private functions
  bool findNVal(const N&) const;
  bool findNVal(const N&, int& ) const;
};


#include "Graph.hxx"
