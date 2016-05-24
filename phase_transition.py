
from __future__ import division

import numpy as np
import matplotlib.pyplot as plt
from scipy.io import loadmat
import networkx as nx

# custom modules here
import util
import pymcmc

# number of nodes
n = 1000 

# number of colors to use
Q = np.arange(3,50)

# number of iterations
num_iter = 10000000

# energy
dmax = np.zeros((Q.shape[0]))
first_fail = np.zeros((Q.shape[0]))

# cooling schedule parameters
p1_int = 1000
p2_double = 0.005

# Create all the necessary arrays
coloring = np.zeros(n, dtype=np.int16)
energy_history = np.zeros(num_iter+1, dtype=np.int32)
beta_history = np.zeros(num_iter+1, dtype=np.double)

d = 1
for iq,q in enumerate(Q):
    while d < n:
        # Create the random graph
        G = nx.erdos_renyi_graph(n, d/n)
        A = np.array(nx.adjacency_matrix(G).todense(), dtype=np.int16)

        # run the damn thing
        energy = pymcmc.color_graph(A, q, num_iter, coloring, 
                energy_history, beta_history,
                p1_int, p2_double)

        if energy == 0:
            dmax[iq] = d
            d += 1
        else:
            first_fail[iq] = energy
            break


    # some feedback to user
    print 'Finished %d colors for dmax=%d' % (q, d-1)

plt.imshow(success, aspect='auto')
plt.show()
