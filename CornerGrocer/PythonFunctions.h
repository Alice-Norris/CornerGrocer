
#ifndef PYTHON_FUNCTIONS_H
#define PYTHON_FUNCTIONS_H
#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <vector>
using namespace std;

void CallProcedure(string pName);

PyObject* CallObjectMethod(PyObject* object, string& methodName, string& argument);

PyObject* CallObjectMethod(PyObject* object, string& methodName);

PyObject* CallReturnProcedureWithArguments(string pName, vector<string> &arguments);

int callIntFunc(string proc, string param);

int callIntFunc(string proc, int param);


#endif
