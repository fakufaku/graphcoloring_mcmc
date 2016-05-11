
#include <random>

#include "graph.h"

Vertex::Vertex(int _c)
  color(_color)
{

}

ErdosRenyi::ErdosRenyi(int _size, double _p)
  size(_size)
{
  // Create a uniform distribution in [0,1]
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0.0,1.0);

  // Create the vector of vertices
  vertices = new vector<Vertex>(_size);

  // Now draw all the edges at random in the graph
  for (int u = 0 ; u < size ; u++)
    for (int v = u+1 ; v < size ; v++)
    {
      // draw a random sample
      double b = distribution(generator);

      // add the edge with prob _p
      if (b < _p)
        vertices[u].push_back(v);

    }
}
