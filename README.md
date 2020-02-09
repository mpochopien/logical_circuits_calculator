# logical_circuits_calculator
C++ program for calculating output of the logical circuit.

# Sample input file with logical gates:
IN: 1 2<br />
OUT: 10<br />
AND 1 2 3<br />
OR 1 3 4<br />
XOR 2 3 5<br />
NOR 4 5 6<br />
AND 6 5 7<br />
NOT 7 8<br />
AND 4 8 9<br />
XNOR 9 8 10

# Sample input file with input states:
1:0 2:0<br />
1:0 2:1<br />
1:1 2:0<br />
1:1 2:1

# Sample output:
IN: 1:0 2:0 OUT:10:0<br />
IN: 1:0 2:1 OUT:10:0<br />
IN: 1:1 2:0 OUT:10:1<br />
IN: 1:1 2:1 OUT:10:1
