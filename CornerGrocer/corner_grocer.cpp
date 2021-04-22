#include "python_functions.h"
#include "corner_grocer.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <regex>

/*Parameterized constructor, see corner_grocer.h for usage details*/
CornerGrocer::CornerGrocer(const char* inputFileName, const char* outputFileName) {
	InitPython(); //call function to initialize Python and import ProduceManager module

	/*Calls the constructor for the ProduceManager class in ProduceManager.py
	* The PyObject functions use character arrays as inputs, so if called with strings,
	* be sure to convert to c-style strings with c_str()*/
	this->produceManagerInstance = CallClassConstructor(inputFileName, outputFileName);
	this->inputFileName = inputFileName; //set string representing input file name
	this->outputFileName = outputFileName; //set string representing output file name
}

/*Default constructor, should not be used, see corner_grocer.h for usage details*/
CornerGrocer::CornerGrocer() {
	this->inputFileName = "unknown"; //set string representing input file name
	this->outputFileName = "unknown"; //set string reprenting output file name
	
	/*Calls the constructor for the ProduceManager class in ProduceManager.py. 
	* The PyObject functions use const character arrays as arguments, so strings are converted
	* here using c_str()*/
	this->produceManagerInstance = CallClassConstructor(this->inputFileName.c_str(), this->outputFileName.c_str());
}
/*these functions set the filenames to be used as an input or output.
* These methods should not be routinely used, as the same attributes can be 
* set up with the parameterized constructor.*/

void CornerGrocer::setInputFileName(std::string fileName) {
	this->inputFileName = fileName; //set given string as input file name
}
void CornerGrocer::setOutputFileName(std::string fileName) {
	this->outputFileName = fileName; //set given string as output file name
}

/*Specialized declaration of validateInput for being called to return an integer
* Takes one string argument, prompt, which is the prompt to give to the user for 
* desired input. Then compares input to a regex, makes sure it is not empty, and 
* returns an integer. Throws exceptions and repeats if an empty line or nondigit
* are entered.*/
template <> int CornerGrocer::validateInput<int>(std::string prompt) {
	std::smatch matches; //create string match object to hold matches and indicate if a match was found
	std::regex inputFilter("^\\d*$"); //set up regex to be compared against, matches on any length of digits with no nondigits
	std::string input = "";  //initialize empty string to hold input.
	while (true) { //while condition set to true, return statement will exit function when valid input given
		try {
			std::cout << prompt; //print provided prompt
			std::getline(std::cin, input); //get line from cin, enables checking if no input was given
			if (input == "") { //if input is empty...
				throw std::invalid_argument("No input detected!"); //...throw an exception
			}
			//if a string matches, that is, any sequence of digits with no nondigits is entered...
			if (regex_match(input, matches, inputFilter)) { 
				return stoi(input); //...return input converted to an integer
			}
			else { //else branch means the string was not empty but a nondigit character was found...
				throw std::invalid_argument("not a number!");  //...so throw an exception
			}
		}
		catch (std::invalid_argument& error) { //catch previously thrown exceptions
			std::cerr << "Invalid input: " << error.what() << std::endl; //print error message describing what went wrong
		}
	}
}

/*Specialized declaration of validateInput for being called to return a string
* Takes one string argument, prompt, which is the prompt to give to the user for
* desired input. Then compares input to a regex, makes sure it is not empty, and
* returns an integer. Throws exceptions and repeats if an empty line or non-word
* characters are entered.*/
template <> std::string CornerGrocer::validateInput<std::string>(std::string prompt) {
	std::smatch matches;//create string match object to hold matches and indicate if a match was found
	std::regex inputFilter("^[a-zA-Z]*$"); // set up regex to be compared against, matches on any length input of only alphabet characters
	std::string input = ""; //initialize empty string to hold input.
	while (true) { //while condition is true, return statement will exit function
		try {
			std::cout << prompt; //print provided prompt
			std::getline(std::cin, input); //get line from cin, enables checking if no input was given
			if (input == "") { //if no input was entered ... 
				throw std::invalid_argument("No input detected!"); //...throw exception
			}
			//if a string matches, that is, any sequence of only alphabet characters is entered...
			if (regex_match(input, matches, inputFilter)) { 
				return input; //...return input
			}
			else { //...if string is non-empty but no match is found
				throw std::invalid_argument("Please enter an item name!"); //...throw exception
			}
		} 
		catch (std::invalid_argument& error) { //catch previously thrown exceptions
			std::cerr << "Invalid input: " << error.what() << std::endl; //print error message describing what went wrong
		}
	}
}

//Clears the screen by printing 40 endls when called
void CornerGrocer::clearScreen() {
	for (int i = 0; i < 40; ++i) { // sets i to 0, increments i until i = 40
		std::cout << std::endl; //print an endline, scrolling the terminal by 1
	}
}

//This function simply outputs the userMenu when the Application method loops
void CornerGrocer::userMenu() {
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
	* Python dictionary in the ProduceManager instance, making the data persistent
	* across all method calls in Python. Program must be reloaded if input file is changed,
	* as the input data is only processed once. */
	CallObjectMethod<void>(produceManagerInstance, "totalProduceFrequencies");
	do { //while condition is "true"- infinite loop until a value is returned
		userMenu();
		try { //throws an invalid_argument exception if a choice less than 1 or greater than 3 is given.
			int choice = validateInput<int>("Please choose an option (1, 2, 3, or 4): "); //assign choice the integer returned by validateInput
			if (choice > 4 || choice < 1) { //make sure choice is valid. If it is outside the range of 1-3...
				throw(std::invalid_argument("Input error:")); //...throw an exception
			}
			else if (choice == 1) { //if choice is 1...
				/*...Call python_functions' CallObjectMethod, which in turn calls the instance of ProduceManager's "printAllProduceFrequencies" method
				* See ProduceManager.py for details, essentially prints all item names and quantities sold*/
				CallObjectMethod<void>(this->produceManagerInstance, "printAllProduceFrequencies");
			}
			else if (choice == 2) {//if choice is 2...
				//see checkSingleItemFrequency for details, prints a prettified version of requested item qty sold
				checkSingleItemFrequency();
			}
			else if (choice == 3) { //if choice is 3...

				/*...Call python_functions' CallObjectMethod, which in turn calls the instance of ProduceManager's "writeFrequencyFile" method
				* See ProduceManager.py for details, but essentially writes item names and item quantities from input file to an output file*/
				CallObjectMethod<void>(this->produceManagerInstance, "writeFrequencyFile");

				/*This function prints a slightly prettified histogram in text format,
				* graphically displaying the items sold that day and quantity sold.
				* uses file produced by above Method call to ProduceManager*/
				printFrequencyHistogram(outputFileName);
			}
			else if (choice == 4) {
				/*Call FinalizePython, pass in ProduceManager instance, clean up*/
				FinalizePython(produceManagerInstance);
				//Exit Program
				return;
			}
		}
		catch (std::invalid_argument& error) { //catch exception thrown if user input is not a valid choice, even though it is an integer
			std::cerr << error.what() << std::endl; //tell the user what happened
			std::cerr << "Invalid option chosen. Please choose a listed option. (1, 2, or 3)" << std::endl; //tell them what's expected
		}
	} while (true); //infinite loop intentional, return statement breaks loop
}

/*This function prompts python to check its data for the quantity associated with an
* item name. See python_functions.cpp for implementation details. This function then prints
* a slightly prettified text output that displays the requested item and number sold that day
* to the user. */
void CornerGrocer::checkSingleItemFrequency() {
	while (true) { //While condition is true, function exited by return statements
		clearScreen(); //clear screen

		//Call ValidateInput to set string itemName to a valid string consisting of alphabetic characters
		std::string itemName = validateInput<std::string>("Please enter the name of an item to see quantity sold today ('quit' to quit): ");
		itemName[0] = std::toupper(itemName[0]); //convert first letter of itemName to uppercase, so it matches the file. DOes nothing if already uppercase.

		//call ProduceManager instance's returnProduceFrequency with item name converted to c-style string. Store resulting integer in itemQty
		int itemQty = CallObjectMethod<int>(this->produceManagerInstance, "returnProduceFrequency", itemName.c_str());

		//returnProduceFrequency method returns -1 if no match to itemName is found, so if that's not the case...
		if (itemQty != -1) {
			//...use the following cout statements to print a prettified version of the provided itemName and its returned quantity
			//justifying output as necessary, and setting field widths.
			std::cout << " ======================= " << std::endl;
			std::cout << "|" << std::left << std::setw(12) << itemName << "\t";
			std::cout << std::right << std::setw(8) << itemQty << "|" << std::endl;
			std::cout << " ----------------------- " << std::endl;
			std::cout << "Press Enter to continue..."; //pause program, wait for user input
			std::cin.get(); //get whatever the user typed on the buffer
			std::cin.ignore(0); //discard EOF
			clearScreen(); //clearscreen
			return;
		} 
		//if user enters 'quit' or 'Quit', (string capitalized by function) ...
		else if (itemName == "Quit") {
			clearScreen(); //...clear screen
			return; //exit function
		}
		else { //if itemQty is -1 (returned by returnProduceFrequency if match not found)...
			//output itemName and message
			std::cout << "\"" << itemName << "\" not found in today's sales!" << std::endl;
			std::cout << "Press Enter to continue..."; //pause for input
			std::cin.get(); //get whatever the user typed on the buffer
			std::cin.ignore(0); //discard EOF
		}
	}
}

/*This function prints a slightly prettified histogram in text format,
* graphically displaying the items sold that day and quantity sold.
* takes the name of an input file as an argument. */
void CornerGrocer::printFrequencyHistogram(std::string dataFile) {
	std::ifstream fileReader; //create ifstream to read in from file
	fileReader.exceptions(std::ifstream::badbit); //set exception mask, error thrown if badbit caught (issue opening file, file missing, etc.)
	try { 
		fileReader.open(dataFile, std::ifstream::in); //open file with the name of argument, setting fileReader to "in" mode
		if (fileReader.is_open()) { //if  file is open...
			int largestQty = 0;  //integer used to hold highest quantity give (used when printing chart)
			int longestString = 0; //integer used to hold length of longest itemName (used when printing chart)
			std::vector<std::string> itemNames; //setting up a string vector to hold all item names
			std::vector<int> itemQtys; //setting up an integer vector to hold all quantities

			for (std::string line; std::getline(fileReader, line);) { //read from file until getline returns an EOF character
				//find the position of the first space in each line. As the output file is written with the itemname separated by a single space, this gives
				//the itemname before that position, and the item qty after it
				int spacePos = line.find(" ", 0); 
				std::string itemName = line.substr(0, spacePos); //find the itemName, which is the substring from the 0 (first) position to the position of the space
				//find the itemQty, which is the substring from the space position to end of line (converted to int by stoi())
				int itemQty = stoi(line.substr(spacePos, std::string::npos)); 
				itemNames.push_back(itemName); //add itemName to end of name vector
				itemQtys.push_back(itemQty); //add itemQty to end of qty vector
				if (itemQty > largestQty) { largestQty = itemQty; } //inlined if, replaces largestQty with the itemQty if itemQty is larger
				//since the position of the space indicates the length of each itemname, if space position is greater than the current longest string, 
				//assign longest string the value of the space character's position
				if (spacePos > longestString) { longestString = spacePos; }
			}
			
			size_t chartWidth = longestString + largestQty + 5; //used to determine how long (wide) the header row should be. Five added to account for spaces
			std::string header(chartWidth, '='); //make header a string filled with a number of "=" characters equivalent to chartWidth
			clearScreen(); //clear screen
			std::cout << " " << header << " " << std::endl; //print chart header
			//loop until i is the same as the size of itemNames, i.e., until the vector of itemNames has been gone through, 
			//printing a line of the chart until complete
			for (int i = 0; i < itemNames.size(); i++) { 
				//print edge of chart, right justify the current itemName from the vector, set the fieldwidth to the width of the longest
				//itemname, print item name, print separator
				std::cout << "| "  << std::right << std::setw(longestString) << itemNames[i] << " | "; 
				//create a string of '*' characters equal to the number of current itemQty from integer vector, right justify, set width
				//to highest itemQty in vector. print endline
				std::cout << std::left << std::setw(largestQty) << std::string(itemQtys[i], '*') << " |" << std::endl;
			}
			//print chart header again, to close off the chart
			std::cout << " " << header << " " << std::endl;
		} 
		std::cout << "Press Enter to continue..."; //pause for user input
		std::cin.get(); //get whatever the user typed on the buffer
		std::cin.ignore(0); //discard EOF
		clearScreen(); //clear screen
	}
	catch (std::ifstream::failure fileError) { //if file can't be opened (ifstream's badbit gets set)...
		//...print an error. This function does not loop and simply exits, because there is no action that can be taken to fix the file in situ
		std::cerr << "Error opening File!"; 
	}
}