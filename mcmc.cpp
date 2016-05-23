
#include <iostream>
#include <random>
#include <math.h>
#include <iostream>
#include <fstream>
#include "mcmc.h"

using namespace std;

MCMC::MCMC(Graph *G, int q, default_random_engine &_generator,
    int32_t *_energy_history, double *_beta_history)
  :generator(_generator), graph(G), energy(_energy_history), beta_history(_beta_history)
{
  this->time = 0;
  this->beta = 1.e-2;

  // update all distributions
  this->dist_color = new uniform_int_distribution<int>(1,q-1);
  this->dist_vertices = new uniform_int_distribution<int>(0,G->size-1);
  this->dist_U = new uniform_real_distribution<double>(0.0, 1.0);

  // initial energy
  this->H = this->graph->hamiltonian();
  this->H0 = this->H;

  // save history
  if (this->energy != NULL)
    this->energy[0] = this->H;
  if (this->beta_history != NULL)
    this->beta_history[0] = this->beta;
}

MCMC::~MCMC()
{
  delete this->dist_color;
  delete this->dist_vertices;
  delete this->dist_U;
}

void MCMC::move()
{
  this->time++;

  // pick vertex at random (that is not legally colored
  int v = (*this->dist_vertices)(this->generator);

  // pick color at random (diff from v)
  int c = (*this->dist_color)(this->generator);
  if (c >= this->graph->vertices[v].color)
    c++;

  // compute delta H
  delta = this->graph->delta_h(v, c);

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
      this->graph->vertices[v].color = c;
    else
      // energy doesn't change
      delta = 0;
  }

  // update energy
  this->H += delta;
  
  // reduce the temperature according to schedule
  this->cool();

  // save the history
  if (this->energy != NULL)
    this->energy[time] = this->H;
  if (this->beta_history != NULL)
    this->beta_history[time] = this->beta;
}

void MCMC::cool()
{
  static int stuck_count = 0;

  // This schedule works great for q=3 d=4
  /*
  if (this->H > this->H0/10)
  {
    if (this->time % 4000 == 0)
        this->beta *= 1.105;
  }
  else if (this->H > this->H0/20)
  {
    if (this->time % 40000 == 0)
        this->beta *= 1.105;
  }
  else if (this->H > this->H0/100)
  {
    if (this->time % 400000 == 0)
        this->beta *= 1.05;
  }
  else
  {
    if (this->time % 400000 == 0)
        this->beta *= 1.01;
  }
  */

  
  // Trying to generalize
  /*
#define ALPHA 0.105
  if (this->H > this->H0/10)
  {
    if (this->time % 4000 == 0)
        this->beta *= 1. + ALPHA;
  }
  else if (this->H > this->H0/20)
  {
    if (this->time % 40000 == 0)
        this->beta *= 1. + ALPHA;
  }
  else if (this->H > this->H0/100)
  {
    if (this->time % 400000 == 0)
        this->beta *= 1. + ALPHA/2;
  }
  else
  {
    if (this->time % 4000000 == 0)
        this->beta *= 1. + ALPHA/10;
  }
  */

#define ALPHA 0.005
#define GAMMA 8000

  int frac = this->H0/this->H;
  if (frac < 1)
    frac = 1;
  int freeze_time = GAMMA*frac;

  if (this->time % freeze_time == 0)
    this->beta *= 1. + ALPHA/(frac);

  /*
#define ALPHA 0.00005
  this->beta = log(1. + ALPHA*this->time);
  */

  /*
  if (this->time % ((this->time/60000 + 1)*2000) == 0)
      this->beta *= 1.105;
  */

  if (this->beta > this->H0)
    this->beta = this->H0;

}

void MCMC::run(int n_steps)
{
  for (int i = 0 ; i < n_steps ; i++)
  {
    this->move();
    if (this->H == 0)
      return;
  }
}

void MCMC::save()
{
  ofstream myfile;
  myfile.open ("mcmc.txt");
  for (int i = 0 ; i < this->time ; i++)
    myfile << this->beta_history[i] << " " << this->energy[i] << endl;
  myfile.close();
}
