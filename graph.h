
#include <vector>
using namespace std;

class Vertex
{
  public:
    vector<int> neighbors;
    int color;

    Vertex(int _c);
    ~Vertex();

    friend std::ostream &operator<<(std::ostream &os, Vertex const &m);
};

class ErdosRenyi
{
  public:
    int size;
    vector<Vertex> vertices;

    ErdosRenyi(int size, double p);
    ~ErdosRenyi();

    friend std::ostream &operator<<(std::ostream &os, ErdosRenyi const &m);
};
