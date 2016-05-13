#!/Users/scheibler/anaconda/bin/python

import numpy as np
import matplotlib.pyplot as plt

a = np.loadtxt('mcmc.txt')
time = np.arange(a.shape[0])
plt.plot(time, a[:,0], 'r')
plt.plot(time, a[:,1], 'b')
plt.legend(('beta','H'))
plt.show()
