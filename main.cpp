#include <iostream>
#include <chrono>
#include <random>

#include "graph.h"

using namespace std;

int main(int argc, char** argv)
{
  // Initialize the RNG
  typedef std::chrono::high_resolution_clock myclock;
  myclock::time_point beginning = myclock::now();
  for (int i = 0 ; i < 1000 ; i++)
    ;
  myclock::duration d = myclock::now() - beginning;
  unsigned seed = d.count();
  std::default_random_engine generator(seed);

  // Create a graph
  ErdosRenyi g(10, 0.5, 4, generator);

  cout << g;

  return 0;
}
