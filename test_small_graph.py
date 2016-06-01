#----------------------------------------------------------------------------
#"THE BEER-WARE LICENSE" (Revision 42):
#Robin, Gilles, Elie wrote this file.  As long as you retain this notice you
#can do whatever you want with this stuff. If we meet some day, and you think
#this stuff is worth it, you can buy me a beer in return.   Robin, Gilles, Elie
#----------------------------------------------------------------------------

import numpy as np
import matplotlib.pyplot as plt
from scipy.io import loadmat
import util
import pymcmc

a = loadmat('matrix_full.mat')

A = np.array(a['B'], dtype=np.int16)

q = 3
num_iter = 400000000

energy_history = np.zeros(num_iter+1, dtype=np.int32)
beta_history = np.zeros(num_iter+1, dtype=np.double)
coloring = np.zeros(A.shape[0], dtype=np.int16)

# cooling schedule parameters
schedule = 0
p1_int = 2000
p2_double = 0.0005

print A.shape
print A.dtype
print coloring.shape

energy = pymcmc.color_graph(A, q, num_iter, coloring, energy_history, beta_history, p1_int, p2_double, schedule)

print energy
print util.hamiltonian(A, coloring)

plt.plot(energy_history[::100000])
plt.show()
