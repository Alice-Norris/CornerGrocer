#ifndef PYTHON_FUNCTIONS_H
#define PYTHON_FUNCTIONS_H
#include <Python.h>

/* This file represents the functions that are used to interface with the PythonCode.py file. As such, certain values
* are hard coded and the call functions account for a limited number of arguments as necessitated by the class,
* ProduceManager, found in PythonCode.py. Refer to it for implementation details. */

/*This function initializes the Python interpreter and imports the ProduceManager class AS PythonCode,
* thus completing setup of the Python environment. No provision is made for manual setup or modification short
* of changing hardcoded values.*/
void InitPython();

/*This function calls the constructor of the ProduceManager class. It then returns a PyObject pointer
* representing an instance of ProduceManager. This is taken as an argument to all CallObjectMethod templates and overloads.
* This way, we don't have to manually call up references to the ProduceManager instance for every method.*/
PyObject* CallClassConstructor(const char* inputFileName, const char* outputFileName);

/*This template function takes a PyObject pointer to a ProduceManager instance, and a name of one of its methods.
* This function is meant for methods that take no arguments (technically they take one, "self", similar to "this"
* in C++, but Python handles that.)*/
template<typename returnType> returnType CallObjectMethod(PyObject* object, const char* methodName);

/*This template function takes a PyObject pointer to a ProduceManager instance, a name of one of its methods,
* and a single argument. No method in this class takes more than one argument, so arbitrarily long argument
* lists were *not* implemented.*/
template<typename returnType> returnType CallObjectMethod(PyObject* object, const char* methodName, const char* argument);

/* This function explicitly flags Python's garbage collector to delete the instance of ProduceManager, thus (ideally
* freeing all memory */
void FinalizePython(PyObject* object);
#endif
