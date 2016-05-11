
#include <random>
#include <math.h>
#include "mcmc.h"

MCMC::MCMC(int N, int c, int q, default_random_engine &_generator)
  :generator(_generator)
{
  this->time = 0;
  this->beta = 1.;
  this->graph = new ErdosRenyi(N, (double)c/N, q, _generator);

  // update all distributions
  this->dist_color = new uniform_int_distribution<int>(1,q-1);
  this->dist_vertices = new uniform_int_distribution<int>(0,N-1);
  this->dist_U = new uniform_real_distribution<double>(0.0, 1.0);

  // initial energy
  this->energy.push_back(this->graph->hamiltonian());
}

MCMC::~MCMC()
{
  delete this->graph;
  delete this->dist_color;
  delete this->dist_vertices;
  delete this->dist_U;
}

void MCMC::move()
{
  this->time++;

  // reduce the temperature according to schedule
  this->cool();

  // pick vertex at random
  int v = (*this->dist_vertices)(this->generator);

  // pick color at random (diff from v)
  int c = (*this->dist_color)(this->generator);
  if (c >= this->graph->vertices[v].color)
    c++;

  // compute delta H
  int delta = this->graph->delta_h(v, c);

  if (delta <= 0)
  {
    // accept the move
    this->graph->vertices[v].color = c;
  }
  else
  {
    // generate random value
    double p = (*this->dist_U)(this->generator);
    
    // accept move wp exp(-beta*delta)
    if (p < exp(-this->beta*delta))
    {
      this->graph->vertices[v].color = c;
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
  // here we will update the value of beta
}

