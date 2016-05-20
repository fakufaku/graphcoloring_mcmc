
import numpy as np

def hamiltonian(A, c):

    n = A.shape[0]
    H = 0

    for i in xrange(n):
        for j in xrange(i+1,n):
            if A[i,j] == 1 and c[i] == c[j]:
                H += 1

    return H

