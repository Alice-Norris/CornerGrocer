#include "python_functions.h" //include PythonFunctions file
#include "corner_grocer.h" //include CornerGrocer class definition
#include <memory> //included to support unique_ptr


int main() {
	/* create a unique_ptr to make memory management easier. Call CornerGrocer's parameterized constructor with hardcoded file names.
	* See corner_grocer.cpp for details of constructor.*/
	auto cornerGrocerInstance = std::unique_ptr<CornerGrocer>(new CornerGrocer("CS210_Project_Three_Input_File.txt", "frequency.dat"));
	cornerGrocerInstance->Application(); //call cornerGrocerInstance's primary function, Application().
	/*this is reached only after the user has exited the Application function. Explicitly delete produceManagerInstance to (ideally)
	* free up all memory to gracefully exit.*/
	cornerGrocerInstance.reset(); 
}