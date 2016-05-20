#ifndef __MCMC_H__
#define __MCMC_H__

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
    vector<double> beta_history;
    vector<int> energy;

    default_random_engine generator;
    uniform_int_distribution<int> *dist_color;
    uniform_int_distribution<int> *dist_vertices;
    uniform_real_distribution<double> *dist_U;

    Graph *graph;

    MCMC(Graph *G, int q, default_random_engine &generator);
    ~MCMC();

    void move();
    void cool();
    void run(int n_steps);

    double get_energy() { return this->energy.back(); };

    void save();
};

#endif // __MCMC_H__
