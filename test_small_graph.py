
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import loadmat
import util
import pymcmc

a = loadmat('graph_adjacency_matrix.mat')

A = np.array(a['A'], dtype=np.int16)

q = 13
num_iter = 200000000

energy_history = np.zeros(num_iter+1, dtype=np.int32)
beta_history = np.zeros(num_iter+1, dtype=np.double)
coloring = np.zeros(A.shape[0], dtype=np.int16)

# cooling schedule parameters
schedule = 0
p1_int = 1000
p2_double = 0.0005

print A.shape
print A.dtype
print coloring.shape

energy = pymcmc.color_graph(A, q, num_iter, coloring, energy_history, beta_history, p1_int, p2_double, schedule)

print energy
print util.hamiltonian(A, coloring)

plt.plot(energy_history)
plt.show()
