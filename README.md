# logical_circuits_calculator
C++ program for calculating output of the logical circuit.

# Sample input file with logical gates:
IN: 1 2
OUT: 10
AND 1 2 3
OR 1 3 4
XOR 2 3 5
NOR 4 5 6
AND 6 5 7
NOT 7 8
AND 4 8 9
XNOR 9 8 10

# Sample input file with input states:
1:0 2:0
1:0 2:1
1:1 2:0
1:1 2:1

# Sample output:
IN: 1:0 2:0 OUT:10:0
IN: 1:0 2:1 OUT:10:0
IN: 1:1 2:0 OUT:10:1
IN: 1:1 2:1 OUT:10:1
