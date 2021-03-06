/**
* @Elekes Lukacs-Roland
* @group 30424
*You are required to implement correctly and efficiently two methods for building a heap,
 namely the bottom­up and the top­down strategies. Compare and analyze them in average case.
 *In average case the number of comparisons are close to each other for the two algorithms,
  while the number of assignments of the bottom-up algorithm is half of top-down algorithm's.
  That makes that the number of total operations of top down is 1.5 times of bottom-up.
 *In worst case the difference between them is much bigger in favor of bottom-up algorithm

*/

#include "pch.h"
#include <iostream>
#include <limits>
#include "Profiler.h"
#define MAX_SIZE 10000
#define M 5

using namespace std;

Profiler profiler("Average_Case");

int B_U_A = 0, B_U_C = 0, T_D_A = 0, T_D_C = 0;


/*Utility functions*/
void initGlobalVar() {
	B_U_A = 0;
	B_U_C = 0;
	T_D_A = 0;
	T_D_C = 0;
}

//Creates two arrays, v1 is filled random, while v2 is the copy of v1
void createArrays(int sizeArray, int v1[], int v2[], int sorted) {
	FillRandomArray(v1, sizeArray, 10, 5000, false, sorted);
	CopyArray(v2, v1, sizeArray);
}


void createCharts(int sizeArray) {
	profiler.countOperation("Bottom-Up_Assignments", sizeArray, B_U_A);
	profiler.countOperation("Bottom-Up_Comparisons", sizeArray, B_U_C);
	profiler.countOperation("Top-Down_Assignments", sizeArray, T_D_A);
	profiler.countOperation("Top-Down_Comparisons", sizeArray, T_D_C);

	profiler.addSeries("Bottom-Up_Total", "Bottom-Up_Assignments", "Bottom-Up_Comparisons");
	profiler.addSeries("Top-Down_Total", "Top-Down_Assignments", "Top-Down_Comparisons");
}

/*Bottom-up*/

//returns the index of the maximum element among current element, right child, left child
int indexOfLargest(int v[], int sizeArray, int i, int indexLeft, int indexRight) {
	int largest;

	B_U_C++;
	if (indexLeft < sizeArray && v[indexLeft] > v[i]) {     //left child greatest
		largest = indexLeft;
	}
	else {
		largest = i;
	}

	B_U_C++;
	if (indexRight < sizeArray && v[indexRight] > v[largest]) {    //right child greatest
		largest = indexRight;
	}

	return largest;
}

//heapify assumes we have two heaps and by inserting a new element creates a new heap
void heapify(int v[], int sizeArray, int i) {
	int leftIndex = 2 * i + 1;
	int rightIndex = 2 * i + 2;
	int largest = indexOfLargest(v, sizeArray, i, leftIndex, rightIndex);    
	if (largest != i) {                                     //if largest found
		swap(*(v + i), *(v + largest));						//then swap the current with largest
		B_U_A += 3;
		heapify(v, sizeArray, largest);						//apply again to the current element which is now at position largest
	}
		

}

void buildHeap(int v[], int sizeArray) {
	for (int i = sizeArray / 2 - 1; i >= 0; i--) {          //leaves are considered one-element heaps, we start building from first non-leaf node
		heapify(v, sizeArray, i);
	}
}


/*Top-down*/

//gives the index of parent of a certain node
int parentIndex(int i) {
	if (i == 0) {
		return 0;
	}
	if (i % 2 == 1) {
		return i / 2;
	}
	else {
		return (i - 1) / 2;
	}
}

void insertHeap(int v[], int sizeArray, int value) {
	int i = sizeArray - 1;
	v[i] = value;										//new value inserted on the last position, as a leaf
	T_D_A++;

	while (i > 0 && v[parentIndex(i)] < v[i]) {			//if its value bigger than parent's, swap
		T_D_C++;
		swap(v[i], v[parentIndex(i)]);
		T_D_A += 3;
		i = parentIndex(i);
	}	
	T_D_C++;

}
 
//insert each element in the heap
void buildHeapTopDown(int v[], int size) {
	int sizeArray = 0;
	for (int i = 0; i < size; i++) {
		sizeArray++;
		insertHeap(v, sizeArray, v[i]);
	}
}


/*Average case, cases 100....10000 incrementing by 100*/
void averageCase() {
	int v[MAX_SIZE];
	int v2[MAX_SIZE];

	for (int sizeArray = 100; sizeArray <= MAX_SIZE; sizeArray += 100) {
		initGlobalVar();
		cout << sizeArray << endl;
		for (int k = 0; k < M; k++) {
			createArrays(sizeArray, v, v2, 0);
			buildHeap(v, sizeArray);
			buildHeapTopDown(v2, sizeArray);
		}
		B_U_A /= M;
		B_U_C /= M;
		T_D_A /= M;
		T_D_C /= M;

		createCharts(sizeArray);

		profiler.createGroup("Average_Case_Assignments", "Bottom-Up_Assignments", "Top-Down_Assignments");
		profiler.createGroup("Average_Case_Comparisons", "Bottom-Up_Comparisons", "Top-Down_Comparisons");
		profiler.createGroup("Average_Case_Total", "Bottom-Up_Total", "Top-Down_Total");

	}
	profiler.reset("worst");
}


//Worst case, cases 100....10000, incrementing by 100
void worstCase() {
	int v[MAX_SIZE];
	int v2[MAX_SIZE];

	for (int sizeArray = 100; sizeArray <= 10000; sizeArray += 100) {
		cout << sizeArray << endl;
		initGlobalVar();
		createArrays(sizeArray, v, v2, 1);
		buildHeap(v, sizeArray);
		buildHeapTopDown(v2, sizeArray);

		createCharts(sizeArray);

		profiler.createGroup("Worst_Case_Assignments", "Bottom-Up_Assignments", "Top-Down_Assignments");
		profiler.createGroup("Worst_Case_Comparisons", "Bottom-Up_Comparisons", "Top-Down_Comparisons");
		profiler.createGroup("Worst_Case_Total", "Bottom-Up_Total", "Top-Down_Total");
	}

	profiler.showReport();
}


int main()
{
	/*int v[10] = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7 };
	buildHeapTopDown(v, 10);
	for (int i = 0; i < 10; i++)
		cout << v[i] << ' ';
	
	
	cout << endl;
	
	
	int v2[10] = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7 };
	buildHeap(v2, 10);
	for (int i = 0; i < 10; i++)
		cout << v2[i] << ' ';*/

	averageCase();
	worstCase();
	return 0;
}




















// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
