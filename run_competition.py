
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
    import numpy as np
    import matplotlib.pyplot as plt
    from scipy.io import loadmat, savemat
    import util
    import pymcmc
    import ipyparallel as ip
    import time
    import datetime

    from parameter_tuning import get_opt_params


    # arguments
    filename = sys.argv[1]
    num_iter = int(sys.argv[2])
    try:
        q_opt = int(sys.argv[3])
    except IndexError:
        q_opt = None


    # Extract graph and number of color from mat file
    a = loadmat(filename)
    A = np.array(a['A'], dtype=np.int16)
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

    # get the optimal parameters
    params0 = get_opt_params(0, d_avg, q)
    params3 = get_opt_params(3, d_avg, q)


    # prepare return buffers
    energy_history = np.zeros(1, dtype=np.int32)
    beta_history = np.zeros(1, dtype=np.double)
    coloring = np.zeros(A.shape[0], dtype=np.int16)

    # cooling schedule parameters
    repeat = 2
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

    # save all results in mat files
    today = datetime.date.today()
    for i,o in enumerate(out):

        # random id number
        rid = np.random.choice(100000)

        # format the file name
        out_fn = 'result_' + today.strftime('%Y%m%d') + \
            '_id_%06d_sched_%d_p1_%d_p2_%.5f_energy_%d.mat' \
            % (rid, S[i], P1[i], P2[i], int(o[0]))

        print o[0]
        print o[1]

        # a dictionary with the variables to save
        var = {'E': o[0], 'X': o[1]}

        # save to the mat file
        savemat(out_fn, var)


