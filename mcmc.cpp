
#include <iostream>
#include <random>
#include <math.h>
#include <iostream>
#include <fstream>
#include "mcmc.h"

using namespace std;

MCMC::MCMC(Graph *G, int q, default_random_engine &_generator,
    int32_t *_energy_history, double *_beta_history,
    int p_int, double p_double, int _schedule)
  :energy(_energy_history), beta_history(_beta_history), generator(_generator), graph(G),
  param1_int(p_int), param2_double(p_double), schedule(_schedule)
{
  this->time = 0;

  // initial value for beta
  this->beta = 0.4;
  this->beta0 = this->beta;

  // update all distributions
  this->dist_color = new uniform_int_distribution<int>(1,q-1);
  this->dist_vertices = new uniform_int_distribution<int>(0,G->size-1);
  this->dist_U = new uniform_real_distribution<double>(0.0, 1.0);

  // initial energy
  this->H = this->graph->hamiltonian();
  this->H0 = this->H;

  this->last_H = this->H;
  this->last_beta = this->beta0;
  this->last_last_beta = 0.;

  // initialize the best current coloring
  this->best_coloring = new int[this->graph->size];
  save_coloring();

  // save history
  if (this->energy != NULL)
    this->energy[0] = this->H;
  if (this->beta_history != NULL)
    this->beta_history[0] = this->beta;
  if (this->energy != NULL)
    cout << this->energy.size() << endl;
}

MCMC::~MCMC()
{
  delete this->best_coloring;
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
    this->n_move++;
    this->total_move++;
  }
  else
  {
    // generate random value
    double p = (*this->dist_U)(this->generator);
    
    // accept move wp exp(-beta*delta)
    if (p < exp(-this->beta*delta))
    {
      this->graph->vertices[v].color = c;
      this->n_move++;
      this->total_move++;
    }
    else
    {
      // energy doesn't change
      delta = 0;
    }
  }

  // update energy
  this->H += delta;
  
  // reduce the temperature according to schedule
  if (this->H != 0)
      this->cool();

  // save the history
  if (this->energy != NULL)
    this->energy[time] = this->H;
  if (this->beta_history != NULL)
    this->beta_history[time] = this->beta;
}

void MCMC::cool()
{
  switch (this->schedule)
  {
    case 0:
      if (n_move == param1_int)
      {
        this->beta = this->beta0*log(1. + param2_double*this->time);
        n_move = 0;
      }
      break;

    case 1:
      // Trying to generalize
      if (this->H > this->H0/10)
      {
        if (this->time % 4000 == 0)
            this->beta *= 1. + param2_double;
      }
      else if (this->H > this->H0/20)
      {
        if (this->time % 40000 == 0)
            this->beta *= 1. + param2_double;
      }
      else if (this->H > this->H0/100)
      {
        if (this->time % 400000 == 0)
            this->beta *= 1. + param2_double/2;
      }
      else
      {
        if (this->time % 4000000 == 0)
            this->beta *= 1. + param2_double/10;
      }
      break;

    case 2:
      // This schedule works great for q=3 d=4
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

      break;

  
    case 3:

      if (n_move == param1_int)
      {
        int frac = this->H0/this->H;
        if (frac < 1)
          frac = 1;

        this->beta *= 1. + param2_double/(frac);
        n_move = 0;
      }

      break;

    case 4:
      /* The first naive strategy we tried */
      if (this->time % ((this->time/60000 + 1)*2000) == 0)
          this->beta *= 1.105;
      break;

    case 5:
      // Implementing something close to gradient descent every M iterations with step sixe mu
      if (this->n_move == param1_int)
      {
        double db = last_beta - last_last_beta;
        int dH = this->H - last_H;

        if (db <= 0.0001)
          db = 0.0001;

        this->beta = last_beta - param2_double * (dH/db);
        if (this->beta <= 0)
          this->beta = this->beta0;

        last_last_beta = last_beta;
        last_beta = this->beta;
        last_H = this->H;
        n_move = 0;
      }
      break;

  }
}

void MCMC::run(unsigned long n_steps)
{
  for (int i = 0 ; i < n_steps ; i++)
  {
    // Make your move!
    this->move();

    // keep track of minimum energy configuration
    if (this->H0 > 100*this->H && this->H < this->best_energy)
      this->save_coloring();

    // Stop if we reach 0 energy
    if (this->H == 0)
      return;
  }
}

void MCMC::save_coloring()
{
  for (int i = 0 ; i < this->graph->vertices.size() ; i++)
    this->best_coloring[i] = this->graph->vertices[i].color;
  this->best_energy = this->H;
}

void MCMC::save()
{
  ofstream myfile;
  myfile.open ("mcmc.txt");
  for (int i = 0 ; i < this->time ; i++)
    myfile << this->beta_history[i] << " " << this->energy[i] << endl;
  myfile.close();
}
