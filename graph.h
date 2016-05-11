
#include <vector.h>

class Vertex
{
  Vector(int _c);
  ~Vector();

  vector<int> *neighbors;
  int color;
};

class ErdosRenyi
{
  ErdosRenyi(int size, double p);
  ~ErdosRenyi();

  int size;
  vector<Vertex> *vertices;
};
