
import numpy as np
import matplotlib.pyplot as plt

def compute_chromatic_number(d, k=2., iterations=20):
    '''
    Compute the chromatic number of an Erdos-Renyi (n, d/n) graph,
    following a result from the following paper:

    The Two Possible Values of the Chromatic Number of a Random Graph
    Dimitris Achlioptas and Assaf Naor

    This function uses Newton method to find the smallest integer k
    such that d < 2 k log(k).
    '''

    k = d
    for t in xrange(iterations):
        k -= (k*np.log(k) - d/2.)/(np.log(k) + 1)

    return k

d_max = 1000

d = np.arange(1, d_max+1)
k_d_raw = np.array([compute_chromatic_number(i) for i in d])
k_d = np.ceil(k_d_raw)

plt.stem(d, k_d)
plt.plot(d, k_d_raw)

plt.figure()
plt.plot(d)
plt.plot(2*k_d*np.log(k_d))
plt.show()
