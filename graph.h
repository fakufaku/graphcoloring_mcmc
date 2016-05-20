#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <cstdint>
#include <vector>
using namespace std;

class Vertex
{
  public:
    vector<int> neighbors;
    int color;

    Vertex(int _c);

    friend std::ostream &operator<<(std::ostream &os, Vertex const &m);
};

class Graph
{
  public:
    int size;
    vector<Vertex> vertices;

    Graph(int size, double p, int q, default_random_engine &generator);
    Graph(int size, int16_t *adjacency, int q, default_random_engine &generator);

    int is_coloring_legal(int u);
    int delta_h(int u, int new_color);
    int hamiltonian();

    friend std::ostream &operator<<(std::ostream &os, Graph const &m);
};

#endif // __GRAPH_H__
