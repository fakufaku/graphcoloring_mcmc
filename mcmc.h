
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
    vector<double> energy;

    default_random_engine generator;
    uniform_int_distribution<int> dist_color(1,10);
    uniform_int_distribution<int> dist_vertices(0,9);
    std::uniform_real_distribution<double> dist_U(0.0, 1.0);

    ErdosRenyi *graph;

    MCMC(int N, int c, int q, default_random_engine generator);

    void move();
    void cool();
};
