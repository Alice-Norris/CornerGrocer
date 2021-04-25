/**
 * Author: Alice Norris
 * Class: CS-210-T4249 Programming Languages 21EW4
 * Professor: Eric Gregori, MSCS
 * Main file, completed 04/23/2021
 *
 * The code in this file attempts to follow the Google C++ Style Guide
 *(https://google.github.io/styleguide/cppguide.html).
 * Comments are used extensively, even when code is obvious, to present an
 * understanding of the code, and for corrections or clarifications in
 * execution and concept. In accordance with the Google C++ style guide, all
 * lines have been wrapped as neatly as possible to 80 characters, and
 * all naming conventions have been followed to the best of my ability
*/
#include "python_functions.h" //include PythonFunctions file
#include "corner_grocer.h" //include CornerGrocer class definition
#include <memory> //included to support unique_ptr


int main() {
	/*Create a unique_ptr to make memory management easier. Call CornerGrocer's
	* parameterized constructor with hardcoded filenames. See corner_grocer.cpp 
	* for details of constructor.*/
	auto corner_grocer_instance = std::unique_ptr<CornerGrocer>(
	  new CornerGrocer("CS210_Project_Three_Input_File.txt", "frequency.dat"));

	//call cornerGrocerInstance's primary function, Application().
	corner_grocer_instance->Application(); 
	/*this is reached only after the user has exited the Application function. 
	* Explicitly delete produce_manager_instance to ideally free up all memory 
	* to gracefully exit.*/
	corner_grocer_instance.reset(); 
}