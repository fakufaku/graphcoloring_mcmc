Random Walk Class Project
=========================

This code tries to find a proper coloring of a graph using the Monte-Carlo
Markov Chain method (simulated annealing).  It was developed for the [Random
Walks](http://ipg.epfl.ch/~leveque/Random_Walks/) class taught at
[EPFL](http://www.epfl.ch) in Spring 2016 by Olivier Lévêque et Nicolas Macris.

The core simulated annealing engine is written in C++. A python wrapper was
created to make it easier to interact with the code.

Authors
-------

Robin Scheibler, Gilles Baechler, Elie Najm

Install
-------

The code requires a C++ compiler compatible with C++11. 
To build the python module run 

        `python setup.py build_ext --inplace`.

The python code uses the modules `numpy`, `scipy`, `matplotlib`, `networkx`, and `ipyparallel`.

If only the C++ part is needed, it can compiled using the `scons` ustility from
the [Software Construction](http://www.scons.org/) suite. Install it with

        pip install scons

or (on Mac OS X)

        brew install scons

Compile the code by typing

        scons 
        
To test run `python test_small_graph.py`.

Run the code
------------

1. Start the cluster (N is the number of workers)

        ipcluster start -n <N>

2. Estimate quickly the time needed to run 1000000 iterations (replace with
   proper filename and value of q, matrix_var_name and output_var_name
   correspond to variable names in the mat files)

        time ipython ./run_competition.py -- <adjacency.mat> 1000000 <q> <matrix_var_name> <output_var_name>

3. Run the competition script with appropriate number of iterations

        time ipython ./run_competition.py -- <filename.mat> <num_iter> <q> <matrix_var_name> <output_var_name>

4. Pick the best solution file (in `./results/` directory) and quickly check the solution by running

        ipython ./check_solution.py <adjacency.mat> <coloring.mat> <q> <matrix_var_name> <output_var_name>

5. Send back the file by email to Olivier and Nicolas

License
-------

        /*
         * ----------------------------------------------------------------------------
         * "THE BEER-WARE LICENSE" (Revision 42):
         * Robin, Gilles, Elie wrote this code.  As long as you retain this notice you
         * can do whatever you want with this stuff. If we meet some day, and you think
         * this stuff is worth it, you can buy me a beer in return.   Robin, Gilles, Elie
         * ----------------------------------------------------------------------------
         */
