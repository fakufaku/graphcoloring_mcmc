#ifndef __MCMC_H__
#define __MCMC_H__

#include <cstdint>
#include <vector>
#include <random>
#include "graph.h"

class MCMC
{
  public:
    
    unsigned long time;
    int c;
    int q;
    double beta;
    double beta0;
    int H;
    int delta;
    int H0;
    int32_t *energy = NULL;
    double *beta_history = NULL;

    unsigned long n_move = 0;
    unsigned long total_move = 0;

    // keep track of best solution seen so far
    int *best_coloring;
    int best_energy; 

    default_random_engine generator;
    uniform_int_distribution<int> *dist_color;
    uniform_int_distribution<int> *dist_vertices;
    uniform_real_distribution<double> *dist_U;

    Graph *graph;

    int param1_int = 0;
    double param2_double = 0.;
    int schedule = 0;

    int last_H = 0;
    double last_last_beta = 0.;
    double last_beta = 0.;

    MCMC(Graph *G, int q, default_random_engine &generator, 
        int32_t *energy_history, double *beta_history, 
        int p_int, double p_double, int _schedule);
    ~MCMC();

    void move();
    void cool();
    void run(unsigned long n_steps);
    void save_coloring();

    double get_energy() { return this->H; };

    void save();
};

#endif // __MCMC_H__
