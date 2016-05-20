#include <iostream>
#include <chrono>
#include <random>

#include "mcmc.h"

#define N_GRAPH 1000
#define N_STEPS 4000000
#define Q_MIN 4
#define Q_MAX 4
#define C_MIN 5
#define C_MAX 10

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

  MCMC *chain;

  for (int q = Q_MIN ; q <= Q_MAX ; q += 3)
  {
    for (int c = C_MIN ; c <= C_MAX ; c += 1)
    {
      // Create the MCMC
      chain = new MCMC(N_GRAPH, c, q, generator);

      chain->run(N_STEPS);
      cout << q << " " << c << " " << chain->get_energy() << endl;

      delete chain;
    }
  }

  chain->save();


  return 0;
}
