
if __name__ == "__main__":

    import sys
    import numpy as np
    import matplotlib.pyplot as plt
    from scipy.io import loadmat, savemat
    import util

    a_file = sys.argv[1]
    sol_file = sys.argv[2]
    try:
        q_opt = int(sys.argv[3])
    except IndexError:
        q_opt = None
    matrix_var_name = sys.argv[4]
    output_var_name = sys.argv[5]

    # load adjacency matrix
    a = loadmat(a_file)
    A = np.array(a[matrix_var_name], dtype=np.int16)
    try:
        q = a['q']
    except:
        if q_opt is None:
            raise ValueError('Please specify a value for q.')
        else:
            q = q_opt

    # load the solution file
    b = loadmat(sol_file)
    X = np.array(b[output_var_name], dtype=np.int16)
    E = b['E']

    # check that the number of colors is correct
    qp = np.max(X)
    if qp != q:
      print 'Error: The solution uses %d colors instead of %d.' % (qp, q)
    else:
      print 'Ok: Number of colors is legal.'

    # Check that the energy corresponds to that of the coloring
    Ep = util.hamiltonian(A, X[:,0])
    if E == Ep:
      print 'Ok: The declared energy is that of the coloring.'
    else:
      print 'Error: The energy declared is %d but the energy of the coloring is %d.' % (E, Ep)
    
