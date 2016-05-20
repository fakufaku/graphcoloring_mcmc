
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import loadmat
import util
import pymcmc

a = loadmat('graph_adjacency_matrix.mat')

A = np.array(a['A'], dtype=np.int16)
vec = np.zeros(A.shape[0], dtype=np.int16)
q = 4
num_iter = 10000000
energy_history = np.zeros(num_iter+1, dtype=np.int32)
beta_history = np.zeros(num_iter+1, dtype=np.double)

print A.shape
print A.dtype
print vec.shape

energy = pymcmc.color_graph(A, q, num_iter, vec, energy_history, beta_history)

print energy
print util.hamiltonian(A, vec)

plt.plot(energy_history)
plt.show()
