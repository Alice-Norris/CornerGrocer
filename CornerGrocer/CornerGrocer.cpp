#include "CornerGrocer.h"
#include <iostream>

using namespace std;


CornerGrocer::CornerGrocer(string inputFileName, string outputFileName) {
	this->inputFileName = inputFileName;
	this->outputFileName = outputFileName;
}

CornerGrocer::CornerGrocer() {
	this->inputFileName = "unknown";
	this->outputFileName = "unknown";
}


void CornerGrocer::setInputFileName(string fileName) {
	this->inputFileName = fileName;
}
void CornerGrocer::setOutputFileName(string fileName) {
	this->outputFileName = fileName;
}