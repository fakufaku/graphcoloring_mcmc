
import numpy as np
from scipy.io import loadmat
import pymcmc

a = loadmat('graph_adjacency_matrix.mat')

A = np.array(a['A'], dtype=np.int16)
vec = np.zeros(A.shape[0], dtype=np.int16)
q = 4
num_iter = 1000000

print A.shape
print A.dtype
print vec.shape

energy = pymcmc.color_graph(A, q, num_iter, vec)

print energy
