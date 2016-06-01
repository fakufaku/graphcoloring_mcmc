#!/Users/scheibler/anaconda/bin/python

#----------------------------------------------------------------------------
#"THE BEER-WARE LICENSE" (Revision 42):
#Robin, Gilles, Elie wrote this file.  As long as you retain this notice you
#can do whatever you want with this stuff. If we meet some day, and you think
#this stuff is worth it, you can buy me a beer in return.   Robin, Gilles, Elie
#----------------------------------------------------------------------------

import numpy as np
import matplotlib.pyplot as plt

a = np.loadtxt('mcmc.txt')
time = np.arange(a.shape[0])
plt.plot(time, a[:,0], 'r')
plt.plot(time, a[:,1], 'b')
plt.legend(('beta','H'))
plt.show()
