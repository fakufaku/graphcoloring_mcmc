#include <iostream>
#include <chrono>
#include <random>

#include "mcmc.h"

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

  // Create the MCMC
  MCMC chain(1000, 100, 7, generator);

  chain.run(1000000);
  chain.save();

  return 0;
}
