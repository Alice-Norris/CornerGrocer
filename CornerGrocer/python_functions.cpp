#include "python_functions.h"

/* This file represents the functions that are used to interface with the ProduceManager.py file. As such, certain values
* are hard coded and the call functions account for a limited number of arguments as necessitated by the class,
* ProduceManager, found in ProduceManager.py. Refer to it for implementation details. Py_XDECREF is throughout because it 
* takes no action if an object reference count would be NULL, so Py_Finalize doesn't freak out*/


/*This function initializes the Python interpreter and imports the ProduceManager class AS ProduceManager module,
* thus completing setup of the Python environment. No provision is made for manual setup or modification short
* of changing hardcoded values.*/
void InitPython() {
	
	/*initialize Python interpreter. This is done once to ensure persistence of ProduceManager instance.
	* calling Py_Initialize multiple times has also caused "mystery" (inconsistent) errors */
	Py_Initialize(); 

	/*build Python list consisting of one string (denoted by "[s]"), using the string 
	* "ProduceManager"*/
	PyObject* moduleList = Py_BuildValue("[s]", "ProduceManager"); 
	PyErr_Print(); //print errors, if any
	
	/*This line implements python's low-level import (__import__()). As no frame is currently running,
	* there are no globals or locals, so NULL was substituted for both. Essentially, this is:
	* "from ProduceManager import ProduceManager"(importing the class ProduceManager from the module ProduceManager)
	* */
	PyObject* module = PyImport_ImportModuleEx("ProduceManager", NULL, NULL, moduleList);
	PyErr_Print(); //print errors, if any
	
	//cleanup. These items will not be used again, and upon exit leave scope. Python's garbage collection
	//also appears to pick these up once they are XDECREF'd
	Py_XDECREF(moduleList);
	Py_XDECREF(module);
}

/*This function calls the constructor of the ProduceManager class. It then returns a PyObject pointer
* representing an instance of ProduceManager. This is taken as an argument to all CallObjectMethod templates and overloads.
* This way, we don't have to manually call up references to the ProduceManager instance for every method.*/
PyObject* CallClassConstructor(const char* inputFileName, const char* outputFileName) {
	
	/*AddModule only returns a reference to the module, and counterintuitively, does not reload it.
	* It is also a borrowed reference, and so does not need to be XDECREF'd, as it was not implicitly
	* INCREF'd */
	PyObject* module = PyImport_AddModule("ProduceManager"); 
	PyErr_Print(); //print errors, if any
	
	/*Basically, retrieve the value from ProduceManager's __dict__, using the key "ProduceManager".
	* This produces a reference to the ProduceManager class*/
	PyObject* produceManagerClass = PyObject_GetAttrString(module, "ProduceManager"); 
	PyErr_Print(); //print errors, if any
	
	/*This line creates a tuple consisting of two strings (denoted by the const char* "(ss)".
	* The values packed into the tuple are the inputFileName, and outputFileName*/
	PyObject* constructorArguments = Py_BuildValue("(ss)", inputFileName, outputFileName);
	PyErr_Print(); //print errors, if any
	
	/*This calls the ProduceManager class, which the interpreter turns into a call to __new__(), 
	* in turn producing a call to __init__, creating a new instance of ProduceManager, initializing it
	* With the passed arguments, and then returning a reference to it to produceManagerInstance
	* This reference is returned and kept alive until destroyed when FinalizePython() is called*/
	PyObject* produceManagerInstance = PyObject_CallObject(produceManagerClass, constructorArguments);
	PyErr_Print(); //print errors, if any
	
	/* Cleanup. Add_Module is once again not XDECREF'd because it only borrows a reference.
	* Naturally, produceManagerInstance is not XDECREF'd because we want it to live :) */
	Py_XDECREF(produceManagerClass);
	Py_XDECREF(constructorArguments);
	//return the ProduceManagerInstance so it can be passed to the other functions.
	return produceManagerInstance; 
}

/*This specialization of the template function takes a PyObject pointer to a ProduceManager instance, and a
* name of one of its methods. This function is meant for methods that take no arguments (technically they
* take one, "self", similar to "this" in C++, but Python handles that.) It returns one integer (type int)*/
template<> int CallObjectMethod<int>(PyObject* object, const char* methodName) {
	
	/* Calls the method methodName of our ProduceManager instance. NULL terminates the variadic template
	* used to call methods (can take an arbitrary number of C arguments). Here, NULL signals no arguments
	* A reference to the PyObject holding the result is stored in PyObject* (as one would expect it to be)*/
	PyObject* result = PyObject_CallMethod(object, methodName, NULL);
	PyErr_Print(); //print errors, if any
	
	//convert value returned from python method to a c Long, which is then implicitly cast to an int
	int returnVal = PyLong_AsLong(result); 
	
	//Cleanup. XDECREF result, will be cleaned up by Python GC once this scope gets left
	Py_XDECREF(result);
	return returnVal; //return returnVal
}

/*This specialization of the template function takes a PyObject pointer to a ProduceManager instance, and a 
* name of one of its methods. This function is meant for methods that take no arguments (technically they 
* take one, "self", similar to "this" in C++, but Python handles that.) It returns nothing (type void)*/
template<> void CallObjectMethod<void>(PyObject* object, const char* methodName) {

	/* Calls the method methodName of our ProduceManager instance. NULL terminates the variadic template
	* used to call methods (can take an arbitrary number of C arguments). Here, NULL signals no arguments.
	* Method returns nothing, so no need to store anything.*/
	PyObject_CallMethod(object, methodName, NULL);
	PyErr_Print(); //print errors, if any
}


/*This specialization of the template function takes a PyObject pointer to a ProduceManager instance, a name of
* one of its methods, and a single argument. No method in ProduceMnaager's class takes more than one
* argument, so arbitrarily long argument lists were *not* implemented. It returns nothing (type void)*/
template <> void CallObjectMethod<void>(PyObject* object, const char* methodName, const char* argument) {

	/* Call the method methodName of our ProduceManager instance. NULL terminates the variadic template,
	* in this case, after one argument is supplied. Method returns nothing, so no need to store anything.*/
	PyObject_CallMethod(object, methodName, "s", argument, NULL);
	PyErr_Print(); //print errors, if any
}

/*This specialization of the template function takes a PyObject pointer to a ProduceManager instance, a name of 
* one of its methods, and a single argument. No method in ProduceMnaager's class takes more than one  
* argument, so arbitrarily long argument lists were *not* implemented. It returns one integer (type int)*/
template <> int CallObjectMethod<int>(PyObject* object, const char* methodName, const char* argument) {

	/* Call the method methodName of our ProduceManager instance. NULL terminates the variadic template,
	* in this case, after one argument is supplied. Method returns nothing, so no need to store anything.
	* A reference to the PyObject holding the result is stored in PyObject* (as one would expect it to be)*/
	PyObject* result = PyObject_CallMethod(object, methodName, "s", argument, NULL);
	PyErr_Print(); //print errors, if any

	//convert value returned from python method to a c Long, which is then implicitly cast to an int
	int returnVal = _PyLong_AsInt(result);

	//Cleanup. XDECREF result, will be cleaned up by Python GC once this scope gets left
	Py_XDECREF(result);
	return returnVal; //return returnVal
}

/* This function explicitly flags Python's garbage collector to delete the instance of ProduceManager, thus (ideally
* freeing all memory */
void FinalizePython(PyObject* object) {

	//Call Python's garbage collector to come return our loyal ProduceManager instance to the void.
	PyObject_GC_Del(object);

	PyErr_Print(); //print errors, if any
	Py_Finalize(); //Finalize interpreter, end all threads, (ideally) release all memory. All clean!
}