
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
Q = np.arange(3,8)

# number of average vertices 
D = np.arange(2,25)

# number of iterations
num_iter = 200000000

# energy
energy = np.zeros((Q.size,D.size))

# cooling schedule parameters
p1_int = 1000
p2_double = 0.005
#p1_int = 100000
#p2_double = 0.00002

# Create all the necessary arrays
coloring = np.zeros(n, dtype=np.int16)
energy_history = np.zeros(num_iter+1, dtype=np.int32)
beta_history = np.zeros(num_iter+1, dtype=np.double)

for iq,q in enumerate(Q):
    for i,d in enumerate(D):
        # Create the random graph
        G = nx.erdos_renyi_graph(n, d/n)
        A = np.array(nx.adjacency_matrix(G).todense(), dtype=np.int16)

        # run the damn thing
        energy[iq,i] = pymcmc.color_graph(A, q, num_iter, coloring, 
                    energy_history, beta_history,
                    p1_int, p2_double)
        print 'Color ', q, ' d ', d, ' energy ', energy[iq,i]


plt.figure();
for iq,q in enumerate(Q):
    plt.plot(energy[iq,:])

#plt.imshow(success, aspect='auto')
plt.show()
