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
    PyObject *arg1=NULL, *out=NULL, *out2=NULL, *out3=NULL, *ret=NULL;
    PyArrayObject *arr1=NULL, *oarr=NULL, *barr=NULL, *earr=NULL;
    int q = 5;
    int num_iter = 1000;
    int energy = -1;
    int arr1_nd, oarr_nd, barr_nd, earr_nd;
    npy_intp *arr1_shape, *oarr_shape, *barr_shape, *earr_shape;
    int16_t *adjacency = NULL, *output_vector = NULL;
    double *beta_history = NULL;
    int32_t *energy_history = NULL;
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
    if (!PyArg_ParseTuple(args, "OiiO!O!O!", &arg1, &q, &num_iter,
        &PyArray_Type, &out, &PyArray_Type, &out2, &PyArray_Type, &out3)) return NULL;

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
    
    // Fourth argument is used to store the history of energy
    earr = (PyArrayObject*)PyArray_FROM_OTF(out2, NPY_INT32, NPY_INOUT_ARRAY);
    if (earr == NULL)
    {
      cout << "Could not get pointer to return cost vector." << endl;
      goto fail;
    }
    
    // Fifth argument is used to store the history of beta
    barr = (PyArrayObject*)PyArray_FROM_OTF(out3, NPY_DOUBLE, NPY_INOUT_ARRAY);
    if (barr == NULL)
    {
      cout << "Could not get pointer to return cost vector." << endl;
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

    // Now some checks on the energy_history vector
    earr_nd = PyArray_NDIM(earr);
    if (earr_nd != 1)
    {
      cout << "Cost output vector should be 1D." << endl;
      goto fail;
    }
    earr_shape = PyArray_DIMS(earr);
    if (earr_shape[0] == num_iter+1)
      energy_history = (int32_t *)PyArray_DATA(earr);

    // Now some checks on the beta_history vector
    barr_nd = PyArray_NDIM(barr);
    if (barr_nd != 1)
    {
      cout << "Cost output vector should be 1D." << endl;
      goto fail;
    }
    barr_shape = PyArray_DIMS(barr);
    if (barr_shape[0] == num_iter+1)
      beta_history = (double *)PyArray_DATA(barr);

    // recover the data and graph size (finally!)
    adjacency = (int16_t *)PyArray_DATA(arr1);
    graph_size = arr1_shape[0];
    output_vector = (int16_t *)PyArray_DATA(oarr);

    // Create the graph from the adjacency matrix
    G = new Graph(graph_size, adjacency, q, generator);

    // Run Simulated annealing
    mcmc = new  MCMC(G, q, generator, energy_history, beta_history);
    mcmc->run(num_iter);

    // and the energy
    energy = G->hamiltonian();
    
    // Store the resulting coloring in the output vector
    for (int i = 0 ; i < graph_size ; i++) 
      output_vector[i] = G->vertices[i].color;

    // Clean up
    Py_DECREF(G);
    Py_DECREF(mcmc);
    
    /*^^* code that makes use of arguments *^^*/

    Py_DECREF(arr1);
    Py_DECREF(oarr);
    Py_DECREF(barr);
    Py_DECREF(earr);

    // return the energy
    ret = Py_BuildValue("i", energy);
    return ret;

 fail:
    cout << "Exiting with error." << endl;
    cout.flush();

    Py_XDECREF(arr1);
    PyArray_XDECREF_ERR(oarr);
    PyArray_XDECREF_ERR(barr);
    PyArray_XDECREF_ERR(earr);
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
