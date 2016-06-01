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
import itertools as it
import time
import sys

import ipyparallel as ip

# custom modules here
import util
import pymcmc

def get_opt_params(schedule, d, q):
  A = np.load('tuned_parameters_schedule'+str(schedule)+'.npz')

  P = A['cartesian']
  e = A['energy']
  D = A['D']
  Q = A['Q']
  p1 = A['param1']
  p2 = A['param2']

  # find closest degree and colors for which we have tuned parameters
  i_d = np.argmin(np.abs(d - A['D']))
  i_q = np.argmin(np.abs(q - A['Q']))

  # find the indices
  I = np.where(np.logical_and(P[:,1] == D[i_d], P[:,2] == Q[i_q]))

  score = e[I].reshape((p1.shape[0], p2.shape[0]))

  m = np.argmin(e[I[0]])
  params = P[I[0][m],3:5]

  return params

def print_params(schedule, d, q):
  A = np.load('tuned_parameters_schedule'+str(schedule)+'.npz')

  D = A['D']
  Q = A['Q']
  P = A['cartesian']
  e = A['energy']
  p1 = A['param1']
  p2 = A['param2']

  # find closest degree and colors for which we have tuned parameters
  i_d = np.argmin(np.abs(d - A['D']))
  i_q = np.argmin(np.abs(q - A['Q']))

  I = np.where(np.logical_and(P[:,1] == D[i_d], P[:,2] == Q[i_q]))

  score = e[I].reshape((p1.shape[0], p2.shape[0]))

  print '===================='
  print 'schedule=%d d=%d q=%.5f' % (schedule, D[i_d], Q[i_q])

  for i in xrange(p1.shape[0]):
    print '%5d |' % p1[i],
    for j in xrange(p2.shape[0]):
      print ' %3.2f ' % score[i,j],
    print ''

  print '-----------'
  print '   ',
  for j in xrange(p2.shape[0]):
    print '%1.5f' % (p2[j]),
  print ''
  print '===================='


if __name__ == "__main__":

    # number of nodes
    N = 1000

    # average node degree
    D = [3,4,5,7,9,11,13,15,18,21,23]

    # number of colors to use
    Q = [3,5,7,9]

    # parameters to tune
    schedule = [0,3,5]
    params1 = [ [250, 500, 1000, 2000, 4000, 8000, 16000],
                [250, 500, 1000, 2000, 4000, 8000, 16000],
                [12500, 25000, 100000, 200000, 400000, 8000000], ]
    params2 = [ np.logspace(np.log2(0.0001), np.log2(2.0), num=6, base=2),
                np.logspace(np.log2(0.0001), np.log2(2.0), num=6, base=2),
                np.logspace(np.log2(0.00001), np.log2(0.01), num=6, base=2), ]

    nsched = 1 # choice of schedule to tune

    # number of iterations
    iterations = 100000000

    # number of loops for every parameters combination
    loops = 5

    c = ip.Client()
    NC = len(c.ids)

    nv = []
    dv = []
    qv = []
    num_iter_v = []
    p1v = []
    p2v = []
    schedv = []

    cartesian = [e for e in it.product([N],D,Q,params1[nsched],params2[nsched].tolist(),[schedule[nsched]],[iterations],[loops])]

    def run_mcmc(arg):
        '''
        Just a wrapper to call the coloring algorithm;
        '''

        import pymcmc
        import numpy as np
        import networkx as nx

        n = arg[0]
        d = arg[1]
        q = arg[2]
        p1 = int(arg[3])
        p2 = float(arg[4])
        schedule = arg[5]
        num_iter = arg[6]
        loops = arg[7]

        # receptacle arrays
        coloring = np.zeros((n), dtype=np.int16)
        eh = np.zeros(0, dtype=np.int32)
        bh = np.zeros(0, dtype=np.double)

        energy = 0.
        for l in xrange(loops):
            G = nx.erdos_renyi_graph(n, d/n)
            A = np.array(nx.adjacency_matrix(G).todense(), dtype=np.int16)

            energy += pymcmc.color_graph(A, q, num_iter, coloring, eh, bh, p1, p2, schedule)

        energy /= float(loops)

        return energy

    # before we start to run everything, estimate the time
    arg = (N, 23, 7, params1[nsched][-1], params2[nsched][-1], schedule[nsched], iterations, loops)
    print 'Estimating time with arg=',arg
    start = time.time()
    run_mcmc(arg)
    end = time.time()
    print 'We will run %d parameter combinations.' % (len(cartesian))
    print 'Time for one run:',end - start
    print 'Number of workers:',NC
    print 'Predicted time to terminate:', len(cartesian)*(end-start)/NC
    print 'Go? [Y/n]'
    ch = sys.stdin.read(1)
    if ch == 'n' or ch == 'N':
      sys.exit(0)


    # run many instances in parallel
    out = c[:].map_sync(run_mcmc, cartesian)

    np.savez('tuned_parameters_schedule'+str(schedule[nsched])+'.npz',
        N=N, D=D, Q=Q, schedule=schedule[nsched], 
        param1=params1[nsched], param2=params2[nsched], 
        iterations=iterations, cartesian=cartesian, energy=out)

