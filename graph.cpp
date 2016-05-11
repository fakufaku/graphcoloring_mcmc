
#include <random>

using namespace std;

#include "graph.h"

Vertex::Vertex(int _c)
  : color(_c)
{
  // more stuff to do ?
}

std::ostream & operator<<(std::ostream &os, Vertex const &v)
{
  os << "Color: " << v.color;
  for (auto it = v.neighbors.cbegin(); it != v.neighbors.cend(); ++it)
    os << " " << *it;

}

ErdosRenyi::ErdosRenyi(int _size, double _p)
  : size(_size)
{
  // Create a uniform distribution in [0,1]
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0.0,1.0);


  // Now draw all the edges at random in the graph
  for (int u = 0 ; u < size ; u++)
  {
    vertices.push_back(Vertex(0));

    for (int v = u+1 ; v < size ; v++)
    {
      // draw a random sample
      double b = distribution(generator);

      // add the edge with prob _p
      if (b < _p)
        vertices[u].neighbors.push_back(v);

    }
  }
}

std::ostream &operator<<(std::ostream &os, ErdosRenyi const &m)
{
  int i = 0;
  for (auto it = m.vertices.cbegin(); it != m.vertices.cend(); ++it)
  {
    os << i << ": " << *it << endl;
    i++;
  }
  os << '\n';
}
