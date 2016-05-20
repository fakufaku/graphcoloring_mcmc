#ifndef __MCMC_H__
#define __MCMC_H__

#include <cstdint>
#include <vector>
#include <random>
#include "graph.h"

class MCMC
{
  public:
    
    int time;
    int c;
    int q;
    double beta;
    int H;
    int delta;
    int H0;
    int32_t *energy = NULL;
    double *beta_history = NULL;

    default_random_engine generator;
    uniform_int_distribution<int> *dist_color;
    uniform_int_distribution<int> *dist_vertices;
    uniform_real_distribution<double> *dist_U;

    Graph *graph;

    MCMC(Graph *G, int q, default_random_engine &generator, 
        int32_t *energy_history, double *beta_history);
    ~MCMC();

    void move();
    void cool();
    void run(int n_steps);

    double get_energy() { return this->H; };

    void save();
};

#endif // __MCMC_H__
