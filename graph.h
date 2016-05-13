
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

class ErdosRenyi
{
  public:
    int size;
    vector<Vertex> vertices;

    ErdosRenyi(int size, double p, int q, default_random_engine &generator);

    int is_coloring_legal(int u);
    int delta_h(int u, int new_color);
    int hamiltonian();

    friend std::ostream &operator<<(std::ostream &os, ErdosRenyi const &m);
};
