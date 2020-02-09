#ifndef STRUCTURES_H
#define STRUCTURES_H

/** Structure of the node in binary tree - in this case represented as logical gate */
struct gate {
    int number; ///< number of the gate
    bool output; ///< boolean output of the gate
    bool outputSet; ///< flag (check if gate is calculated)

    /** pointers to inputs of gates */
    gate *pLInput, *pRInput;

    /** pointers to childs of the tree */
    gate *pLeft, *pRight;

    bool (*operation) (bool, bool); ///function calculate output of the gate depends on gate type (operation)
};

/** Structure of the item in singly linked list, storing input states */
struct item {
    int lInput, ///< left input state in a circuit
        rInput; ///< right input state in a circuit
    item * pNext; ///< pointer to the next element in the list
};

#endif
