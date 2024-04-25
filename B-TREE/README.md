# B-Tree Data Structure

## Overview

A B-tree is a self-balancing tree data structure that maintains sorted data and allows for efficient search, insertion, and deletion operations. It is commonly used in databases and file systems where large amounts of data need to be stored and accessed efficiently.

## B-Tree Structure

A B-tree is characterized by the following properties:

- Each node can contain a variable number of keys.
- Nodes are kept balanced, ensuring that the depth of the tree remains small.
- All leaf nodes are at the same level.
- Keys in each node are stored in sorted order.

## Implemented Functions

The following functions are implemented in the provided code:

1. **Insertion**: Inserts a new key into the B-tree while maintaining its properties.
2. **Deletion**: Deletes a key from the B-tree while preserving its balance.
3. **Search**: Searches for a key in the B-tree.
4. **Display**: Displays the keys of the B-tree in sorted order.
5. **Split Child**: Splits a child node into two when it reaches its maximum capacity.
6. **Merge Nodes**: Merges two nodes along with their parent when necessary during deletion.
7. **Borrow From Left/Right**: Borrows a key from the left or right sibling when a node becomes deficient during deletion.
8. **Get Predecessor/Successor**: Finds the predecessor or successor key of a given node.

## How to Use

To use the provided B-tree implementation, follow these steps:

1. Clone or download the repository.
2. Compile the code using a C++ compiler.
3. Execute the compiled program.
4. Choose an operation from the menu:
   - Insert: Insert a new key into the B-tree.
   - Delete: Delete a key from the B-tree.
   - Search: Search for a key in the B-tree.
   - Display: Display the keys of the B-tree in sorted order.
   - Exit: Exit the program.