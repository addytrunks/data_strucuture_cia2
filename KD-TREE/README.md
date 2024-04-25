# KD Tree

## Overview

A KD Tree, short for K-Dimensional Tree, is a data structure used for organizing points in a k-dimensional space. It is a binary tree structure where each node represents a k-dimensional point. KD Trees are particularly useful for efficient nearest neighbor search and range search operations in multi-dimensional spaces.

This repository contains an implementation of a KD Tree in C++. The KD Tree is implemented as a class, providing methods for insertion, deletion, search, and display of the tree.

## Functions

### 1. Constructor

- **Description:** Initializes an empty KD Tree.

### 2. `insert`

- **Description:** Inserts a new k-dimensional point into the KD Tree.
- **Parameters:**
  - `data`: An array containing the coordinates of the point to be inserted.
  - `dim`: The dimensionality of the point (number of coordinates).

### 3. `deleteNode`

- **Description:** Deletes a specified k-dimensional point from the KD Tree.
- **Parameters:**
  - `data`: An array containing the coordinates of the point to be deleted.
  - `dim`: The dimensionality of the point (number of coordinates).

### 4. `search`

- **Description:** Searches for a specified k-dimensional point in the KD Tree.
- **Parameters:**
  - `data`: An array containing the coordinates of the point to be searched.
  - `dim`: The dimensionality of the point (number of coordinates).
- **Returns:** `true` if the point is found, `false` otherwise.

### 5. `displayTree`

- **Description:** Displays the structure of the KD Tree in 2D, showing the arrangement of nodes and their coordinates.
- **Parameters:**
  - `dim`: The dimensionality of the points stored in the tree.

## Usage

To use the KD Tree implementation, follow these steps:

1. Include the `Tree.h` header file in your C++ program.
2. Create a `Tree` object.
3. Call the appropriate methods (`insert`, `deleteNode`, `search`, `displayTree`) as needed.