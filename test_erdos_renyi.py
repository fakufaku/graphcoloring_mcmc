
from __future__ import division

import numpy as np
import matplotlib.pyplot as plt
from scipy.io import loadmat
import networkx as nx

# custom modules here
from generate_cooked_graph import *
import util
import pymcmc

# number of nodes
n = 1000 

# average node degree
d = 23

# number of colors to use
q = 7

# number of iterations
num_iter = 200000000

# cooling schedule parameters
schedule = 0
p1_int = 4000
p2_double = 0.105
#p1_int = 100000
#p2_double = 0.00002

# Create the random graph
G = nx.erdos_renyi_graph(n, d/n)
A = np.array(nx.adjacency_matrix(G).todense(), dtype=np.int16)
#A,legal_coloring = generate_cooked_graph(n,d,q)
#G = nx.from_numpy_matrix(np.matrix(A))
print 'Average vertex degree:', np.mean(A.sum(axis=1))
#print 'Cost of legal coloring:', util.hamiltonian(A, legal_coloring)


# Create all the necessary arrays
coloring = np.zeros(A.shape[0], dtype=np.int16)
if num_iter <= 320000000:
    energy_history = np.zeros(num_iter+1, dtype=np.int32)
    beta_history = np.zeros(num_iter+1, dtype=np.double)
else:
    energy_history = np.zeros(1, dtype=np.int32)
    beta_history = np.zeros(1, dtype=np.double)

# run the damn thing
energy = pymcmc.color_graph(A, q, num_iter, coloring, 
                            energy_history, beta_history,
                            p1_int, p2_double, schedule)

print 'Found energy:', energy
print 'Check Hamiltonian just in case:', util.hamiltonian(A, coloring)


if energy_history.shape[0] == num_iter+1:

    plt.figure()
    plt.plot(energy_history[::10000])
    plt.plot(beta_history[::10000])
    plt.show()
