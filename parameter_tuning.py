
from __future__ import division

import numpy as np
import matplotlib.pyplot as plt
from scipy.io import loadmat
import networkx as nx
import itertools as it

import ipyparallel as ip

# custom modules here
import util
import pymcmc

# number of nodes
N = 1000

# average node degree
D = [3,11,13,15,18,21,23]

# number of colors to use
Q = [3,5,7,9]

# parameters to tune
schedule = [0,3,5]
params1 = [ [250, 500, 1000, 2000, 4000, 8000, 16000],
            [250, 500, 1000, 2000, 4000, 8000, 16000],
            [12500, 25000, 100000, 200000, 400000, 8000000], ]
params2 = [ np.logspace(np.log2(0.0001), np.log2(0.5), num=5, base=2),
            np.logspace(np.log2(0.0001), np.log2(0.5), num=5, base=2),
            np.logspace(np.log2(0.00001), np.log2(0.01), num=5, base=2), ]

nsched = 0 # choice of schedule to tune

# number of iterations
iterations = 2000000

# number of loops for every parameters combination
loops = 10

c = ip.Client()
print c.ids

NC = len(c.ids)
print NC,'workers on the job'

nv = []
dv = []
qv = []
num_iter_v = []
p1v = []
p2v = []
schedv = []

cartesian = [e for e in it.product([N],D,Q,params1[nsched],params2[nsched].tolist(),[schedule[nsched]],[iterations],[loops])]

print 'Number of combinations:',len(cartesian)



def run_mcmc(arg):
    '''
    Just a wrapper to call the coloring algorithm;
    '''

    import pymcmc
    import numpy as np
    import networkx as nx

    n = arg[0]
    d = arg[1]
    q = arg[2]
    p1 = int(arg[3])
    p2 = float(arg[4])
    schedule = arg[5]
    num_iter = arg[6]
    loops = arg[7]

    # receptacle arrays
    coloring = np.zeros((n), dtype=np.int16)
    eh = np.zeros(0, dtype=np.int32)
    bh = np.zeros(0, dtype=np.double)

    energy = 0.
    for l in xrange(loops):
        G = nx.erdos_renyi_graph(n, d/n)
        A = np.array(nx.adjacency_matrix(G).todense(), dtype=np.int16)

        energy += pymcmc.color_graph(A, q, num_iter, coloring, eh, bh, p1, p2, schedule)

    energy /= loops

    return energy

# run many instances in parallel
out = c[:].map_sync(run_mcmc, cartesian)

np.savez('tuned_parameters_schedule'+str(schedule[nsched])+'.npz',
    N=N, D=D, Q=Q, schedule=schedule[nsched], 
    param1=params1[nsched], param2=params2[nsched], 
    iterations=iterations, cartesian=cartesian, energy=out)

