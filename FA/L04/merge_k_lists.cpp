/**
*@author Elekes Lukacs-Roland
*@group 30424
*
*You are required to implement correctly and efficiently an O(nlogk) method for merging k
*sorted sequences​, where n is the total number of elements. (Hint: use a heap, see seminar no. 2
*notes).
*Implementation requirements:
*Use linked lists to represent the k sorted sequences and the output sequence
*
*When the value of k (number of lists) is fixed, we can observe that the complexity becomes O(n),
*as k becomes a constant.
*In the other case, when n is fixed, it becomes logarithmic, the complexity is O(logn), as n is a
*constant.
*/

#include <iostream>
#include <stdlib.h>
#include "Profiler.h"

using namespace std;

Profiler profiler("Average_case_k");

int mergeAssign = 0, mergeComp = 0;

//Function for initializing global variables

void initGlobalVar()
{
    mergeAssign = 0;
    mergeComp = 0;
}

struct Node
{
    int value;
    Node * next;
};

//insertNode at the end of the list
//This function is not so efficient as it has to traverse the whole list to reach the last element
//A more efficient method would be inserting on the beginning, and we would generate values in
//descending order to put them in the list
Node* insertNode(Node* head, int data)
{
    if (head == NULL)
    {
        head = new Node();

        head->value = data;
        head->next = NULL;

        return head;
    }
    else
    {
        Node* pWalker = head;
        while (pWalker->next != NULL)
        {
            pWalker = pWalker->next;
        }

        Node* newNode = new Node();

        newNode->value = data;
        newNode->next = NULL;
        (pWalker->next) = newNode;

        return head;
    }
}

//function for printing the list
void showList(struct Node* head)
{
    while (head != NULL)
    {
        cout << head->value << " ";
        head = head->next;
    }
}



/*Build Heap*/

//When we create the heap, we will also have to store the index of the list from which an element is taken
//For this I used a two dimensional array, which stores on the column 0 the key, on column 1 the index

int indexOfSmallest(int v[][2], int sizeArray, int i, int indexLeft, int indexRight)
{
    int smallest;

    mergeComp++;

    if (indexLeft < sizeArray && v[indexLeft][0] < v[i][0])       //left child smallest
    {
        smallest = indexLeft;
    }
    else
    {
        smallest = i;
    }

    mergeComp++;

    if (indexRight < sizeArray && v[indexRight][0] < v[smallest][0])      //right child smallest
    {
        smallest = indexRight;
    }

    return smallest;
}

void heapify(int v[][2], int sizeArray, int i)
{
    int leftIndex = 2 * i + 1;
    int rightIndex = 2 * i + 2;
    int smallest = indexOfSmallest(v, sizeArray, i, leftIndex, rightIndex);
    if (smallest != i)                                       //if smallest found
    {
        swap(v[i][0], v[smallest][0]);
        mergeAssign+=3;
        swap(v[i][1], v[smallest][1]);						//then swap the current with smallest
        heapify(v, sizeArray, smallest);						//apply again to the current element which is now at position smallest
    }
}

void buildHeap(int v[][2], int sizeArray)
{
    for (int i = sizeArray / 2 - 1; i >= 0; i--)            //leaves are considered one-element heaps, we start building from first non-leaf node
    {
        heapify(v, sizeArray, i);
    }
}

/*Generate Lists and Merge Lists*/

//Function for generating the lists required. If the elements can not be distributed equally to the lists
//we have to take distribute them one by one to lists, so there will be n modulo k elements to be distributed
//thus n modulo k lists will have n/k + 1 elements
void generateLists(Node * arr[], int k, int n, int limit1, int limit2)
{
//    for (int i = 0; i<k; i++)
//    {
//        arr[i]=NULL;
//    }

    int nodes = n / k;
    int rest = n % k;

    for(int i=0; i<k; i++)
    {
        arr[i]=NULL;
        if(rest == 0)
        {
            int *v = (int*)malloc(nodes*sizeof(int));
            FillRandomArray(v, nodes, limit1, limit2, false, 1);

            for(int j=0; j<nodes; j++)
            {
                arr[i] = insertNode(arr[i], v[j]);
            }
        }
        else
        {
            int *v = (int*)malloc((nodes+1)*sizeof(int));
            FillRandomArray(v, nodes + 1, limit1, limit2, false, 1);
            rest--;

            for(int j=0; j<nodes + 1; j++)
            {
                arr[i] = insertNode(arr[i], v[j]);
            }
        }
    }


}


void mergekList(Node** head, Node* arr[], int k, int n)
{
    //we copy the first elements' value from the lists, and then we build the heap
    int heapArr[k][2];
    for(int i=0; i<k; i++)
    {
        heapArr[i][0] = arr[i]->value;
        mergeAssign++;
        heapArr[i][1] = i;
    }


    buildHeap(heapArr, k);

    int heapSize = k;


//    for (int i=0; i<heapSize; ++i){
//        cout<<heapArr[i][0]<<' '<<heapArr[i][1]<<endl;
//    }

//we know exactly, that n steps we need to merge the lists
    for(int i = 0; i<n; ++i)
    {
        //insert the smallest element of the heap,which is the smallest element always
        *head = insertNode(*head, heapArr[0][0]);
        mergeAssign++;

        //move the head pointer for the list from which we took out the element
        arr[heapArr[0][1]] = arr[heapArr[0][1]]->next;

        mergeComp++;

        //the new element is taken from the list if possible than we apply heapify
        //else we put the last element of the heap to the first position

        if(arr[heapArr[0][1]] != NULL)
        {
            heapArr[0][0] = arr[heapArr[0][1]]->value;
            mergeAssign++;

            heapify(heapArr, heapSize, 0);
        }
        else
        {
            heapArr[0][0] = heapArr[heapSize-1][0];
            mergeAssign++;
            heapArr[0][1] = heapArr[heapSize-1][1];

            heapSize--;
            heapify(heapArr, heapSize, 0);
        }
    }
    //showList(head);
}

void caseFixedK ()
{
    int k1 = 5;
    int k2 = 10;
    int k3 = 100;
    Node* arr1[100];


    for (int n = 100; n<=10000; n+=100)
    {
        initGlobalVar();
        //Node* arr1[5];
        Node* head1 = NULL;
        generateLists(arr1, k1, n, 10, 50000);
        mergekList(&head1, arr1, k1, n);
        profiler.countOperation("k1_total", n, mergeAssign + mergeComp);



        initGlobalVar();
        //Node* arr2[10];
        Node* head2 = NULL;
        generateLists(arr1, k2, n, 10, 50000);
        mergekList(&head2, arr1, k2, n);
        profiler.countOperation("k2_total", n, mergeAssign + mergeComp);



        initGlobalVar();
        //Node* arr3[100];
        Node* head3 = NULL;
        generateLists(arr1, k3, n, 10, 50000);
        mergekList(&head3, arr1, k3, n);
        profiler.countOperation("k3_total", n, mergeAssign + mergeComp);


        profiler.createGroup("k_fixed", "k1_total","k2_total", "k3_total");


    }

    int n = 10000;
    Node* arr[500];


    for (int k = 10; k<=500; k+=10)
    {
        initGlobalVar();
        //Node* arr[k];
        Node* head = NULL;
        generateLists(arr, k, n, 10, 50000);
        mergekList(&head, arr, k, n);
        profiler.countOperation("n", k, mergeAssign + mergeComp);
        profiler.createGroup("n_fixed", "n");
    }
    profiler.showReport();
}

int main()
{
    int k = 3;
    int n = 16;

    Node* arr[3];

    generateLists(arr, k, n, 10, 50);

    for (int i=0; i<k; i++)
    {
        showList(arr[i]);
        cout<<endl;
    }
    Node* head = NULL;

    mergekList(&head, arr, k, n);

    cout<<endl;

    showList(head);

    // cout<<endl<<mergeAssign + mergeComp;

    //caseFixedK();
    return 0;
}
