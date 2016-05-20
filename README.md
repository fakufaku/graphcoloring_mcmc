Random Walk Class Project
=========================

The core simulated annealing engine is written in C++.

It can compiled by calling the command `scons`, the utility from the [Software
Construction](http://www.scons.org/) suite. The suite can installed by
running `pip install scons` or `brew install scons` (on mac).

For ease of interaction with the C++ engine, a python wrapper was written around.
To build the python module run `python setup.py build_ext --inplace`.

To test run `python test_small_graph.py`.

