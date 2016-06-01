#----------------------------------------------------------------------------
#"THE BEER-WARE LICENSE" (Revision 42):
#Robin, Gilles, Elie wrote this file.  As long as you retain this notice you
#can do whatever you want with this stuff. If we meet some day, and you think
#this stuff is worth it, you can buy me a beer in return.   Robin, Gilles, Elie
#----------------------------------------------------------------------------

from __future__ import division

import numpy as np
import matplotlib.pyplot as plt
import networkx as nx

def generate_cooked_graph(n,d,q):
    '''
    This function generates a graph with n vertices and average degree d
    with a proper q-coloring similar to G(n,d/n).

    The method used was described in section 2.4.3 of

    OPTIMIZATION BY SIMULATED ANNEALING: AN EXPERIMENTAL EVALUATION; PART II,
    GRAPH COLORING AND NUMBER P ARTITIONING
    By DS Johnson, CR Aragon, LA McGeoch, C Schevon

    The method:
    1. Randomly assign vertices with equal probability to K color classes.
    2. For each pair {u, v} of vertices not in the same color class, place an
      edge between u and v with probability q/(qn/d-1), i.e., the probability
      required to make the average degree roughly d.
    3. Pick one vertex as a representative of each class and add any necessary
      edges to ensure that these K vertices form a clique (assuming that K is not
      too large, no class will be empty, so such representatives will exist).
    '''

    # the adjacency matrix
    A = np.zeros((n,n), dtype=np.int16)

    # 1. pick a coloring at random
    col = np.random.choice(q,size=(n))

    # 2. Add edges between vertices of different color
    for u in xrange(n):
        for v in xrange(u+1,n):

            if col[u] != col[v]:
                p = np.random.uniform()
                if p < q/(n/d*(q-1)):
                    A[u,v] = 1
                    A[v,u] = 1

    # 3. Create at least one clique
    classes = []
    for color in xrange(q):
        u = 0
        while col[u] != color:
            u += 1
        if u == n:
            raise ValueError('Not all colors were assigned! Try again.')
        else:
            classes.append(u)

    for u in xrange(q):
        for v in xrange(u+1,q):
            A[classes[u],classes[v]] = 1
            A[classes[v],classes[u]] = 1

    return (A,col)


