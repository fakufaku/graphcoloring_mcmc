#include <iostream>
#include <cstdint>
#include <random>

#include "Python.h"
#include "numpy/arrayobject.h"

#include "mcmc.h"

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

/*
 * A python wrapper to call MCMC
 */

static PyObject*
color_graph (PyObject *dummy, PyObject *args)
{
    PyObject *arg1=NULL, *out=NULL, *ret;
    PyArrayObject *arr1=NULL, *oarr=NULL;
    int q = 5;
    int num_iter = 1000;
    int energy = -1;
    int arr1_nd, oarr_nd;
    int arr1_type, oarr_type;
    npy_intp *arr1_shape, *oarr_shape;
    int16_t *adjacency, *output_vector;
    Graph *G;
    int graph_size;
    MCMC *mcmc;

    // Initialize the random number generator (RNG)
    typedef std::chrono::high_resolution_clock myclock;
    myclock::time_point beginning = myclock::now();
    for (int i = 0 ; i < 1000 ; i++)
      ;
    myclock::duration d = myclock::now() - beginning;
    unsigned seed = d.count();
    std::default_random_engine generator(seed);

    // Parse the input arguments of the function
    if (!PyArg_ParseTuple(args, "OiiO!", &arg1, &q, &num_iter,
        &PyArray_Type, &out)) return NULL;
    cout << "Start with q=" << q << " num_iter=" << num_iter << endl;
    cout.flush();

    // First argument is the adjacency matrix of a graph
    arr1 = (PyArrayObject*)PyArray_FROM_OTF(arg1, NPY_INT16, NPY_IN_ARRAY);
    if (arr1 == NULL) 
    {
      cout << "Could not get pointer to adjacency matrix" << endl;
      goto fail;
    }

    // Second argument is the number of colors
    // It's been taken care of at this point

    // Third argument is used to store the coloring found
    oarr = (PyArrayObject*)PyArray_FROM_OTF(out, NPY_INT16, NPY_INOUT_ARRAY);
    if (oarr == NULL)
    {
      cout << "Could not get pointer to return vector." << endl;
      goto fail;
    }

    /*vv* code that makes use of arguments *vv*/

    // We run some check on input adjacency matrix
    arr1_nd = PyArray_NDIM(arr1);   // check 2 dimensional
    if (arr1_nd != 2)
    {
      cout << "Adjacency matrix not 2D" << endl;
      goto fail;
    }
    arr1_shape = PyArray_DIMS(arr1);  // npy_intp array of length nd showing length in each dim.
    if (arr1_shape[0] != arr1_shape[1]) 
    {
      cout << "Adjacency matrix not square." << endl;
      goto fail;
    }
    arr1_type = PyArray_TYPE(arr1); // check the type is int16
    cout << "Data type: " << arr1_type << endl;
    cout << "NPY_INT16=" << NPY_INT16 << endl;
    if (arr1_type != NPY_INT16) 
    {
      cout << "Adjacency matrix should be of type int16." << endl;
      goto fail;
    }

    // Now some checks on the output array
    oarr_nd = PyArray_NDIM(oarr);
    if (oarr_nd != 1) 
    {
      cout << "Output vector should be 1D." << endl;
      goto fail;
    }
    oarr_shape = PyArray_DIMS(oarr);
    if (oarr_shape[0] != arr1_shape[0]) 
    {
      cout << "Output vector should have length matching adjacency matrix." << endl;
      goto fail;
    }
    oarr_type = PyArray_TYPE(oarr); // check the type is int16
    if (oarr_type != NPY_INT16) 
    {
      cout << "Output vector type should be int16." << endl;
      goto fail;
    }

    // recover the data and graph size (finally!)
    adjacency = (int16_t *)PyArray_DATA(arr1);
    graph_size = arr1_shape[0];
    output_vector = (int16_t *)PyArray_DATA(oarr);

    // Create the graph from the adjacency matrix
    G = new Graph(graph_size, adjacency, q, generator);
    Py_INCREF(G);
    cout << "Created graph of size " << graph_size << " from adjacency matrix" << endl;
    cout.flush();

    // Run Simulated annealing
    mcmc = new  MCMC(G, q, generator);
    Py_INCREF(mcmc);
    cout << "Created MCMC" << endl;
    cout.flush();
    mcmc->run(num_iter);
    cout << "Finished running the MCMC." << endl;
    cout.flush();

    // and the energy
    energy = G->hamiltonian();
    cout << "Final energy " << energy << endl;
    cout.flush();
    
    // Store the resulting coloring in the output vector
    for (int i = 0 ; i < graph_size ; i++) 
      output_vector[i] = G->vertices[i].color;

    cout << "Stored up output in vector." << endl;
    cout.flush();

    // Clean up
    Py_DECREF(G);
    Py_DECREF(mcmc);
    
    /*^^* code that makes use of arguments *^^*/

    Py_DECREF(arr1);
    Py_DECREF(oarr);

    // return the energy
    ret = Py_BuildValue("i", energy);
    return ret;

 fail:
    cout << "Exiting with error." << endl;
    cout.flush();

    Py_XDECREF(arr1);
    PyArray_XDECREF_ERR(oarr);
    return NULL;
}

static struct PyMethodDef methods[] = {
    {"color_graph", color_graph, METH_VARARGS, "Tries to find a proper coloring by simulated annealing"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initpymcmc (void)
{
    (void)Py_InitModule("pymcmc", methods);
    import_array();
}
