/**
 * Author: Alice Norris
 * Class: CS-210-T4249 Programming Languages 21EW4
 * Professor: Eric Gregori, MSCS
 * Corner Grocer header file, completed 04/23/2021
 *
 * The code in this file attempts to follow the Google C++ Style Guide 
 *(https://google.github.io/styleguide/cppguide.html).
 * Comments are used extensively, even when code is obvious, to present an 
 * understanding of the code, and for corrections or clarifications in 
 * execution and concept. In accordance with the Google C++ style guide, all 
 * lines have been wrapped as neatly as possible to 80 characters, and
 * all naming conventions have been followed to the best of my ability
*/
#ifndef CORNER_GROCER_H
#define CORNER_GROCER_H
#include "python_functions.h"
#include <string>

/*This class is the "application" class so to speak- it manages the results 
* between this program and python, holds and validates data, and manages 
* input/output, except where such is handled by python */
class CornerGrocer {
public:
	/*The parameterized constructor is the only one that should be called,
	* But in case the default constructor is called, functions have been
	* provided to allow users to set the input and output filenames. Once this
	* is done you MUST call "CallClassConstructor" from python_functions.cpp
	* in order for this program to work! */
	CornerGrocer(const char* input_file_name, const char* output_file_name);
	CornerGrocer();

	/*These functions set the class' input_file_name  and output_file_name 
	* attributes. They should never be needed, as it greatly simplifies use to
	* call the parameterized constructor.*/
	void SetInputFileName(std::string fileName);
	void SetOutputFileName(std::string fileName);

	/*This function, depending on how it is called, will return either an int 
	* or a string in either case, it will use a regex to verify the data 
	* matches the return type.*/
	template<typename returnType> returnType ValidateInput(std::string prompt);

	/*This function prompts python to check its data for the quantity 
	* associated with an item name. See python_functions.cpp for implementation
	* details. This function then prints a slightly prettified text output that
	* displays the requested item and number sold that day to the user.*/
	void CheckSingleItemFrequency();
	
	//This function clears the screen upon request by printing 40 endls
	void ClearScreen();

	//This function simply displays the program menu.
	void UserMenu();

	/*This function works as the primary function of CornerGrocer instances.
	* Other than the constructor, it is the only function that has to be called
	* from elsewhere. */
	void Application();

	/*This function prints a slightly prettified histogram in text format,
	* graphically displaying the items sold that day and quantity sold.
	* takes the name of an input file as an argument. */
	void PrintFrequencyHistogram(std::string &dataFile);

private:
	//this function simply sets the filename to be used as an input
	std::string input_file_name_;
	
	//this function simply sets the filename to be used for output
	std::string output_file_name_;

	/*this object should never need to be touched directly. It pretty much 
	* handles itself, and will be cleaned up when FinalizePython is called. 
	* (This is done automatically when the user selects option 4.*/
	PyObject* produce_manager_instance_;
};
#endif