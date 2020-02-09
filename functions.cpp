/** @file functions.cpp */

#include <cctype>
#include <algorithm>
#include <string>
#include <regex>
#include <iostream>
#include <fstream>

#include "structures.h"
#include "functions.h"

using namespace std;

bool andFunc (const bool input1, const bool input2) {
    return input1 and input2;
}

bool orFunc (const bool input1, const bool input2) {
    return input1 or input2;
}

bool xorFunc (const bool input1, const bool input2) {
    return input1 ^ input2;
}

bool notFunc (const bool input1, const bool input2) {
    return !input1;
}

bool nandFunc (const bool input1, const bool input2) {
    return !(input1 and input2);
}

bool norFunc (const bool input1, const bool input2) {
    return !(input1 or input2);
}

bool xnorFunc (const bool input1, const bool input2) {
    return !(input1 ^ input2);
}

gate * find (gate * pR, const int number) {
    while (pR)
    {
        if (pR->number == number)
            return pR;
        if (number < pR->number)
            pR = pR->pLeft;
        else
            pR = pR->pRight;
    }
    return pR;
}

gate * addToTreeTemp (gate *& pR, const int number) {
    if (!pR) {
        pR = new gate { number, 0, 0, nullptr, nullptr, nullptr, nullptr };
        return pR;
    } else {
        auto p = pR;

        while ( ( number < p->number &&  p->pLeft ) || ( number >= p->number && p->pRight) ) {
            if (number < p->number)
                p = p->pLeft;
            else
                p = p->pRight;
        }
        if (number < p->number){
            p->pLeft = new gate { number, 0, 0, nullptr, nullptr, nullptr, nullptr };
            return p->pLeft;
        } else {
            p->pRight = new gate{number, 0, 0, nullptr, nullptr, nullptr, nullptr};
            return p->pRight;
        }
    }
}

void addToTree (gate *& pR, const int number, gate * pLInput, const int lInputTmp, gate * pRInput, const int rInputTmp, bool (*operation)(bool, bool)) {
    auto pExists = find(pR, number);
    if(!pLInput){
        pLInput = addToTreeTemp(pR, lInputTmp);
    }

    if(!pRInput && operation!=notFunc){
        pRInput = addToTreeTemp(pR, rInputTmp);
    }

    if(pExists){
        pExists->pLInput = pLInput;
        pExists->pRInput = pRInput;
        pExists->operation = operation;
    } else {
        auto p = pR;

        while ( ( number < p->number &&  p->pLeft ) || ( number >= p->number && p->pRight) ) {
            if (number < p->number)
                p = p->pLeft;
            else
                p = p->pRight;
        }
        if (number < p->number)
            p->pLeft = new gate { number, 0, 0,  pLInput, pRInput, nullptr, nullptr, operation };
        else
            p->pRight = new gate { number, 0, 0, pLInput, pRInput, nullptr, nullptr, operation };
    }
}

void deleteTree (gate *& pR) {
    if (pR)
    {
        deleteTree(pR->pLeft);
        deleteTree(pR->pRight);
        delete pR;
        pR = nullptr;
    }
}

bool validGateName (string & gateName) {
    for(char & i : gateName)
        i = tolower(i);
    return gateName == "and" || gateName == "or" || gateName == "xor" || gateName == "not" || gateName == "nand" ||
           gateName == "nor" || gateName == "xnor";
}

void addToList(item * & pH, const int lInput, const int rInput) {
    if (!pH)
        pH = new item { lInput, rInput, nullptr };
    else {
        auto p = pH;

        while ( p->pNext )
            p = p->pNext;

        p->pNext = new item { lInput, rInput, nullptr };
    }
}

void removeList(item * & pH) {
    while ( pH ) {
        auto pSucc = pH->pNext;
        delete pH;
        pH = pSucc;
    }
}

void evaluateCircuit (gate *& output){
    if(output && output->outputSet==0){
        if(output->pLInput && output->pLInput->outputSet==0)
            evaluateCircuit(output->pLInput);
        if(output->pRInput && output->pRInput->outputSet==0)
            evaluateCircuit(output->pRInput);
        if(output->pRInput)
            output->output = output->operation(output->pLInput->output, output->pRInput->output);
        else
            output->output = output->operation(output->pLInput->output, 0);
        output->outputSet = 1;
    }
}

void cleanTree(gate * pR) {
    if (pR)
    {
        cleanTree (pR->pLeft);
        pR->outputSet = 0;
        cleanTree (pR->pRight);
    }
}

void createStructure (string file, bool &fileStructure, gate*& pRoot, map<string, gateFnPtr> gateFunc, int& lInput, int& rInput, int& output) {
    ifstream input_stream (file);
    if (input_stream) {
        string n;
        regex pattern(R"((NOT\s*[0-9]+\s*[0-9]+)|(IN\s*:\s*[0-9]+\s*[0-9]+)|(OUT\s*:\s*[0-9]+)|(\w*\s*[0-9]+\s*[0-9]+\s*[0-9]+))"); //regular expression checking if pattern in file is correct
        regex empty_line(R"(^\s*$)");
        while (getline(input_stream, n)) {
            if (regex_search(n, pattern) || regex_search(n, empty_line)) { //checking pattern
                istringstream buf(n);
                istream_iterator<string> beg(buf), end;
                vector<string> formatted(beg,end); //converting string (one line taken from file) into vector for easier manipulation

                if(formatted.size() > 0) {
                    if (formatted[0] == "IN:") {
                        lInput = stoi(formatted[1]);
                        rInput = stoi(formatted[2]);
                    } else if (formatted[0] == "OUT:") {
                        output = stoi(formatted[1]);
                    } else if (validGateName(formatted[0]) && formatted[0] != "not") {
                        int numberTmp = stoi(formatted[3]);
                        int lInputTmp = stoi(formatted[1]);
                        auto pLInputTmp = find(pRoot, lInputTmp);
                        int rInputTmp = stoi(formatted[2]);
                        auto pRInputTmp = find(pRoot, rInputTmp);

                        addToTree(pRoot, numberTmp, pLInputTmp, lInputTmp, pRInputTmp, rInputTmp,
                                  gateFunc[formatted[0] + "Func"]);
                    } else if (validGateName(formatted[0]) && formatted[0] == "not") {
                        int numberTmp = stoi(formatted[2]);
                        int lInputTmp = stoi(formatted[1]);
                        auto pLInputTmp = find(pRoot, lInputTmp);

                        addToTree(pRoot, numberTmp, pLInputTmp, lInputTmp, nullptr, 0, notFunc);
                    } else {
                        cout << "ERROR: There's no such gate: " << formatted[0] << endl;
                        fileStructure = false;
                        break;
                    }
                }
            } else {
                cout << "ERROR: There's something wrong with circuit input file structure" << endl;
                fileStructure = false;
                break;
            }
        }
        input_stream.close();
    } else {
        cout << "ERROR: Wrong path (parameter -u)" << endl;
        fileStructure = false;
    }
}

void createStatesList(string file, bool &fileInputs, item *& pStates, int lInput, int rInput) {
    ifstream input_stream (file);
    if (input_stream) {
        string n;
        regex pattern("[0-9]+\\s*:\\s*[0-1]\\s*[0-9]+\\s*:\\s*[0-1]");
        regex empty_line(R"(^\s*$)");
        while (getline(input_stream, n)) {
            if(regex_search(n, pattern) || regex_search(n, empty_line)){
                replace(n.begin(), n.end(), ':', ' ');
                istringstream buf(n);
                istream_iterator<string> beg(buf), end;
                vector<string> formatted(beg, end); //converting string (one line taken from file) into vector for easier manipulation

                if(formatted.size() > 0) {
                    int input1Tmp = stoi(formatted[0]);
                    int input2Tmp = stoi(formatted[2]);
                    bool state1Tmp = stoi(formatted[1]);
                    bool state2Tmp = stoi(formatted[3]);

                    if (input1Tmp == lInput && input2Tmp == rInput) { //complex condition checking, if inputs match inputs form circuit file
                        addToList(pStates, state1Tmp, state2Tmp);
                    } else if (input2Tmp == lInput && input1Tmp == rInput) {
                        addToList(pStates, state2Tmp, state1Tmp);
                    } else {
                        cout << "ERROR: One of the input numbers is not correct and can not be calculated!" << endl;
                    }
                }
            } else {
                cout << "ERROR: One of the input states is not correct and can not be calculated!" << endl;
            }
        }
    } else {
        cout << "ERROR: Wrong path (parameter -i)" << endl;
        fileInputs = false;
    }
}

void processData(item * iterator, gate* pRoot, int lInput, int rInput, int output, string output_file) {
    bool error_displayed = false;
    while(iterator){
        auto lInputGate = find(pRoot, lInput);
        if(lInputGate){
            lInputGate->output = iterator->lInput;
            lInputGate->outputSet = 1;
        }

        auto rInputGate = find(pRoot, rInput);
        if(rInputGate){
            rInputGate->output = iterator->rInput;
            rInputGate->outputSet = 1;
        }

        auto outputGate = find(pRoot, output);
        evaluateCircuit(outputGate);

        ofstream output_stream;
        output_stream.open(output_file, ios_base::app);
        if (output_stream)
        {
            output_stream << "IN: " << lInput << ":" << iterator->lInput << " " << rInput << ":" << iterator->rInput << " OUT:" << output << ":" << outputGate->output << endl;
            output_stream.close();
        } else {
            if(!error_displayed){
                cout << "ERROR: Can't open output file!" << endl;
                error_displayed = true;
            }
        }

        cout << "IN: " << lInput << ":" << iterator->lInput << " " << rInput << ":" << iterator->rInput << " OUT:" << output << ":" << outputGate->output << endl;
        iterator = iterator->pNext;
        cleanTree(pRoot);
    }
}