/**
*@author Elekes Lukacs-Roland
*@group 30424
*
*You are required to implement correctly and efficiently linear time transformations between
three different representations for a multi-way tree:
R1: parent representation: for each key you are given the parent key, in a vector.
R2: multi-way tree representation: for each node you have the key and a vector of
children nodes
R3: binary tree representation: for each node, you have the key, and two pointers: one to
the first child node, and one to the brother on the right (i.e. the next brother node)
*
*The complexity of T1 is O(n) and T2 has also a complexity of O(n). There is a necessity of extra space
as we use the stack at the recursive calls.
*/
#include <iostream>
#include <stdlib.h>

using namespace std;

struct NodeV2
{
    int x;
    int dim;

    NodeV2 *children[20];

};

struct NodeV3
{
    int x;
    NodeV3* left;
    NodeV3* right;
};

void initV3(NodeV3 * node)
{
    node->left = node->right = NULL;
}

void insertKey (NodeV2 * current[], int key, int i)
{
    if (key == -1)
    {
        return;
    }

    int currentChild = current[key]->dim;
    current[key]->dim++;
    current[key]->children[currentChild] = current[i];

}

void T1 (int v[], int n, NodeV2* nodes[], NodeV2** root)
{

    for (int i = 1; i < n+1; i++)
    {
        nodes[i] = (NodeV2*)malloc(sizeof(NodeV2));
        nodes[i]->dim = 0;
        nodes[i]->x = i;
    }

    for (int i = 1; i < n; i++)
    {
        if (v[i] == -1)
        {
            *root = nodes[i];
        }
        else
        {
            insertKey(nodes, v[i], i);
        }
    }
}

//void T2 (NodeV3* &destination, NodeV2* &source){
//    destination->x = source->x;
//    //cout<<destination->x<<endl;
//    //destination->left = destination->right = NULL;
//    //cout<<source->dim<<endl;
//    if(source->dim == 0)
//    {
//        //destination->left = destination->right = NULL;
//        return;
//    }
//    if(source->dim >= 1){
//       // (*destination)->left->x = source->children[0]->x;
//       cout<<"inserting left "<<source->children[0]->x<<endl;
//       destination->left = (NodeV3*)malloc(sizeof(NodeV3));
//        T2(destination->left, source->children[0]);
//    }
//    else {
//        destination->left = NULL;
//    }
//
//    int i = 1;
//    //NodeV3 * &temp = destination->left->right;
//    while (i < source->dim){
//        cout<<"inserting right "<<source->children[i]->x<<" to ";
//        cout << destination->left->x<<endl;
////        temp = (NodeV3*)malloc(sizeof(NodeV3));
//        destination->left->right = (NodeV3*)malloc(sizeof(NodeV3));
//        T2(destination->left->right, source->children[i]);
//        cout<<destination->left->right->x<<endl;
//        i++;
//        //destination->left->right = destination->left->right->right;
//
//    }
//
//    destination->right = NULL;
//
//}




void add_children (NodeV3* v3, NodeV2* v2)
{

    initV3(v3);
    if(v3 == NULL)
    {
        v3 = (NodeV3*)malloc(sizeof(NodeV3));
    }
        if(v2->dim > 0)
        {
            v3->left = (NodeV3*)malloc(sizeof(NodeV3));

            v3->left->x = v2->children[0]->x;

            add_children(v3->left, v2->children[0]);
        }

    NodeV3 *current = v3->left;

    for (int i = 1; i < v2->dim; i++)
    {

        current->right = (NodeV3*)malloc(sizeof(NodeV3));
        int aux = v2->children[i]->x;
        current->right->x = aux;


        add_children(current->right, v2->children[i]);
        current = current->right;
    }
    //current->right = NULL;

}



NodeV3* T2 ( NodeV2 * root)
{
    NodeV3 * current = (NodeV3*)malloc(sizeof(NodeV3));
    current->x = root->x;
    //cout<<current->x<<endl;
    add_children(current, root);
    return current;
}

void printTree(NodeV2* root)
{
    cout << "node " << root->x << " has children: ";
    for (int i = 0; i < root->dim; i++)
    {
        cout << root->children[i]->x << " ";
    }
    cout << endl;
    for (int i = 0; i < root->dim; i++)
    {
        if (root->children[i]->dim != 0)
        {
            printTree(root->children[i]);
        }
    }
}

void showTreeIn(NodeV3 *root, int level)
{
    int i;
    if(root!=NULL)
    {
        showTreeIn(root->right, level+1);
        for(i=0; i<level; i++)
        {
            cout<<"         ";
        }
        cout<<root->x<<endl;
        showTreeIn(root->left, level+1);
    }
}

void printT2(NodeV3 *root, int level)
{
	for (int i=0; i<level; i++)
    {
		cout<<"  ";
	}
    cout<<root->x<<endl;

	if (root->left != NULL)
		{
		    printT2(root->left, level + 1);
		}
	if (root->right != NULL)
        {
		    printT2(root->right, level);
		}
}

int main()
{
    int arr[10] = {0,2, 7, 5, 2,  7, 7, -1, 5, 2};
    NodeV2* nods[10];
    NodeV2* root;
    T1(arr, 10, nods, &root);
    printTree(root);
    NodeV3* root2 = (NodeV3*)malloc(sizeof(NodeV3));


    root2 = T2(root);
    //printT2(root2, 0);
    showTreeIn(root2, 0);



    return 0;
}
