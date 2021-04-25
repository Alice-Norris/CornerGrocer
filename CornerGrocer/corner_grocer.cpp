/**
 * Author: Alice Norris
 * Class: CS-210-T4249 Programming Languages 21EW4
 * Professor: Eric Gregori, MSCS
 * Corner Grocer definition file, completed 04/23/2021
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
#include "corner_grocer.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <regex>

/*Parameterized constructor, see corner_grocer.h for usage details*/
CornerGrocer::CornerGrocer(const char* input_file_name, 
	                       const char* output_file_name) {

	InitPython(); //call function to initialize Python, import ProduceManager

	/*Calls the constructor for the ProduceManager class in ProduceManager.py
	* The PyObject functions use character arrays as inputs, so if called with 
	* strings, be sure to convert to c-style strings with c_str()*/
	produce_manager_instance_ = CallClassConstructor(*input_file_name,
		                                              *output_file_name);

	//set string representing input file name
	input_file_name_ = input_file_name; 

	//set string representing output file name
	output_file_name_ = output_file_name; 
}

/*Default constructor, shouldn't be used, see corner_grocer.h for use details*/
CornerGrocer::CornerGrocer() {
	input_file_name_ = "unknown";//set string representing input file name
	output_file_name_ = "unknown";//set string reprenting output file name
	
	/*Calls the constructor for the ProduceManager class in ProduceManager.py. 
	* The PyObject functions use const character arrays as arguments, so 
	* strings are converted here using c_str()*/
	produce_manager_instance_ = CallClassConstructor(*input_file_name_.c_str(), 
		                                           *output_file_name_.c_str());
}
/*these functions set the filenames to be used as an input or output.
* These methods should not be routinely used, as the same attributes can be 
* set up with the parameterized constructor.*/

void CornerGrocer::SetInputFileName(std::string fileName) {
	this->input_file_name_ = fileName; //set given string as input file name
}
void CornerGrocer::SetOutputFileName(std::string fileName) {
	this->output_file_name_ = fileName; //set given string as output file name
}

/*Specialized declaration of ValidateInput for being called to return an 
* integer. Takes one string argument, prompt, which is the prompt to give to 
* the user for desired input. Then compares input to a regex, makes sure it is 
* not empty, and returns an integer. Throws exceptions and repeats if an empty 
* line or nondigit are entered.*/
template <> int CornerGrocer::ValidateInput<int>(std::string prompt) {

	//string match object to hold matches and indicate if a match was found
	std::smatch matches; 

	//set up regex to be compared against, matches any length of digits only
	std::regex input_filter("^\\d*$"); 


	std::string input = "";  //initialize empty string to hold input.

	//infinite loop, return statement will exit when valid input given
	while (true) { 
		try {
			std::cout << prompt; //print provided prompt
			std::getline(std::cin, input); //get line from cin, assign to input
			if (input == "") { //if input is empty...
				throw std::invalid_argument("No input detected!"); 
			}
			//if input matches any sequence of digits with no nondigits...
			if (regex_match(input, matches, input_filter)) { 
				return stoi(input); //...return input converted to an integer
			}
			else { //a nondigit character was found...
				throw std::invalid_argument("not a number!");
			}
		}
		catch (std::invalid_argument& error) { //catch thrown exceptions
			//print error message describing what went wrong
			std::cerr << "Invalid input: " << error.what() << std::endl; 
		}
	}
}

/*Specialized declaration of ValidateInput for being called to return a string
* Takes one string argument, prompt, which is the prompt to give to the user 
* for desired input. Then compares input to a regex, makes sure it is not 
* empty, and returns an integer. Throws exceptions and repeats if an empty line
* or non-wordcharacters are entered.*/
template <> 
std::string CornerGrocer::ValidateInput<std::string>(std::string prompt) {

	//string match object to hold matches and indicate if a match was found
	std::smatch matches;

	// set up regex, matches any string of only alphabet characters
	std::regex input_filter("^[a-zA-Z]*$"); 
	std::string input = ""; //initialize empty string to hold input.
	while (true) { //infinite loop, return statement will exit function
		try {
			std::cout << prompt; //print provided prompt
			std::getline(std::cin, input); //get line from cin
			if (input == "") { //if no input was entered ... 
				throw std::invalid_argument("No input detected!");
			}
			//if inpu tmatches any sequence of only alphabet characters...
			if (regex_match(input, matches, input_filter)) { 
				return input; //...return input
			}
			else { //...if string is non-empty but no match is found
				throw std::invalid_argument("Please enter an item name!");
			}
		} 
		catch (std::invalid_argument& error) { //catch thrown exceptions

			//print error message describing what went wrong
			std::cerr << "Invalid input: " << error.what() << std::endl; 
		}
	}
}

//Clears the screen by printing 40 endls when called
void CornerGrocer::ClearScreen() {
	for (int i = 0; i < 40; ++i) { // sets i to 0, increments i until i = 40
		std::cout << std::endl; //print an endline, scrolling the terminal by 1
	}
}

//This function simply outputs the UserMenu when the Application method loops
void CornerGrocer::UserMenu() {
	std::cout << "Welcome to the Corner Grocer Produce Manager!" << std::endl;
	std::cout << "Please choose from the following options: " << std::endl;
	std::cout << "1: View All Items With Quantity Sold Today" << std::endl;
	std::cout << "2: View Amount Sold of Specific Item" << std::endl;
	std::cout << "3: View Graph of All Items Sold Today" << std::endl;
	std::cout << "4: Exit" << std::endl;
}

/*This function works as the primary function of CornerGrocer instances.
* Other than the constructor, it is the only function that has to be caleld
* from elsewhere. */
void CornerGrocer::Application() {
	/*Calls ProduceManager's totalProduceFrequencies function, which sets up a 
	* Python dictionary in the ProduceManager instance, making the data 
	* persistent across all method calls in Python. Program must be reloaded if
	*  input file  is changed, as the input data is only processed once. */
	CallObjectMethod<void>(produce_manager_instance_, 
		                   *"total_produce_frequencies");
	do { //while condition is "true"- infinite loop until a value is returned
		UserMenu();
		try {

			//assign the integer returned by ValidateInput to choice
			int choice = ValidateInput<int>("Please choose an option number:");
			if (choice > 4 || choice < 1) { //check if choice is a valid option
				throw(std::invalid_argument("Input error:"));
			}
			else if (choice == 1) { //if choice is 1...
				/*...Call python_functions' CallObjectMethod, which in turn 
				* calls ProduceManager's "printAllProduceFrequencies" method
				* See ProduceManager.py for details, essentially prints all 
				* item names and quantities sold*/
				CallObjectMethod<void>(produce_manager_instance_, 
					                   *"print_all_produce_frequencies");
				ClearScreen();
			}
			else if (choice == 2) {//if choice is 2...
				/*see CheckSingleItemFrequency for details, prints a prettified 
				*version of requested item qty sold*/
				CheckSingleItemFrequency();
			}
			else if (choice == 3) { //if choice is 3...

				/*...Call python_functions' CallObjectMethod, which in turn 
				* calls the instance of ProduceManager's "writeFrequencyFile" 
				* method. See ProduceManager.py for details, but essentially 
				* writes item names and item quantities from input file to an 
				* output file*/
				CallObjectMethod<void>(produce_manager_instance_, 
					                   *"write_frequency_file");

				/*This function prints a slightly prettified histogram in text 
				* format, graphically displaying the items sold that day and 
				* quantity sold. uses file produced by above Method call to 
				* ProduceManager */
				PrintFrequencyHistogram(output_file_name_);
			}
			else if (choice == 4) {
				/*Call FinalizePython, pass ProduceManager instance, clean up*/
				FinalizePython(produce_manager_instance_);
				//Exit Program
				return;
			}
		}
		/*catch exception thrown if user input is not a valid choice, even 
		* though it is an integer */
		catch (std::invalid_argument& error) { 
			std::cerr << error.what() << std::endl; //tell user what happened
			std::cerr << 
				"Invalid option chosen. Please choose a listed option." 
				<< std::endl; //tell user what's expected
		}
	} while (true); //infinite loop intentional, return statement breaks loop
}

/*This function prompts python to check its data for the quantity associated 
* with an item name. See python_functions.cpp for implementation details. This 
* function then prints a slightly prettified text output that displays the 
* requested item and number sold that day to the user. */
void CornerGrocer::CheckSingleItemFrequency() {
	while (true) { //infinite loop, function exited by return statements
		ClearScreen(); //clear screen

		/*Call ValidateInput to set string item_name to a valid string 
		* consisting of alphabetic characters*/
		std::string item_name = ValidateInput<std::string>
			("Please enter an item to see quantity sold ('quit' to quit): ");
		
		/*convert first letter of item_name to uppercase, so it matches the 
		* file. DOes nothing if already uppercase.
		item_name[0] = std::toupper(item_name[0]); 

		/*call ProduceManager instance's returnProduceFrequency with item name 
		* converted to c-style string. Store resulting integer in item_qty */
		int item_qty = CallObjectMethod<int>(produce_manager_instance_,
			                                 *"return_produce_frequency", 
			                                 *item_name.c_str());

		//method returns -1 if no match is found, if that's not the case...
		if (item_qty != -1) {
			/*...use the following cout statements to print a prettified 
			* version of the provided item_name and its returned quantity 
			* justifying output as necessary, and setting field widths. */
			std::cout << " ======================= " << std::endl
				<< "|" << std::left << std::setw(12) << item_name << "\t"
				<< std::right << std::setw(8) << item_qty << "|" << std::endl
				<< " ----------------------- " << std::endl;

			std::cout << "Press Enter to continue..."; //wait for input
			std::cin.get(); //get whatever the user typed on the buffer
			std::cin.ignore(0); //discard EOF
			ClearScreen(); //clearscreen
			return;
		} 
		//if user enters 'quit' or 'Quit', (string capitalized by function) ...
		else if (item_name == "Quit") {
			ClearScreen(); //...clear screen
			return; //exit function
		}
		else { //if item_qty is -1 (match not found)...
			//output item_name and message
			std::cout << "\"" << item_name << "\" not found in today's sales!" 
				<< std::endl;
			std::cout << "Press Enter to continue..."; //pause for input
			std::cin.get(); //get whatever the user typed on the buffer
			std::cin.ignore(0); //discard EOF
		}
	}
}

/*This function prints a slightly prettified histogram in text format,
* graphically displaying the items sold that day and quantity sold.
* takes the name of an input file as an argument. */
void CornerGrocer::PrintFrequencyHistogram(std::string &dataFile) {
	std::ifstream fileReader; //create ifstream to read in from file

	//set exception mask, error thrown if badbit caught (file missing, etc.)
	fileReader.exceptions(std::ifstream::badbit); 
	try { 
		//open file with the name of argument, setting fileReader to "in" mode
		fileReader.open(dataFile, std::ifstream::in); 
		if (fileReader.is_open()) { //if  file is open...
			int largest_qty = 0;  //highest qty found (used to print chart)
			int longest_string = 0; //longest name length (used to print chart)
			
			//setting up a string vector to hold all item names
			std::vector<std::string> item_names; 
			
			//setting up an integer vector to hold all quantities
			std::vector<int> item_qtys; 

			//read from file until getline returns an EOF character
			for (std::string line; std::getline(fileReader, line);) { 

				/*find the position of the first space in each line. As the 
				* output file is written with the itemname separated by a 
				* single space, this gives the itemname before that position,
				*  and the item qty after it */
				int space_pos = line.find(" ", 0); 
				
				/*find the item_name, which is the substring from the 0 (first)
				* position to the position of the space*/
				std::string item_name = line.substr(0, space_pos); 
				
				/*find the item_qty, which is the substring from the space 
				* position to end of line (converted to int by stoi()) */
				int item_qty = stoi(line.substr(space_pos, std::string::npos)); 
				item_names.push_back(item_name); //add item_name to vector
				item_qtys.push_back(item_qty); //add item_qty to vector

				//replaces largest_qty with item_qty if item_qty is larger
				if (item_qty > largest_qty) { largest_qty = item_qty; } 
				
				/*since the position of the space indicates the length of each 
				* itemname, if space position is greater than the current 
				* longest string, assign longest string the value of the space 
				* character's position */
				if (space_pos > longest_string) { longest_string = space_pos; }
			}
			
			/*used to determine how long (wide) the header row should be. Five 
			* added to account for spaces */
			size_t chart_width = longest_string + largest_qty + 5; 

			/*make header a string filled with a number of "=" characters 
			*equivalent to chart_width */
			std::string header(chart_width, '='); 
			ClearScreen(); //clear screen
			std::cout << " " << header << " " << std::endl;//print chart header

			/*loop until i is the same as the size of item_names, i.e., 
			* until the vector of item_names has been gone through, 
			* printing a line of the chart until complete*/
			for (int i = 0; i < item_names.size(); i++) { 
				/*print edge of chart, right justify the current item_name from
				*  the vector, set the fieldwidth to the width of the longest 
				* itemname, print item name, print separator*/
				std::cout << "| "  << std::right << std::setw(longest_string) 
					<< item_names[i] << " | "; 
				/*create a string of '*' characters equal to the number of 
				* current item_qty from integer vector, right justify, set 
				* width to highest item_qty in vector. print endline*/
				std::cout << std::left << std::setw(largest_qty) 
					<< std::string(item_qtys[i], '*') << " |" << std::endl;
			}
			//print chart header again, to close off the chart
			std::cout << " " << header << " " << std::endl;
		} 
		std::cout << "Press Enter to continue..."; //pause for user input
		std::cin.get(); //get whatever the user typed on the buffer
		std::cin.ignore(0); //discard EOF
		ClearScreen(); //clear screen
	}

	//if file can't be opened (ifstream's badbit gets set)...
	catch (std::ifstream::failure fileError) { 
		/*...print an error. This function does not loop and simply exits, 
		* asthere is no action that can be taken to fix the file in situ*/
		std::cerr << "Error opening File!"; 
	}
}