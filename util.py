#----------------------------------------------------------------------------
#"THE BEER-WARE LICENSE" (Revision 42):
#Robin, Gilles, Elie wrote this file.  As long as you retain this notice you
#can do whatever you want with this stuff. If we meet some day, and you think
#this stuff is worth it, you can buy me a beer in return.   Robin, Gilles, Elie
#----------------------------------------------------------------------------

import numpy as np

def hamiltonian(A, c):

    n = A.shape[0]
    H = 0

    for i in xrange(n):
        for j in xrange(i+1,n):
            if A[i,j] == 1 and c[i] == c[j]:
                H += 1

    return H

