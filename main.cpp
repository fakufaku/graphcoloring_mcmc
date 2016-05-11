#include <iostream>

#include "graph.h"

using namespace std;

int main(int argc, char** argv)
{
  ErdosRenyi g(10, 0.1);

  cout << g;

  return 0;
}
