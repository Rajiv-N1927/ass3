#include <stdlib.h>
#include "Graph.h"
int main(int argc, char ** argv) {
  Graph<char, int> test;
  test.addNode('a');
  test.addNode('b');
  test.addNode('c');

  test.addEdge('a', 'c', 1);
  test.addEdge('a', 'c', 2);
  test.addEdge('a', 'b', 2);
  test.addEdge('a', 'b', 3);

  test.replace('a', 'e');
  test.deleteNode('a');
  test.deleteNode('e');
  return 0;
}
