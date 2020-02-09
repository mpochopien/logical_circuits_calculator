/** @file main.cpp */

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <map>

#include "functions.h"
#include "structures.h"

using namespace std;

int main (int count, char * params[]) 
{
    bool fileStructure = false; //flags, checking if every parameter has been passed
    bool fileInputs = false;
    bool fileOutputs = false;

    gate * pRoot = nullptr;

    /* map of functions pointers, to be able to pass function pointer into tree node using only function name */
    map<string, gateFnPtr> gateFunc;

    gateFunc["andFunc"] = andFunc;
    gateFunc["orFunc"] = orFunc;
    gateFunc["xorFunc"] = xorFunc;
    gateFunc["nandFunc"] = nandFunc;
    gateFunc["norFunc"] = norFunc;
    gateFunc["xnorFunc"] = xnorFunc;

    int lInput = 0; //variables to store input and output numbers
    int rInput = 0;
    int output = 0;
    string output_file;
    string inputStates_file;

    item * pStates = nullptr; //list keeping input states to calculate in the circuit

    for(int i = 1; i<count; i+=2){
        string tmp(params[i]);
        string tmpNxt;
        if(params[i+1])
            tmpNxt = params[i+1];

        if(tmp == "-u" && !fileStructure){
            fileStructure = true;
            createStructure (tmpNxt, fileStructure, pRoot, gateFunc, lInput, rInput, output);
            if(fileInputs && fileStructure)
                createStatesList(inputStates_file, fileInputs, pStates, lInput, rInput);
        } else if (tmp == "-i" && !fileInputs) {
            fileInputs = true;
            inputStates_file = tmpNxt;
            if(fileStructure)
                createStatesList(inputStates_file, fileInputs, pStates, lInput, rInput);
        } else if (tmp == "-o" && !fileOutputs) {
            fileOutputs = true;
            output_file = tmpNxt;
        } else {
            cout << "ERROR: Wrong parameters!" << endl;
            break;
        }
    }

    if(fileInputs && fileOutputs && fileStructure) {
        processData(pStates, pRoot, lInput, rInput, output, output_file);
    } else {
        cout << "ERROR: One of parameters has error or wasn't pass!" << endl;
    }

    deleteTree(pRoot);
    removeList(pStates);

    return 0;
}
