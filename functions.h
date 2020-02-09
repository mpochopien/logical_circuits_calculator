#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>

typedef bool (*gateFnPtr)(bool, bool);

#include "structures.h"

using namespace std;

/** The function finds nodes in the tree.
@param pR address of the root of the tree
@param number number of the gate to find
@return address of the gate in the tree or nullptr if not found
*/
gate * find (gate * pR, const int number);

/** The function adds a new node to the tree (adds node to future use - functions only initialize new node with gate number).
@param pR address of the root of the tree
@param number number of the gate to add
@return pointer to created gate */
gate * addToTreeTemp (gate *& pR, const int number);

/** The function adds a new node to the tree.
@param pR address of the root of the tree
@param number number of the gate to add
@param pLInput address of the left gate input
@param lInputTmp number of the left gate input
@param pRInput address of the right gate input
@param rInputTmp number of the right gate input
@param operation pointer to the function indicating bitwise operation on the gate */
void addToTree (gate *& pR, const int number,
                gate * pLInput, const int lInputTmp, 
                gate * pRInput, const int rInputTmp, 
                bool (*operation)(bool, bool));

/** The function removes the tree.
@param pR address of the root of the tree */
void deleteTree (gate *& pR);

/** The function calculate logical value of AND gate.
@param input1 first logical value on the input
@param input2 second logical value on the input
@return logical value from AND operation */
bool andFunc (const bool input1, const bool input2);

/** The function calculate logical value of OR gate.
@param input1 first logical value on the input
@param input2 second logical value on the input
@return logical value from OR operation */
bool orFunc (const bool input1, const bool input2);

/** The function calculate logical value of XOR gate.
@param input1 first logical value on the input
@param input2 second logical value on the input
@return logical value from XOR operation */
bool xorFunc (const bool input1, const bool input2);

/** The function calculate logical value of NOT gate.
@param input1 logical value on the input
@param input2 always 0, considering that this is NOT gate (only for gate structure purpose)
@return logical value from NOT operation (simply negated value) */
bool notFunc (const bool input1, const bool input2 = 0);

/** The function calculate logical value of NAND gate.
@param input1 first logical value on the input
@param input2 second logical value on the input
@return logical value from NAND operation */
bool nandFunc (const bool input1, const bool input2);

/** The function calculate logical value of NOR gate.
@param input1 first logical value on the input
@param input2 second logical value on the input
@return logical value from NOR operation */
bool norFunc (const bool input1, const bool input2);

/** The function calculate logical value of XNOR gate.
@param input1 first logical value on the input
@param input2 second logical value on the input
@return logical value from XNOR operation */
bool xnorFunc (const bool input1, const bool input2);

/** The function validates if passed string is a valid logical gate name.
 *
 * @param[in,out] gateName name of the gate to validate
 * @return true if name is valid, otherwise false
 */
bool validGateName (string & gateName);

/** Function adds at the end of the list.
 *
 * @param[in,out] pH pointer to the first element in the list
 * @param lInput left input into a circuit
 * @param rInput right input into a circuit
*/
void addToList(item * & pH, const int lInput, const int rInput);

/** Function removes the list.
 *
 * @param[in,out] pH pointer to the first element in the list
*/
void removeList(item * & pH);

/** Function evaluates given circuit.
 *
 * @param[in,out] output output gate of the circuit
*/
void evaluateCircuit (gate *& output);

/** Function sets default values in tree.
 *
 * @param pR pointer to root of a tree
*/
void cleanTree(gate * pR);

/** Function creates circuit structure in binary tree.
 *
 * @param file name of the file with circuit structure to read
 * @param[in,out] fileStructure flag indicating if there's no error during data structure create
 * @param[in,out] pRoot addres of the root of the tree
 * @param gateFunc map with pointers to functions calculating outputs of gates
 * @param[in,out] lInput number of left circuit input
 * @param[in,out] rInput number of right circuit input
 * @param[in,out] output number of circuit output
 */
void createStructure (string file, bool &fileStructure, gate*& pRoot, map<string, gateFnPtr> gateFunc, int& lInput, int& rInput, int& output);

/** Function creates list with input states to evaluate.
 *
 * @param file name of the file with circuit structure to read
 * @param[in,out] fileInputs flag indicating if there's no error during data structure create
 * @param[in,out] pStates pointer to the first element in the list
 * @param lInput number of left circuit input
 * @param rInput number of right circuit input
 */
void createStatesList(string file, bool &fileInputs, item *& pStates, int lInput, int rInput);

/** Function is processing data given in input states.
 *
 * @param iterator address of the list with input states to evaluate
 * @param pRoot addres of the root of the tree
 * @param[in,out] lInput number of left circuit input
 * @param[in,out] rInput number of right circuit input
 * @param[in,out] output number of circuit output
 * @param[in,out] output_file name of the output file
 */
void processData(item * iterator, gate* pRoot, int lInput, int rInput, int output, string output_file);

#endif
