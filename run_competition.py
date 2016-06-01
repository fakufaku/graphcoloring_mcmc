
def run_mcmc(arg):
    '''
    Just a wrapper to call the coloring algorithm;
    '''

    import pymcmc
    import numpy as np

    n = arg[0]
    q = arg[1]
    p1 = int(arg[2])
    p2 = float(arg[3])
    schedule = arg[4]
    num_iter = arg[5]
    A = arg[6]
    coloring = arg[7]

    # receptacle arrays
    eh = np.zeros(0, dtype=np.int32)
    bh = np.zeros(0, dtype=np.double)

    energy = pymcmc.color_graph(A, q, num_iter, coloring, eh, bh, p1, p2, schedule)

    return (energy, coloring)

if __name__ == "__main__":

    import sys
    import os
    import numpy as np
    import matplotlib.pyplot as plt
    from scipy.io import loadmat, savemat
    import util
    import pymcmc
    import ipyparallel as ip
    import time
    import datetime

    from parameter_tuning import get_opt_params, print_params


    # arguments
    filename = sys.argv[1]
    num_iter = int(sys.argv[2])
    try:
        q_opt = int(sys.argv[3])
    except IndexError:
        q_opt = None
    matrix_var_name = sys.argv[4]
    output_var_name = sys.argv[5]


    # Extract graph and number of color from mat file
    a = loadmat(filename)
    A = np.array(a[matrix_var_name], dtype=np.int16)
    try:
        q = a['q']
    except:
        if q_opt is None:
            raise ValueError('Please specify a value for q.')
        else:
            q = q_opt

    # compute average degree
    d_avg = np.mean(A.sum(axis=1))
    n = A.shape[0]

    print 'The GRAPH:'
    print 'd_avg=',d_avg
    print 'n=',n

    # get the optimal parameters
    params0 = get_opt_params(0, d_avg, q)
    params3 = get_opt_params(3, d_avg, q)

    print_params(0, d_avg, q)
    print_params(3, d_avg, q)


    # cooling schedule parameters
    repeat = 3
    schedules = [0, 3]

    S = [schedules[0]]*repeat + [schedules[1]]*repeat
    P1 = [int(params0[0])]*repeat + [int(params3[0])]*repeat
    P2 = [float(params0[1])]*repeat + [float(params3[1])]*repeat

    N = [n]*repeat*len(schedules)
    Q = [q]*repeat*len(schedules)
    I = [num_iter]*repeat*len(schedules)
    adjacencies = [A.copy()]*repeat*len(schedules)
    colorings = [np.zeros((n), dtype=np.int16)]*repeat*len(schedules)

    # zip all the arguments together
    args = zip(N, Q, P1, P2, S, I, adjacencies, colorings)

    # get ref to engines
    c = ip.Client()
    print c.ids
    NC = len(c.ids)
    print NC,'workers on the job'

    # run many instances in parallel
    out = c[:].map_sync(run_mcmc, args)

    # try to create a directory for the resulting files
    try:
        os.mkdir('./results/')
    except OSError:
        pass

    # save all results in mat files
    M = np.inf
    today = datetime.date.today()
    for i,o in enumerate(out):

        # random id number
        rid = np.random.choice(100000)

        # format the file name
        out_fn = 'results/result_' + today.strftime('%Y%m%d') + \
            '_q_%d_id_%06d_sched_%d_p1_%d_p2_%.5f_energy_%d.mat' \
            % (q, rid, S[i], P1[i], P2[i], int(o[0]))

        # a dictionary with the variables to save
        var = {'E': o[0], output_var_name: np.array([o[1]]).T}

        if o[0] < M:
          M = o[0]

        # save to the mat file
        savemat(out_fn, var)

    print 'Best energy obtained:',M


