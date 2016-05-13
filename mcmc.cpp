
#include <iostream>
#include <random>
#include <math.h>
#include <iostream>
#include <fstream>
#include "mcmc.h"

using namespace std;

MCMC::MCMC(int N, int c, int q, default_random_engine &_generator)
  :generator(_generator)
{
  this->time = 0;
  this->beta = 1.e-2;
  this->graph = new ErdosRenyi(N, (double)c/N, q, _generator);

  // update all distributions
  this->dist_color = new uniform_int_distribution<int>(1,q-1);
  this->dist_vertices = new uniform_int_distribution<int>(0,N-1);
  this->dist_U = new uniform_real_distribution<double>(0.0, 1.0);

  // initial energy
  this->H = this->graph->hamiltonian();
  this->energy.push_back(this->H);
  this->beta_history.push_back(this->beta);
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

  // pick vertex at random (that is not legally colored
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
      this->graph->vertices[v].color = c;
    else
      // energy doesn't change
      delta = 0;
  }

  // update energy
  this->H += delta;
  this->energy.push_back(this->H);

  // if we did a move uphill, rewind beta
  if (delta > 0)
    this->beta = this->beta_history.back();

  // save beta in the history
  this->beta_history.push_back(this->beta);
}

void MCMC::cool()
{
#define ALPHA 0.005
  double dE = this->energy.back() - this->energy[this->energy.size()-2];
  //if (dE < 0.)
    if (this->time % ((this->time/60000 + 1)*2000) == 0)
      //this->beta *= 1. + this->energy.back()/this->graph->size;
      this->beta *= 1.105;
    if (this->beta > this->energy[0])
      this->beta = this->energy[0];

}

void MCMC::run(int n_steps)
{
  for (int i ; i < n_steps ; i++)
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
