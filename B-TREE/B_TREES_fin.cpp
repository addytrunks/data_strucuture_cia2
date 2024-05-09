// Program to implement B-Trees

#include <stdio.h>
#include <stdlib.h>

class BTree
{

private:
    // Function to split the child y of node x
    void splitchild(BTree *x, int i, BTree *y, int t)
    {
        BTree *z = new BTree(y->t, y->leaf, t);
        z->nokeys = t - 1;

        for (int j = 0; j < t - 1; j++)
            z->keys[j] = y->keys[j + t];

        if (!y->leaf)
        {
            for (int j = 0; j < t; j++)
                z->child[j] = y->child[j + t];
        }

        y->nokeys = t - 1;

        for (int j = x->nokeys; j >= i + 1; j--)
            x->child[j + 1] = x->child[j];

        x->child[i + 1] = z;

        for (int j = x->nokeys - 1; j >= i; j--)
            x->keys[j + 1] = x->keys[j];

        x->keys[i] = y->keys[t - 1];
        x->nokeys = x->nokeys + 1;
    }

    // Function to insert key into btree
    void insertnew(BTree *x, int k, int t)
    {
        int i = x->nokeys - 1;

        if (x->leaf)
        {
            while (i >= 0 && k < x->keys[i])
            {
                x->keys[i + 1] = x->keys[i];
                i--;
            }

            x->keys[i + 1] = k;
            x->nokeys = x->nokeys + 1;
        }
        else
        {
            while (i >= 0 && k < x->keys[i])
                i--;

            if (x->child[i + 1]->nokeys == 2 * t - 1)
            {
                splitchild(x, i + 1, x->child[i + 1], t);

                if (k > x->keys[i + 1])
                    i++;
            }

            insertnew(x->child[i + 1], k, t);
        }
    }

    // Function to delete a key from the B-tree
    void deleteKey(BTree *root, int key)
    {
        if (!root)
            return;

        int index = 0;
        while (index < root->nokeys && key > root->keys[index])
            index++;

        if (index < root->nokeys && key == root->keys[index])
        {
            // Case 1: The key is in a leaf node
            if (root->leaf)
            {
                for (int i = index + 1; i < root->nokeys; ++i)
                    root->keys[i - 1] = root->keys[i];

                root->nokeys--;
            }
            else
            {
                // Case 2: The key is in an internal node
                BTree *pred = root->child[index];
                BTree *succ = root->child[index + 1];

                // Case 2a: The predecessor has at least t keys
                if (pred->nokeys >= t)
                {
                    int predKey = getPred(pred);
                    root->keys[index] = predKey;
                    deleteKey(pred, predKey);
                }
                // Case 2b: The successor has at least t keys
                else if (succ->nokeys >= t)
                {
                    int succKey = getSucc(succ);
                    root->keys[index] = succKey;
                    deleteKey(succ, succKey);
                }
                // Case 2c: Both predecessor and successor have t-1 keys
                else
                {
                    mergeNodes(root, index);
                    deleteKey(pred, key);
                }
            }
        }
        else
        {
            // If the key is not present in this node, find the child node that would contain the key
            BTree *child = root->child[index];
            if (!child)
            {
                printf("Key %d does not exist in the B-tree\n", key);
                return;
            }

            // Ensure that the child node has at least t keys
            if (child->nokeys == t - 1)
            {
                // Case 3a: If the left sibling has more than t-1 keys, borrow from it
                if (index > 0 && root->child[index - 1]->nokeys >= t)
                    borrowFromLeft(root, index);
                // Case 3b: If the right sibling has more than t-1 keys, borrow from it
                else if (index < root->nokeys && root->child[index + 1]->nokeys >= t)
                    borrowFromRight(root, index);
                // Case 3c: Merge with a sibling
                else
                {
                    if (index < root->nokeys)
                        mergeNodes(root, index);
                    else
                        mergeNodes(root, index - 1);
                }
            }
            deleteKey(child, key);
        }
    }

    // Function to borrow a key from the left sibling
    void borrowFromLeft(BTree *parent, int index)
    {
        BTree *child = parent->child[index];
        BTree *sibling = parent->child[index - 1];

        // Shift all keys in child to the right
        for (int i = child->nokeys - 1; i >= 0; --i)
            child->keys[i + 1] = child->keys[i];

        // If the child is not a leaf, shift all child pointers in child to the right
        if (!child->leaf)
        {
            for (int i = child->nokeys; i >= 0; --i)
                child->child[i + 1] = child->child[i];
        }

        // Move the key from the parent to the child
        child->keys[0] = parent->keys[index - 1];

        // If the sibling is not a leaf, move its rightmost child pointer to the child's leftmost child pointer
        if (!sibling->leaf)
            child->child[0] = sibling->child[sibling->nokeys];

        // Move the rightmost key from the sibling to the parent
        parent->keys[index - 1] = sibling->keys[sibling->nokeys - 1];

        // Update key counts
        child->nokeys++;
        sibling->nokeys--;
    }

    // Function to borrow a key from the right sibling
    void borrowFromRight(BTree *parent, int index)
    {
        BTree *child = parent->child[index];
        BTree *sibling = parent->child[index + 1];

        // Move the key from the parent to the child
        child->keys[child->nokeys] = parent->keys[index];

        // If the sibling is not a leaf, move its leftmost child pointer to the child's rightmost child pointer
        if (!sibling->leaf)
            child->child[child->nokeys + 1] = sibling->child[0];

        // Move the leftmost key from the sibling to the parent
        parent->keys[index] = sibling->keys[0];

        // Shift all keys in the sibling to the left
        for (int i = 1; i < sibling->nokeys; ++i)
            sibling->keys[i - 1] = sibling->keys[i];

        // If the sibling is not a leaf, shift all child pointers in the sibling to the left
        if (!sibling->leaf)
        {
            for (int i = 1; i <= sibling->nokeys; ++i)
                sibling->child[i - 1] = sibling->child[i];
        }

        // Update key counts
        child->nokeys++;
        sibling->nokeys--;
    }

    // Function to merge two nodes along with their parent
    void mergeNodes(BTree *parent, int index)
    {
        BTree *child = parent->child[index];
        BTree *sibling = parent->child[index + 1];

        // Move the key from the parent to the child
        child->keys[t - 1] = parent->keys[index];

        // Copy keys from the sibling to the child
        for (int i = 0; i < sibling->nokeys; ++i)
            child->keys[i + t] = sibling->keys[i];

        // If the child is not a leaf, copy child pointers from the sibling to the child
        if (!child->leaf)
        {
            for (int i = 0; i <= sibling->nokeys; ++i)
                child->child[i + t] = sibling->child[i];
        }

        // Shift keys and child pointers in the parent
        for (int i = index + 1; i < parent->nokeys; ++i)
            parent->keys[i - 1] = parent->keys[i];

        for (int i = index + 2; i <= parent->nokeys; ++i)
            parent->child[i - 1] = parent->child[i];

        // Update key counts
        child->nokeys += sibling->nokeys + 1;
        parent->nokeys--;

        // Deallocate memory for the sibling
        delete sibling;
    }

    // Function to find the predecessor of a node
    int getPred(BTree *root)
    {
        while (!root->leaf)
            root = root->child[root->nokeys];

        return root->keys[root->nokeys - 1];
    }

    // Function to find the successor of a node
    int getSucc(BTree *root)
    {
        while (!root->leaf)
            root = root->child[0];

        return root->keys[0];
    }

public:
    int *keys;
    int t;
    BTree **child;
    int nokeys;
    bool leaf;

    BTree(int temp, bool isleaf, int degree)
    {
        t = degree; // Initialize 't' with the passed value
        keys = new int[2 * t - 1];
        child = new BTree *[2 * t];
        nokeys = 0;
        leaf = isleaf;

        for (int i = 0; i < 2 * t - 1; i++)
            keys[i] = 0;

        for (int i = 0; i < 2 * t; i++)
            child[i] = nullptr;
    }

    // Method to insert node into btree
    void insert(BTree **root, int key, int t)
    {
        if (*root == nullptr)
        {
            *root = new BTree(t, true, t);
            (*root)->keys[0] = key;
            (*root)->nokeys = 1;
        }
        else
        {
            // If the keys of the root are full, then the median key is pushed up and the root is split.
            if ((*root)->nokeys == 2 * t - 1)
            {
                BTree *newRoot = new BTree(t, false, t);
                newRoot->child[0] = *root;
                splitchild(newRoot, 0, *root, t);

                int i = 0;
                if (newRoot->keys[0] < key)
                    i++;

                insertnew(newRoot->child[i], key, t);
                *root = newRoot;
            }
            else
            {
                insertnew(*root, key, t);
            }
        }
    }

    // Method to search for a node in btree
    bool search(BTree *root, int key)
    {
        if (root == nullptr)
            return false;

        int i = 0;
        while (i < root->nokeys && key > root->keys[i])
            i++;

        if (i < root->nokeys && key == root->keys[i])
            return true;

        if (root->leaf)
            return false;

        return search(root->child[i], key);
    }

    // Method to display a B-tree
    void display(BTree *root, int level = 0)
    {
        if (root == nullptr)
            return;

        // Traverse child nodes in reverse order
        for (int i = root->nokeys; i > 0; i--)
        {
            // Display child nodes recursively
            display(root->child[i], level + 1);

            // Print keys with appropriate indentation
            printf("%*s%d\n", level * 4, "", root->keys[i - 1]);
        }

        // Display the first child
        display(root->child[0], level + 1);
    }

    // Method to delete a key from the B-tree
    void deleteOperation(BTree *root, int key)
    {
        deleteKey(root, key);
    }

    bool isEmpty(BTree *root)
    {
        return root == nullptr;
    }
};

int main()
{
    int t, u, choice;
    // KEYS = 2 * t - 1 (Maximum number of keys in a node)
    // CHILDREN = 2 * t (Maximum number of children a node can have)
    printf("\nKEYS = 2 * m - 1\nCHILDREN = 2 * m\nEnter value of minimum degree(m):");
    scanf("%d", &t);
    BTree tree(t, true, t);
    BTree *root = NULL;
    while (1)
    {
        printf("\n1.Insert\n2.Delete\n3.Search\n4.Display\n5.Exit\nEnter your choice:");
        while (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            scanf("%*s"); // Clear input buffer
        }
        switch (choice)
        {
        case 1:
            printf("Enter value (ONLY INTEGER) to enter into node:");
            while (scanf("%d", &u) != 1)
            {
                printf("Invalid input. Please enter a number.\n");
                scanf("%*s"); // Clear input buffer
            }
            tree.insert(&root, u, t);
            printf("Key %d inserted successfully.\n", u);
            break;
        case 2:
            if (root == NULL)
            {
                printf("B-Tree is empty.Cannot delete.\n");
            }
            else
            {
                printf("Enter value to delete from node:");
                while (scanf("%d", &u) != 1)
                {
                    printf("Invalid input. Please enter a number.\n");
                    scanf("%*s"); // Clear input buffer
                }
                bool is_found;
                is_found = tree.search(root, u);

                if (!is_found)
                {
                    printf("Key %d not found in the B-Tree\n", u);
                    break;
                }
                else
                {
                    tree.deleteOperation(root, u);
                    printf("Key %d deleted successfully.\n", u);
                }
            }
            break;
        case 3:
            int searchKey;

            if (root == NULL)
            {
                printf("B-Tree is empty. Nothing to search.\n");
                break;
            }

            printf("Enter key (ONLY INTEGER) to search: ");
            while (scanf("%d", &searchKey) != 1)
            {
                printf("Invalid input. Please enter a number.\n");
                scanf("%*s"); // Clear input buffer
            }
            bool is_found;
            is_found = tree.search(root, searchKey);
            if (is_found)
            {
                printf("Key %d found in the B-Tree\n", searchKey);
            }
            else
            {
                printf("Key %d not found in the B-Tree\n", searchKey);
            }
            break;
        case 4:
            printf("B-Tree is as follows:\n");
            tree.display(root);
            break;
        case 5:
            exit(0);
            break;
        default:
            printf("Invalid choice. Try again");
            break;
        }
    }
    return 0;
}
