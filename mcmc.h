
#include <vector>
#include "graph.h"

class MCMC
{
  public:
    
    int time;
    int N;
    int c;
    int q;
    double beta;
    vector<double> beta_history;
    vector<double> energy;

    default_random_engine generator;
    uniform_int_distribution<int> *dist_color;
    uniform_int_distribution<int> *dist_vertices;
    uniform_real_distribution<double> *dist_U;

    ErdosRenyi *graph;

    MCMC(int N, int c, int q, default_random_engine &generator);
    ~MCMC();

    void move();
    void cool();
    void run(int n_steps);

    void save();
};
