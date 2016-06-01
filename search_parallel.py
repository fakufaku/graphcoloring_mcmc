#----------------------------------------------------------------------------
#"THE BEER-WARE LICENSE" (Revision 42):
#Robin, Gilles, Elie wrote this file.  As long as you retain this notice you
#can do whatever you want with this stuff. If we meet some day, and you think
#this stuff is worth it, you can buy me a beer in return.   Robin, Gilles, Elie
#----------------------------------------------------------------------------

from __future__ import division

import numpy as np
import matplotlib.pyplot as plt
from scipy.io import loadmat
import networkx as nx

import ipyparallel as ip

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
num_iter = 80000000

# Create the random graph
G = nx.erdos_renyi_graph(n, d/n)
A = np.array(nx.adjacency_matrix(G).todense(), dtype=np.int16)

c = ip.Client()
print c.ids

NC = len(c.ids)
print NC,'workers on the job'

# Create all the necessary arrays
coloring = np.zeros((A.shape[0]), dtype=np.int16)
energy_history = np.zeros(0, dtype=np.int32)
beta_history = np.zeros(0, dtype=np.double)

# this is a bit silly but we just need to repeat the arguments in a list
A_mul = [A for x in xrange(NC)]
q_mul = [q for x in xrange(NC)]
num_iter_mul = [num_iter for x in xrange(NC)]

def run_mcmc(A, q, num_iter):
    '''
    Just a wrapper to call the coloring algorithm;
    '''

    import pymcmc
    import numpy as np

    coloring = np.zeros((A.shape[0]), dtype=np.int16)
    eh = np.zeros(0, dtype=np.int32)
    bh = np.zeros(0, dtype=np.double)

    energy = pymcmc.color_graph(A, q, num_iter, coloring, eh, bh)

    return (energy, coloring)

# run many instances in parallel
out = c[:].map_sync(run_mcmc, A_mul, q_mul, num_iter_mul)

energies = np.array([t[0] for t in out])
color_vectors = [t[1] for t in out]

print energies.min()
