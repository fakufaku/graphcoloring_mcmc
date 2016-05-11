
#include <math>
#include "mcmc.h"

MCMC::MCMC(int N, int c, int q, default_random_engine &_generator)
  :generator(_generator)
{
  this->time = 0;
  this->beta = 1.;
  this->graph = new ErdosRenyi(N, (double)c/N, q, _generator);

  // update color and vertices distributions
  this->dist_color.b = q-1;
  this->dist_vertices.b = N-1;

  // initial energy
  this->energy.push_back(this->graph.hamiltonian());
}

void MCMC::move()
{
  this->time++;

  // reduce the temperature according to schedule
  this->cool();

  // pick vertex at random
  v = this->dist_vertices(this->generator);

  // pick color at random (diff from v)
  c = this->dist_color(this->generator);
  if (c >= this->graph.vertices[v].color)
    c++;

  // compute delta H
  delta = this->graph.delta_h(v, c);

  if (delta <= 0)
  {
    // accept the move
    this->graph.vertices[v].color = c;
  }
  else
  {
    // generate random value
    double p = this->dist_U(this->generator);
    
    // accept move wp exp(-beta*delta)
    if (p < exp(-this->beta*delta))
    {
      this->graph.vertices[v].color = c;
    }
    else
    {
      // energy doesn't change
      delta = 0;
    }
  }

  // update energy
  this->energy.push_back(this->energy[this->energy.size()-1] + delta);
}

void MCMC::cool()
{

}

