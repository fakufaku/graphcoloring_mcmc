Random Walk Class Project
=========================

The core simulated annealing engine is written in C++.

It can compiled by calling the command `scons`, the utility from the [Software
Construction](http://www.scons.org/) suite. The suite can installed by
running `pip install scons` or `brew install scons` (on mac).

For ease of interaction with the C++ engine, a python wrapper was written around.
To build the python module run `python setup.py build_ext --inplace`.

To test run `python test_small_graph.py`.

Run for the competition
-----------------------

1. Start the cluster (<N> is the number of workers)

        ipcluster start -n <N>

2. Estimate quickly the time needed to run 1000000 iterations (replace with proper filename and value of q)

        time ipython ./run_competition.py -- <adjacency.mat> 1000000 <q>

3. Run the competition script with appropriate number of iterations

        time ipython ./run_competition.py -- <filename.mat> <num_iter> <q>

4. Pick the best solution file and quickly check the solution by running

        ipython ./check_solution.py <adjacency.mat> <coloring.mat> <q>

5. Send back the file by email to Olivier and Nicolas
