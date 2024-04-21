#include <stdio.h>
#include <stdlib.h>

// Maximal number of dimensions
#define COUNT 10

class tree
{
  struct node
  {
    struct node* right;
    int data[COUNT];
    struct node* left;
  };
  
public:
  struct node* root;
  tree()
  {
    root=NULL;
  }
  void insert(int*, int);
  struct node * insertRec(struct node*, int*, int, int); 
  //first int array to store the data that needs to be added in the tree
  // second int to check which element needs to be checked
  // third int for counter that keeps incrementing

  void search(int*, int); //sending the array and the dimension
  int searchRec(struct node*, int*, int, int);

  void deleteNode(int*, int );
  void deleteElement(struct node *, struct node *, int* , int , int );
  // for parent node, current node, array int, dimension and count 

  void print2D(int);
  void print2DUtil(struct node*, int, int);
};

int main()
{
  int num;
  int temparr[COUNT];
  int choice;
  int dim;
  tree t;
  printf("Enter 'k' for k-dimension tree. (basically the dimension): ");
  scanf("%d",&dim);

  while(1)
  {
    printf("\n\nEnter\n1. insert\n2. Delete\n3. Search\n4. Exit:\n5. print in 2D ");
    scanf("%d",&choice);
    switch(choice)
    {
    case 1:
      printf("\nEnter %d number elements for one node to be inserted: ",dim);
      for(int i=0; i<dim; i++)
        scanf("%d",&temparr[i]);
      t.insert(&temparr[0],dim);
      printf("\nSuccesfully entered the %d-dimensional array \nthe tree after input is: ",dim);
      t.print2D(dim);
      break;
    case 2:
      printf("\nEnter the element you want to be deleted (must be in %d dimensions): ",dim);
      for(int i=0; i<dim; i++)
        scanf("%d",&temparr[i]);
      t.deleteNode(&temparr[0], dim);
      printf("\nAfter Deleting the %d-dimensional array: \nthe tree after input is: ",dim);
      t.print2D(dim);
      break;
    case 3:
      printf("Enter the elements that have %d-dimensions you want to search for...",dim);
      for(int i=0; i<dim; i++)
        scanf("%d",&temparr[i]);
      t.search(&temparr[0],dim);
      break;
    case 4:
      printf("BREAk");
      exit(0);
      break;
    case 5:
      printf("printing in 2D:\n");
      t.print2D(dim);
      break;
    default:
      printf("Enter a valid Value");
    }
  }
}


// Function to print binary tree in 2D
// It does reverse inorder traversal
void tree::print2DUtil(struct node* root, int space, int dim)
{
    // Base case
    if (root == NULL)
        return;
 
    // Increase distance between levels
    space += COUNT;
 
    // Process right child first
    print2DUtil(root->right, space, dim);
 
    // Print current node after space
    // count
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf(" ");

    //printf("%d\n",root->data);
    // printing the array in the loop 
    
    int* ptr = root->data;
    for(int i = 0; i< dim ; i++) 
    {
    printf("[%d]",*ptr);
    ptr++;
    }
    // Process left child
    print2DUtil(root->left, space, dim);
}
//copy pasted too
// Wrapper over print2DUtil()
void tree::print2D(int dim)
{
    // Pass initial space count as 0
    struct node* temp = root;
    print2DUtil(temp, 0, dim);
}

//method to call the insert function
void tree::insert(int* num, int dim)
{
    root = insertRec(root, num, dim, 0);
}

//method to insert the newnode in the function
struct tree::node* tree::insertRec(struct node* temp,int* val, int dim, int counter)
{
  struct node* newnode;
  newnode=(struct node*)malloc(sizeof(struct node));
  int* newnode_ptr = newnode->data;

  // Tmp pointer to store the values of the data(to be added)
  int* tempval = val; 
  // Tmp pointer to store the values of current node.
  int* temp_ptr=temp->data;
  
  //copying the data from values array to newnodes data array.
  for(int i = 0; i< dim ; i++) 
  {
   *newnode_ptr=*tempval;
   tempval++;
   newnode_ptr++;
  }
  newnode_ptr = newnode->data;

  newnode->left=NULL;
  newnode->right=NULL;
  
  if(temp==NULL)
  {
    temp=newnode;
    return temp;
  }
  
  // Resetting the counter to 0 if it reaches the same dimension
  if(counter==dim)
    counter=0;

  if((*(temp_ptr+counter))<(*(newnode_ptr+counter)))
      temp->left = insertRec(temp->left,val, dim, counter+1);
  else
      temp->right = insertRec(temp->right, val, dim, counter+1);
  
  return temp;
}

//method to call the insert function
void tree::search(int* num, int dim)
{
    int temp = searchRec(root, num, dim, 0);
    if(temp==0)
      printf("Elements not found");
    else
      printf("Element found!");
}

int tree::searchRec(struct node *root, int* val, int dim, int counter)
{
    if (root == NULL)
    {
        printf("Element not found\n");
        return 0;
    }

    int flag = 1; //assuming its true
    int* root_ptr = root->data;
    int* tempval = val; //cause tempval is going to be incremented and u dont want to change that     
    //checking for the values from the value we need to find with the root 
    for(int i = 0; i< dim ; i++) 
    {
     if(*(root_ptr+i)!=*(tempval+i))
      {
        flag = 0;
      }
    }
    if (flag==1)
    {
        printf("Element found\n");
        return 1;
    }

    root_ptr = root->data;
    tempval = val;
    if(counter==dim)
      counter=0;

    if (*(root_ptr+counter)<*(tempval+counter))
    {
      return searchRec(root->left, val, dim, counter+1);
    }
    else
    {
    return searchRec(root->right, val, dim, counter+1);
    }
}

//method to call the delete function
void tree::deleteNode(int* num, int dim)
{
  deleteElement(NULL,root,num,dim,0);
}

void tree::deleteElement(struct node *parent, struct node *current, int* val, int dim, int counter)
{
    if (current == NULL)
    {
        return;
    } 

    int* current_ptr = current->data;
    int* tempval = val;
    int flag=1;
    for(int i = 0; i< dim ; i++) 
    {
     if(*(current_ptr+i)!=*(tempval+i))
        flag = 0;    //meaning its not true (not equal) now.
    }

    if (flag==1)
    {
        if (current->left == NULL && current->right == NULL)
        {
            if (parent == NULL)
            {
                root = NULL; // Update root if deleting the root node
            }
            else if (parent->left == current)
            {
                parent->left = NULL;
            }
            else
            {
                parent->right = NULL;
            }
            free(current);
            return;
        }
        if (current->left == NULL)
        {
            if (parent == NULL)
            {
                root = current->right; // Update root if deleting the root node
            }
            else if (parent->left == current)
            {
                parent->left = current->right;
            }
            else
            {
                parent->right = current->right;
            }
            free(current);
            return;
        }
        if (current->right == NULL)
        {
            if (parent == NULL)
            {
                root = current->left; // Update root if deleting the root node
            }
            else if (parent->left == current)
            {
                parent->left = current->left;
            }
            else
            {
                parent->right = current->left;
            }
            free(current);
            return;
        }
        struct node *temp = current->right;
        while (temp->left != NULL)
        {
            temp = temp->left;
        }
        int* temp_ptr = temp->data;
        current_ptr = current->data;
        //copying the data from values array to newnodes data array.
        for(int i = 0; i< dim ; i++) 
        {
          *current_ptr=*temp_ptr;
          temp_ptr++;
          current_ptr++;
        }
        deleteElement(current, current->right, temp->data, dim, counter+1);
        return;
    }

    current_ptr = current->data;
    if(counter==dim)
      counter=0;
    if (*(val+counter)<*(current_ptr+counter))
    {
        deleteElement(current, current->left, val, dim, counter+1);
    }
    else
    {
        deleteElement(current, current->right, val, dim, counter+1);
    }
}