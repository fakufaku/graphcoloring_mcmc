
#include <random>
#include <chrono>

using namespace std;

#include "graph.h"

Vertex::Vertex(int _c)
  : color(_c)
{
  // more stuff to do ?
}

std::ostream & operator<<(std::ostream &os, Vertex const &v)
{
  os << "Color: " << v.getColor() << ", Neighbors: ";
  for (auto it = v.neighbors.cbegin(); it != v.neighbors.cend(); ++it)
    os << " " << *it;

  return os;
}

Graph::Graph(int _size, double _p, int q, default_random_engine &generator)
  : size(_size)
{
  std::uniform_real_distribution<double> distribution(0.0,1.0);
  std::uniform_int_distribution<int> dist_color(1,q);

  // Add all vertices to the vector
  for (int u = 0 ; u < size ; u++)
    vertices.push_back(Vertex(dist_color(generator)));

  // Now draw all the edges at random in the graph
  for (int u = 0 ; u < size ; u++)
    for (int v = u+1 ; v < size ; v++)
    {
      // draw a random sample
      double b = distribution(generator);

      // add the edge with prob _p
      if (b < _p)
      {
        vertices[u].neighbors.push_back(v);
        vertices[v].neighbors.push_back(u);
      }

    }
}

Graph::Graph(int _size, int16_t *adjacency, int q, default_random_engine &generator)
  : size(_size)
{
  std::uniform_int_distribution<int> dist_color(1,q);

  // Add all vertices to the vector
  for (int u = 0 ; u < size ; u++)
    vertices.push_back(Vertex(dist_color(generator)));

  // Now draw all the edges at random in the graph
  for (int u = 0 ; u < size ; u++)
    for (int v = u+1 ; v < size ; v++)
    {
      // add the edge with if adjacency matrix has a one there
      if (adjacency[u*size + v] == 1)
      {
        vertices[u].neighbors.push_back(v);
        vertices[v].neighbors.push_back(u);
      }

    }
}

int Graph::is_coloring_legal(int u)
{
  // check if a vertex is legally colored 
  
  for (int i = 0 ; i < this->vertices[u].neighbors.size() ; i++)
  {
    int v = this->vertices[u].neighbors[i];

    if (this->vertices[u].color == this->vertices[v].color)
      return false;
  }

  return true;
}

int Graph::delta_h(int u, int new_color)
{
  int delta = 0;

  for (int i = 0 ; i < this->vertices[u].neighbors.size() ; i++)
  {
    int v = this->vertices[u].neighbors[i];

    if (new_color == this->vertices[v].color)
      delta++;

    if (this->vertices[u].color == this->vertices[v].color)
      delta--;
  }

  return delta;
}

int Graph::hamiltonian()
{
  /*
   * Counts the number of invalid neighbor
   */
  int H = 0;
  
  for (auto u = this->vertices.begin(); u != this->vertices.end(); ++u)
    for (int v = 0 ; v < u->neighbors.size() ; v++)
      if (u->color == this->vertices[u->neighbors[v]].color)
        H++;

  return H/2;
}

std::ostream &operator<<(std::ostream &os, Graph const &m)
{
  int i = 0;
  for (auto it = m.vertices.cbegin(); it != m.vertices.cend(); ++it)
  {
    os << i << ": " << *it << endl;
    i++;
  }
  os << '\n';

  return os;
}
