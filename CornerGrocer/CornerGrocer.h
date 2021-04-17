#ifndef CORNER_GROCER_H
#define CORNER_GROCER_H
#include <string>

using namespace std;

class CornerGrocer {
public:
	void setInputFileName(string fileName);
	void setOutputFileName(string fileName);
	CornerGrocer(string inputFileName, string outputFileName);
	CornerGrocer();
private:
	string inputFileName;
	string outputFileName;
};
#endif