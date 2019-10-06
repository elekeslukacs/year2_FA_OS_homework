/**
*@author Elekes Lukacs-Roland
*@group 30424
*You are required to implement correctly and efficiently the management operations of an order
statistics tree​ (chapter 14.1 from the book1).
You have to use a balanced, augmented Binary Search Tree. Each node in the tree holds, besides
the necessary information, also the size field (i.e. the size of the sub-tree rooted at the node).
* The complexity of OS_select is O(log n) and the complexity of OS_delete is also O(log n ).
The complexity of searching and deleting n elements is O(n*log n).
*/
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Profiler.h"
#include <time.h>

using namespace std;

Profiler profiler;

int os_operations = 0;

struct Node
{
    int value;
    int dim;
    Node* parent;
    Node* left;
    Node* right;
};

void fillArray (int v[], int n){
    for (int i=0; i<n; i++){
        v[i] = i+1;
    }
}

Node* build_PBT (int v[], int left, int right, Node* parent)
{
    if (left <= right)
    {
        int p = (left + right) / 2;
        Node *pNew=(Node*)malloc(sizeof(Node));
        os_operations += 3;
        pNew->value = v[p];
        pNew->parent = parent;
        pNew->dim = right - left + 1;
        pNew->left = build_PBT(v, left, p-1, pNew);
        pNew->right = build_PBT(v, p+1, right, pNew);
        return pNew;
    }
    else return NULL;
}

Node* OS_select (Node* root, int k)
{

    if (root == NULL)
    {
        return NULL;
    }

    int dimension;

    os_operations += 3;
    if(root->left == NULL)
    {
        dimension = 1;
    }
    else
    {
        dimension = root->left->dim + 1;
    }

    os_operations++;
    if ( dimension == k )
    {
        return root;
    }
    else if (dimension > k)
    {
        return OS_select(root->left, k);
    }
    else
    {
        return OS_select(root->right, k-dimension);
    }
}



Node* findMin (Node* root)
{
    os_operations++;
    if (root==NULL)
        return NULL;

    os_operations++;
    if (root->left!=NULL)
        return findMin(root->left);
    else return root;
}

void updateDim (Node* root){
    Node* temp = root;
    while (temp!=NULL){
        os_operations +=3;
        temp->dim--;
        temp = temp->parent;
    }
    os_operations++;
}

Node* OS_delete (Node* root, Node* deleted)
{
    if (root==NULL)
    {
        cout<<"error";
    }
    else if (deleted->value < root->value)
    {
        os_operations++;
        root->left = OS_delete(root->left, deleted);
    }
    else if (deleted->value > root->value)
    {
        os_operations++;
        root->right = OS_delete(root->right, deleted);
    }
    else
    {   /*Node has two subtrees*/
        if (root->left != NULL && root->right != NULL)
        {
            os_operations+=3;;
            Node* temp = findMin(root->right);
            root->value = temp->value;
            root->right = OS_delete(root->right, temp);
        }
        else
        {
           // Node* temp = root->parent;
           /*Node is a leaf*/
            if (root->right==NULL && root->left==NULL)
            {
                os_operations +=3;

                updateDim(root->parent);

                root = NULL;


            }
            /*Node has one subtree*/
            else if (root->left==NULL)
            {
                os_operations+=3;
                root->right->parent = root->parent;
                root = root->right;

                updateDim(root->parent);
            }
            else if (root->right==NULL)
            {
                os_operations+=3;
                root->left->parent = root->parent;
                root = root->left;

                updateDim(root->parent);
            }


        }
    }

    return root;
}


void showTreeIn(Node *root, int level)
{
    int i;
    if(root!=NULL)
    {
        showTreeIn(root->right, level+1);
        for(i=0; i<level; i++)
        {
            cout<<"         ";
        }
        cout<<root->value<<"("<<root->dim<<")"<<endl;
        showTreeIn(root->left, level+1);
    }
}


void avgCase (){
    Node* root;
    int v[10000];
    //int select[10000];
    Node* temp;
    for (int n = 100; n <= 10000; n+=100){
        os_operations = 0;
        for (int k = 0; k < 5; k++){
            fillArray(v, n);
            root = build_PBT(v, 0, n-1, NULL);

            int limit = n;

            for (int j = 0; j < n; j++){
                int random = rand()%limit + 1;
                temp = OS_select(root, random);
                root = OS_delete(root, temp);
                limit--;
            }
        }
        os_operations/=5;
        profiler.countOperation("OS_operations", n, os_operations);
    }
    profiler.reset();
}


int main()
{
    Node *pRoot;
    int v[11] = {1, 2, 3, 4, 5, 8, 10, 12, 14, 15,18};

    pRoot = build_PBT(v, 0, 10, NULL);
    showTreeIn(pRoot, 0);
    cout<<"\n\n\n\n";
    Node * test;

    test = OS_select(pRoot, 2);
    pRoot = OS_delete(pRoot, test);
    showTreeIn(pRoot, 0);
    cout<<"\n\n\n\n";

    test = OS_select(pRoot, 3);
    pRoot = OS_delete(pRoot, test);
    showTreeIn(pRoot, 0);
    cout<<"\n\n\n\n";

    test = OS_select(pRoot, 7);
    pRoot = OS_delete(pRoot, test);

    showTreeIn(pRoot, 0);
    cout<<"\n\n\n\n";

    //avgCase();
    return 0;
}
