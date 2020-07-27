#include </usr/include/python3.6/Python.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

bool initPy() {
		
	Py_Initialize();

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\".\")");

	return 1;
}

PyObject* makeListd(double* array, size_t arr_size) {

    PyObject* list = PyList_New(arr_size);
    for (size_t list_idx = 0; list_idx < arr_size; ++list_idx) {
        PyList_SetItem(list, list_idx, PyFloat_FromDouble(array[list_idx]));
    }
    return list;
}

PyObject* makeListu(uint16_t* array, size_t arr_size)  {

    PyObject* list = PyList_New(arr_size);
    for (size_t list_idx = 0; list_idx < arr_size; ++list_idx) {
        PyList_SetItem(list, list_idx, PyLong_FromUnsignedLong((unsigned long) array[list_idx]));
    }
    return list;
}

double* makeArr(PyObject* list, size_t arr_size) {

    double* array = malloc(sizeof(double)*arr_size);
    for (size_t list_idx = 0; list_idx < arr_size; ++list_idx) {
        array[list_idx] = PyFloat_AsDouble(PyList_GetItem(list, list_idx));
    }
    return array;
}

int runPythonFunc(uint16_t num_args, char* module_name, char* func_name, PyObject** args, double** output_arr, size_t output_len) {

    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    /* Error checking of pName left out */

    pModule = PyImport_ImportModule(module_name);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule,  func_name);

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(num_args);
            for (i = 0; i < num_args; ++i) {
                pValue = args[i];
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                PyTuple_SetItem(pArgs, i, pValue);
            }

            pValue = PyObject_CallObject(pFunc, pArgs);

            *output_arr = makeArr(pValue, output_len);

            Py_DECREF(pArgs);
            if (pValue != NULL) {
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", func_name);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", module_name);
        return 1;
    }

    return 0;
}

void plot(double* x_arr, uint32_t x_arr_len, double* y_arr, uint32_t y_arr_len, char* x_label, char* y_label, char* file_name) {

	if (x_arr_len != y_arr_len){ printf("Arrays must be equal len. Exiting.\n"); return;}

	uint16_t num_args = 5;
	uint16_t output_len = 1;

	PyObject** args = malloc(sizeof(PyObject*) * num_args);
	double* output_arr = malloc(sizeof(double)); 
	
	args[0] = makeListd(x_arr, x_arr_len);
	args[1] = makeListd(y_arr, y_arr_len);
	args[2] = PyBytes_FromString(x_label);
	args[3] = PyBytes_FromString(y_label);
	args[4] = PyBytes_FromString(file_name);

	runPythonFunc(num_args, "main", "plotGraph", args, &output_arr, output_len);
}

void addLinearArray(double* array, double min, double max, uint32_t length) {

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    //
    // Adds an array of linearly increasing values between min and max to array.
    //
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

    double step_size = (max - min)/length; //<-- The increase in value between two subsequent array indices.

    for (uint32_t index = 0; index < length; index++) {
        array[index] = min + ( (double) index )*step_size;
    }
}

void addRandomArray(double* array, double min, double max, uint32_t length) {

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    //
    // Adds an array of linearly increasing values between min and max to array.
    //
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //

    for (uint32_t index = 0; index < length; index++) {
        array[index] = rand();
    }
}
 
void main() {

    uint16_t length = 100;

    bool py_int = initPy();

    if (py_int){

        double* x_arr = malloc(length*sizeof(double)); //<-- Allocates memory for new array.
        addLinearArray(x_arr, 0, length, length);

        double* y_arr = malloc(length*sizeof(double));
        addRandomArray(y_arr, 0, length, length);

        plot(x_arr, length, y_arr, length, "x", "y", "example_plot");

        free(x_arr);
        free(y_arr);

    }
    else{
        fprintf(stderr, "Python not intilizised.\n");
    }

    return 0;
}