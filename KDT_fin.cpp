#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <algorithm> // for std::min
#include <iostream>  // for std::cin and std::cout
#include <string>    // for std::string and std::getline
#include <sstream>   // for std::istringstream

#define COUNT 10

// Node structure
struct Node
{
    int *data;          // Array to store data (coordinates)
    struct Node *left;  // Pointer to the left child
    struct Node *right; // Pointer to the right child
};

class Tree
{
private:
    struct Node *root; // Pointer to the root of the tree

    // Helper function to create a new node
    struct Node *createNode(int *data, int dim)
    {
        // Allocate memory for a new node
        struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
        if (newNode == NULL)
        {
            printf("Memory allocation failed.");
            exit(1);
        }
        // Allocate memory for data array in the node and copy data
        newNode->data = (int *)malloc(dim * sizeof(int));
        if (newNode->data == NULL)
        {
            printf("Memory allocation failed.");
            exit(1);
        }
        for (int i = 0; i < dim; ++i)
            newNode->data[i] = data[i];
        // Initialize left and right child pointers
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    // Helper function to insert a node recursively
    struct Node *insertHelperFunction(struct Node *root, int *data, int dim, int depth)
    {
        // If the current node is NULL, create a new node and return
        if (root == NULL)
            return createNode(data, dim);

        // Calculate the axis to compare based on the depth
        int axis = depth % dim;

        // Recursively insert the node based on the value along the axis
        if (data[axis] < (root->data[axis]))
            root->left = insertHelperFunction(root->left, data, dim, depth + 1);
        else
            root->right = insertHelperFunction(root->right, data, dim, depth + 1);

        return root;
    }

    // Helper function to compare two arrays
    bool compareArrays(int *arr1, int *arr2, int dim)
    {
        for (int i = 0; i < dim; ++i)
        {
            if (arr1[i] != arr2[i])
                return false;
        }
        return true;
    }

    // Helper function to check if a node is a leaf
    bool isLeaf(struct Node *node)
    {
        return (node->left == NULL && node->right == NULL);
    }

    // Helper function to find the minimum value in a dimension recursively
    int minValue(struct Node *root, int depth, int dim, int axis)
    {
        // If the current node is NULL, return the maximum possible value
        if (root == NULL)
            return INT_MAX;

        // Calculate the axis to compare based on the depth
        int cd = depth % dim;

        // If the current axis matches the target axis, find the minimum value
        if (cd == axis)
        {
            if (root->left == NULL)
                return root->data[axis];
            return std::min(root->data[axis], std::min(minValue(root->left, depth + 1, dim, axis),
                                                       minValue(root->right, depth + 1, dim, axis)));
        }

        // Recursively search in the left and right subtrees
        return std::min(root->data[axis], std::min(minValue(root->left, depth + 1, dim, axis),
                                                   minValue(root->right, depth + 1, dim, axis)));
    }

    // Helper function to find the maximum value in the left subtree recursively
    int *findMaxLeftData(struct Node *root, int dim, int depth)
    {
        if (root->right == NULL)
            return root->data;
        return findMaxLeftData(root->right, dim, depth + 1);
    }

    // Helper function to find the minimum value in the right subtree recursively
    int *findMinRightData(Node *root, int dim, int depth)
    {
        if (root == NULL)
            return NULL;

        int cd = depth % dim;

        if (root->left == NULL)
            return root->data;

        return findMinRightData(root->left, dim, depth + 1);
    }

    // Utility function to delete a node recursively
    struct Node *deleteNodeUtil(Node *root, int *data, int dim, int depth)
    {
        if (root == NULL)
            return NULL;

        int cd = depth % dim;

        if (compareArrays(root->data, data, dim))
        {
            // Case 1: Node is a leaf
            if (isLeaf(root))
            {
                free(root->data);
                free(root);
                return NULL;
            }
            // Case 2: Node has only left child
            else if (root->right == NULL)
            {
                Node *temp = root->left;
                free(root->data);
                free(root);
                return temp;
            }
            // Case 3: Node has only right child
            else if (root->left == NULL)
            {
                Node *temp = root->right;
                free(root->data);
                free(root);
                return temp;
            }
            // Case 4: Node has both left and right children
            else
            {
                // Find the minimum value in the right subtree
                int *minRightData = findMinRightData(root->right, dim, depth + 1);
                // Replace the root's data with the minimum value
                for (int i = 0; i < dim; ++i)
                    root->data[i] = minRightData[i];
                // Delete the node with the minimum value from the right subtree
                root->right = deleteNodeUtil(root->right, minRightData, dim, depth + 1);
            }
        }
        else if (data[cd] < root->data[cd])
        {
            root->left = deleteNodeUtil(root->left, data, dim, depth + 1);
        }
        else
        {
            root->right = deleteNodeUtil(root->right, data, dim, depth + 1);
        }

        return root;
    }

    // Helper function to search for a node recursively
    bool searchUtil(struct Node *root, int *data, int dim, int depth)
    {
        // If the current node is NULL, return false
        if (root == NULL)
            return false;

        // If the current node's data matches the target data, return true
        if (compareArrays(root->data, data, dim))
            return true;

        // Calculate the axis to compare based on the depth
        int cd = depth % dim;

        // Recursively search in the left subtree if data is less than root's data
        if (data[cd] < root->data[cd])
            return searchUtil(root->left, data, dim, depth + 1);

        // Recursively search in the right subtree if data is greater than or equal to root's data
        return searchUtil(root->right, data, dim, depth + 1);
    }

    // Helper function to print the tree in 2D recursively
    void print2DUtil(struct Node *root, int space, int dim)
    {
        // If the current node is NULL, return
        if (root == NULL)
            return;

        // Increase indentation for each level
        space += COUNT;

        // Process the right subtree
        print2DUtil(root->right, space, dim);

        // Print the current node's data with proper indentation
        printf("\n");
        for (int i = COUNT; i < space; i++)
            printf("--");
        printf(">");
        printf("[");
        for (int i = 0; i < dim; i++)
            printf(" %d ", root->data[i]);
        printf("]");
        printf("\n");

        // Process the left subtree
        print2DUtil(root->left, space, dim);
    }

public:
    // Constructor
    Tree()
    {
        root = NULL;
    }

    struct Node *getRoot()
    {
        return root;
    }

    // Function to insert a node
    void insert(int *data, int dim)
    {
        root = insertHelperFunction(root, data, dim, 0);
    }

    // Function to delete a node
    void deleteNode(int *data, int dim)

    {
        if (root == NULL)
        {
            printf("Tree is empty. Cannot delete anything.");
            return;
        }
        else
        {

            root = deleteNodeUtil(root, data, dim, 0);
        }
    }

    // Function to search for a node
    bool search(int *data, int dim)
    {
        return searchUtil(root, data, dim, 0);
    }

    // Function to display the tree in 2D
    void displayTree(int dim)
    {
        if (root == NULL)
        {
            printf("Tree is empty. Nothing to display.");
            return;
        }
        else
        {

            print2DUtil(root, 0, dim);
        }
    }
};

// Main function
int main()
{
    int temparr[COUNT];
    int choice;
    int dim;
    Tree t;
    printf("Enter 'k' for k-dimension tree. (basically the dimension): ");
    scanf("%d", &dim);
    bool found;

    // Menu-driven loop
    while (1)
    {
        printf("\n\nEnter\n1. Insert\n2. Delete\n3. Search\n4. Display Tree\n5.Exit\nEnter your choice:");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("\nEnter %d number elements for one node to be inserted: \n", dim);
            for (int i = 0; i < dim; i++)
            {
                while (scanf("%d", &temparr[i]) != 1)
                {
                    printf("Invalid input. Please enter an integer value.\n");
                    scanf("%*s"); // Clear input buffer
                }
            }
            t.insert(&temparr[0], dim);
            printf("\nSuccesfully entered the %d-dimensional array.", dim);
            break;
        case 2:
            if (t.getRoot() == NULL)
            {
                printf("Tree is empty. Nothing to delete.");
                break;
            }
            else
            {
                printf("\nEnter the element you want to be deleted (must be in %d dimensions):\n ", dim);
                for (int i = 0; i < dim; i++)
                {
                    while (scanf("%d", &temparr[i]) != 1)
                    {
                        printf("Invalid input. Please enter an integer value.\n");
                        scanf("%*s"); // Clear input buffer
                    }
                }
                t.deleteNode(temparr, dim);
            }
            break;
        case 3:
            printf("Enter the elements that have %d-dimensions you want to search for...", dim);
            for (int i = 0; i < dim; i++)
            {
                while (scanf("%d", &temparr[i]) != 1)
                {
                    printf("Invalid input. Please enter an integer value.\n");
                    scanf("%*s"); // Clear input buffer
                }
            }
            found = t.search(&temparr[0], dim);
            if (found)
                printf("Element found!\n");
            else
                printf("Element not found!\n");
            break;
        case 4:
            printf("Printing in 2D:\n");
            t.displayTree(dim);
            break;
        case 5:
            printf("Exiting the program...");
            exit(0);
            break;
        default:
            printf("Enter a valid choice.");
        }
    }
    return 0;
}
