/**
 * Author: Alice Norris
 * Class: CS-210-T4249 Programming Languages 21EW4
 * Professor: Eric Gregori, MSCS
 * Python Functions definition file, completed 04/23/2021
 *
 * The code in this file attempts to follow the Google C++ Style Guide
 *(https://google.github.io/styleguide/cppguide.html).
 * Comments are used extensively, even when code is obvious, to present an
 * understanding of the code, and for corrections or clarifications in
 * execution and concept. In accordance with the Google C++ style guide, all
 * lines have been wrapped as neatly as possible to 80 characters, and
 * all naming conventions have been followed to the best of my ability
*/
#include "python_functions.h"

/* This file represents the functions that are used to interface with the 
* ProduceManager.py file. As such, certain values are hard coded and the call 
* functions account for a limited number of arguments as necessitated by the 
* class, ProduceManager, found in ProduceManager.py. Refer to it for 
* implementation details. Py_XDECREF is used throughout because it takes no 
* action if an object reference count would be NULL, so Py_Finalize doesn't
*  freak out*/


/*This function initializes the Python interpreter and imports the 
* ProduceManager class AS ProduceManager module, thus completing setup of the 
* Python environment. No provision is made for manual setup or modification 
* short of changing hardcoded values.*/
void InitPython() {
	
	/*initialize Python interpreter. This is done once to ensure persistence of
	* ProduceManager instance. Calling Py_Initialize multiple times has also 
	* caused "mystery" (inconsistent) errors */
	Py_Initialize(); 

	/*build Python list consisting of one string (denoted by "[s]"), using the 
	* string "ProduceManager"*/
	PyObject* module_list = Py_BuildValue("[s]", "ProduceManager"); 
	PyErr_Print(); //print errors, if any
	
	/*This line implements python's low-level import (__import__()). As no 
	* frame is currently running, there are no globals or locals, so NULL was 
	* substituted for both. Essentially, this is a "from" style Python import: 
	* "from ProduceManager import ProduceManager"
	* (importing the class ProduceManager from the module ProduceManager)*/
	PyObject* module = PyImport_ImportModuleEx("ProduceManager", NULL, NULL, 
		                                       module_list);
	PyErr_Print(); //print errors, if any
	
	/*cleanup. These items will not be used again, and upon exit leave scope. 
	* Python's garbage collection
	* also appears to pick these up once they are XDECREF'd */
	Py_XDECREF(module_list);
	Py_XDECREF(module);
}

/*This function calls the constructor of the ProduceManager class. It then 
* returns a PyObject pointer representing an instance of ProduceManager. 
* This is taken as an argument to all CallObjectMethod templates and overloads.
* This way, we don't have to manually call up references to the ProduceManager 
* instance for every method.*/
PyObject* CallClassConstructor(const char &input_file_name, 
	                           const char &output_file_name) {
	
	/*AddModule only returns a reference to the module, and counterintuitively,
	* does not reload it. It is also a borrowed reference, and so does not need
	*  to be XDECREF'd, as it was not implicitly INCREF'd */
	PyObject* module = PyImport_AddModule("ProduceManager"); 
	PyErr_Print(); //print errors, if any
	
	/*Basically, retrieve the value from ProduceManager's __dict__, using the 
	* key "ProduceManager". This produces a reference to the ProduceManager 
	* class*/
	PyObject* produce_manager_class = PyObject_GetAttrString(module, 
		                                                     "ProduceManager");
	PyErr_Print(); //print errors, if any
	
	/*This line creates a tuple consisting of two strings (denoted by the const
	* char* "(ss)". The values packed into the tuple are the input_file_name, 
	* and output_file_name*/
	PyObject* constructor_arguments = Py_BuildValue("(ss)", &input_file_name, 
		                                            &output_file_name);
	PyErr_Print(); //print errors, if any
	
	/*This calls the ProduceManager class, which the interpreter turns into a 
	* call to __new__(), in turn producing a call to __init__, creating a new 
	* instance of ProduceManager, initializing it With the passed arguments, 
	* and then returning a reference to it to produce_manager_instance. This 
	* reference is returned and kept alive until destroyed when FinalizePython
	*  is called*/
	PyObject* produce_manager_instance = PyObject_CallObject(
		produce_manager_class, 
		constructor_arguments);

	PyErr_Print(); //print errors, if any
	
	/* Cleanup. Add_Module is once again not XDECREF'd because it only borrows 
	* a reference. Naturally, produce_manager_instance is not XDECREF'd because
	*  we want it to live :) */
	Py_XDECREF(produce_manager_class);
	Py_XDECREF(constructor_arguments);
	//return the ProduceManagerInstance so it can be passed to other functions.
	return produce_manager_instance; 
}

/*This specialization of the template function takes a PyObject pointer to a 
* ProduceManager instance, and a name of one of its methods. This function is 
* meant for methods that take no arguments (technically they take one, "self", 
* similar to "this" in C++, but Python handles that.) It returns one integer.*/
template<> 
	int CallObjectMethod<int>(PyObject* object, const char &method_name){
	
	/* Calls the method method_name of our ProduceManager instance. NULL 
	* terminates the variadic template used to call methods (can take an 
	* arbitrary number of C arguments). Here, NULL signals no arguments. A 
	* reference to the PyObject holding the result is stored in PyObject* */
	PyObject* result = PyObject_CallMethod(object, &method_name, NULL);
	PyErr_Print(); //print errors, if any
	
	//convert value returned from pythonto a c Long, implicit cast to int
	int return_val = PyLong_AsLong(result); 
	
	/*Cleanup. XDECREF result, will be cleaned up by Python GC once this 
	* scope gets left*/
	Py_XDECREF(result);
	return return_val; //return return_val
}

/*This specialization of the template function takes a PyObject pointer to a 
* ProduceManager instance, and a name of one of its methods. This function is 
* meant for methods that take no arguments (technically they take one, "self",
* similar to "this" in C++, but Python handles that.) It returns nothing. */
template<> 
	void CallObjectMethod<void>(PyObject* object, const char &method_name) {

	/* Calls the method method_name of our ProduceManager instance. NULL 
	* terminates the variadic template used to call methods (can take an 
	* arbitrary number of C arguments). Here, NULL signals no arguments.
	* Method returns nothing, so no need to store anything.*/
	PyObject_CallMethod(object, &method_name, NULL);
	PyErr_Print(); //print errors, if any
}


/*This specialization of the template function takes a PyObject pointer to a 
* ProduceManager instance, a name of one of its methods, and a single argument.
*  No method in ProduceMnaager's class takes more than one argument, so 
* arbitrarily long argument lists were *not* implemented. It returns nothing*/
template <> void CallObjectMethod<void>(PyObject* object, 
	                                    const char &method_name, 
	                                    const char &argument) {

	/* Call the method method_name of our ProduceManager instance. NULL terminates
	* the variadic template, in this case, after one argument is supplied. Method 
	* returns nothing, so no need to store anything.*/
	PyObject_CallMethod(object, &method_name, "s", argument, NULL);
	PyErr_Print(); //print errors, if any
}

/*This specialization of the template function takes a PyObject pointer to a 
* ProduceManager instance, a name of one of its methods, and a single argument. 
* No method in ProduceMnaager's class takes more than one argument, so 
* arbitrarily long argument lists were *not* implemented. It returns one 
* integer (type int)*/
template <> 
int CallObjectMethod<int>(PyObject* object, const char &method_name, 
                          const char &argument) {	

	/* Call the method method_name of our ProduceManager instance. NULL 
	* terminates the variadic template, in this case, after one argument is 
	* supplied. Method returns nothing, so no need to store anything. A 
	* reference to the PyObject holding the result is stored in PyObject* 
	* (as one would expect it to be)*/
	PyObject* result = PyObject_CallMethod(object, &method_name, "s", 
		                                   &argument, NULL);

	PyErr_Print(); //print errors, if any

	/*convert value returned from python method to a c Long, which is then 
	* implicitly cast to an int */
	int return_val = _PyLong_AsInt(result);

	/* Cleanup. XDECREF result, will be cleaned up by Python GC once this scope
	* gets left*/
	Py_XDECREF(result);
	return return_val; //return return_val
}

/* This function explicitly flags Python's garbage collector to delete the 
* instance of ProduceManager, thus (ideally freeing all memory) */
void FinalizePython(PyObject* object) {

	/*Call Python's garbage collector to come return 
	* our loyal ProduceManager instance to the void. */
	PyObject_GC_Del(object);

	PyErr_Print(); //print errors, if any
	//Finalize interpreter, ideally releasing all memory. All clean!
	Py_Finalize(); 
}