
from __future__ import division

import numpy as np
import matplotlib.pyplot as plt
from scipy.io import loadmat
import networkx as nx

# custom modules here
import util
import pymcmc

# number of nodes
n = 100 

# average node degree
d = 23

# number of colors to use
q = 7

# number of iterations
num_iter = 160000000

# Create the random graph
G = nx.erdos_renyi_graph(n, d/n)
A = np.array(nx.adjacency_matrix(G).todense(), dtype=np.int16)

# Create all the necessary arrays
coloring = np.zeros(A.shape[0], dtype=np.int16)
energy_history = np.zeros(num_iter+1, dtype=np.int32)
beta_history = np.zeros(num_iter+1, dtype=np.double)

# run the damn thing
energy = pymcmc.color_graph(A, q, num_iter, coloring, energy_history, beta_history)

print 'Found energy:', energy
print 'Check Hamiltonian just in case:', util.hamiltonian(A, coloring)


plt.figure()
plt.plot(energy_history[::10000])
plt.plot(beta_history[::10000])
plt.show()
