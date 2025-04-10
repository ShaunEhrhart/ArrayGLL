# ArrayGLL – Generic Linked List in Array Form

A templated C++ implementation of a generalized linked list (GLL) using an array of custom objects. Each node stores a pointer to info (`_Info`), a pointer to the next sibling (`_Next`), and a pointer to the first child (`_Down`).

## Features
- Templated design for generic data types
- Deep copy constructors and assignment operators
- Hierarchical structure traversal via recursion
- Find, display path, and parent lookup functions
- Tracks free nodes and size calculation
- Display in nested parentheses format

## How to Use
Compile and run with standard input:
```bash
g++ main.cpp -o gll
./gll
