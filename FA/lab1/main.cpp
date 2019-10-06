/**
 * @author Elekes Lukacs-Roland
 * @group 30424
 *
 * You are required to implement correctly and efficiently 3 direct sorting methods, and then to compare them
   (Bubble Sort,Insertion Sort – using either linear or binary insertion and Selection Sort)
   Input: sequence of numbers < 𝑎1, 𝑎2, … , 𝑎𝑛 >
   Output: an ordered permutation of the input sequence < 𝑎1′ ≤ 𝑎2′ ≤ ⋯ ≤ 𝑎𝑛′ >
 *
 * Both Bubble Sort and Insertion Sort have a complexity of O(n) in best case, but when it comes about
   Selection Sort it has a time complexity of O(n^2). Analyzing the results, in best case Bubble Sort
   does 0 assignments so it behaves in the best way after which comes Insertion Sort and then Selection Sort.
 *
 * In the average case each algorithm has a complexity of O(n^2), but if we consider only the assignments,
   Selection Sort has a complexity of O(n). If we analyze the results, we can observe then Bubble Sort is
   not as efficient as the other two algorithms. Both Insertion and Selection Sort seem to be a good choice.
 *
 * In worst case each algortihm has a complexity of O(n^2). Analyzing the numbers we can see that Bubble Sort
   does not behave so good, and we can observe that Selection Sort works in the best way in this case.
 *
 * In conclusion the best choice from the direct sorting methods is either Insertion or Selection Sort.
 */


//#include "pch.h"
#include <iostream>
#include "profiler.h"
//Define macros, where M is the number of repetitions needed when we are at average case
//MAX_sizeArray is the maximum size, when we do the analysis on different input size
#define M 5
#define MAX_sizeArray 10000

using namespace std;

//Declare global variables, for each algortihm two variables, one for the assignments, other for comparisons
int SELECTION_A = 0, SELECTION_C = 0, INSERTION_A = 0, INSERTION_C = 0, BUBBLE_A = 0, BUBBLE_C = 0;

Profiler profiler("sorting");

void BubbleSortAlg(int *v, int sizeArray) {
	bool swapped = true;

	do {
		swapped = false;
		for (int i = 0; i < sizeArray - 1; i++) {
			BUBBLE_C++;
			//profiler.countOperation("Bubble_comparisons", sizeArray);

			if (v[i] > v[i + 1]) {
				swap(*(v + i), *(v + i + 1));
				swapped = true;

				BUBBLE_A += 3;
				//profiler.countOperation("Bubble_assignments", sizeArray, 3);

			}
		}
	} while (swapped);
}

void InsertionSortAlg(int *v, int sizeArray) {
	int buff, i, j;
	for (i = 1; i < sizeArray; i++) {
		buff = v[i];
		INSERTION_A++;
		//profiler.countOperation("Insertion_assignments", sizeArray);
		j = i - 1;

		while (j >= 0 && v[j] > buff) {
			INSERTION_C++;
			//profiler.countOperation("Insertion_comparisons", sizeArray);
			v[j + 1] = v[j];
			//profiler.countOperation("Insertion_assignments", sizeArray);
			INSERTION_A++;
			j--;
		}
		INSERTION_C++;
		//profiler.countOperation("Insertion_comparisons", sizeArray);

		v[j + 1] = buff;
		INSERTION_A++;
		//profiler.countOperation("Insertion_assignments", sizeArray);

	}
}

void SelectionSortAlg(int *v, int sizeArray) {
	int indexOfMin, i, j;
	for (i = 0; i < sizeArray - 1; i++) {
		indexOfMin = i;

		for (j = i + 1; j < sizeArray; j++) {
			SELECTION_C++;
			//profiler.countOperation("Selection_comparisons", sizeArray);
			if (v[j] < v[indexOfMin]) {
				indexOfMin = j;
			}
		}
		swap(*(v + i), *(v + indexOfMin));
		SELECTION_A += 3;
		//profiler.countOperation("Selection_assignments", sizeArray, 3);

	}
}



int main()
{
	/*
	We declare two arrays with the maximum size, we need two, because we always want to
	have a copy of the original array as we want to apply the algortihms on the same input.
	*/
	int v[MAX_sizeArray], copyOfArray[MAX_sizeArray];




	//Best case, 100....10000 incrementing with 100, n means the size
	cout << "Best Case\n";

	for (int n = 100; n <= MAX_sizeArray; n += 100) {

		//Initialize the global variables
		BUBBLE_A = 0;
		BUBBLE_C = 0;
		INSERTION_A = 0;
		INSERTION_C = 0;
		SELECTION_A = 0;
		SELECTION_C = 0;

		//Fill the array with random numbers that are sorted ascending
		FillRandomArray(v, n, 10, 50000, false, 1);

		cout << n << endl;  //Just to visualize the current size

		//We copy the content of the original array v in the array named copyOfArray

		CopyArray(copyOfArray, v, n);
		BubbleSortAlg(copyOfArray, n);
		profiler.countOperation("Bubble_comparisons", n, BUBBLE_C);
		profiler.countOperation("Bubble_assignments", n, BUBBLE_A);
		profiler.countOperation("Bubble_comparisons1", n, BUBBLE_C);

		CopyArray(copyOfArray, v, n);
		InsertionSortAlg(copyOfArray, n);
		profiler.countOperation("Insertion_comparisons", n, INSERTION_C);
		profiler.countOperation("Insertion_assignments", n, INSERTION_A);
		profiler.countOperation("Insertion_comparisons1", n, INSERTION_C);

		CopyArray(copyOfArray, v, n);
		SelectionSortAlg(copyOfArray, n);
		profiler.countOperation("Selection_comparisons", n, SELECTION_C);
		profiler.countOperation("Selection_assignments", n, SELECTION_A);
	}

	//Create the series to be able to create the chart of total number of operations
	profiler.addSeries("Bubble_total", "Bubble_comparisons", "Bubble_assignments");
	profiler.addSeries("Insertion_total", "Insertion_assignments", "Insertion_comparisons");
	profiler.addSeries("Selection_total", "Selection_assignments", "Selection_comparisons");

	//Create the charts grouping the operations that we want to see on the same representation
	profiler.createGroup("Best_case_Comparisons", "Bubble_comparisons", "Insertion_comparisons", "Selection_comparisons");
	profiler.createGroup("Best_case_Assignments", "Insertion_assignments", "Selection_assignments");
	profiler.createGroup("Best_case_Total", "Bubble_total", "Insertion_total", "Selection_total");
	profiler.createGroup("Best_case_Total1", "Bubble_total", "Insertion_total");










	//We reset the profiler to have the counters on 0
	profiler.reset("average");

	//Average case, 100....10000 incrementing with 100, n means the size
	cout << "Average Case\n";

	for (int n = 100; n <= MAX_sizeArray; n += 100) {

		/*
		Initializing the global variables at each size. We repeat the measurement 5 times
		for each size of input, and the average of that will be put on the chart */
		BUBBLE_A = 0;
		BUBBLE_C = 0;
		INSERTION_A = 0;
		INSERTION_C = 0;
		SELECTION_A = 0;
		SELECTION_C = 0;

		cout << n << endl;

		for (int i = 0; i < M; i++) {

			//Fill the array with random numbers that are not ordered
			FillRandomArray(v, n, 10, 50000, false, 0);

			CopyArray(copyOfArray, v, n);
			BubbleSortAlg(copyOfArray, n);

			CopyArray(copyOfArray, v, n);
			InsertionSortAlg(copyOfArray, n);

			CopyArray(copyOfArray, v, n);
			SelectionSortAlg(copyOfArray, n);
		}

		//Calculate the average for each algortihm, which will be added to the chart
		BUBBLE_A /= M;
		BUBBLE_C /= M;
		profiler.countOperation("Bubble_comparisons", n, BUBBLE_C);
		profiler.countOperation("Bubble_assignments", n, BUBBLE_A);

		INSERTION_A /= M;
		INSERTION_C /= M;
		profiler.countOperation("Insertion_comparisons", n, INSERTION_C);
		profiler.countOperation("Insertion_assignments", n, INSERTION_A);

		SELECTION_A /= M;
		SELECTION_C /= M;
		profiler.countOperation("Selection_comparisons", n, SELECTION_C);
		profiler.countOperation("Selection_assignments", n, SELECTION_A);
		profiler.countOperation("Selection_assignments1", n, SELECTION_A);
	}

	profiler.addSeries("Insertion_total", "Insertion_assignments", "Insertion_comparisons");
	profiler.addSeries("Selection_total", "Selection_assignments", "Selection_comparisons");
	profiler.addSeries("Bubble_total", "Bubble_assignments", "Bubble_comparisons");

	profiler.createGroup("Average_case_Assignments", "Bubble_assignments", "Insertion_assignments", "Selection_assignments");
	profiler.createGroup("Average_case_Comparisons", "Bubble_comparisons", "Insertion_comparisons", "Selection_comparisons");
	profiler.createGroup("Average_case_total", "Insertion_total", "Selection_total");
	profiler.createGroup("Average_case_Total", "Bubble_total", "Insertion_total", "Selection_total");

	profiler.reset("worst");









	//Worst case, 100....10000 incrementing with 100, n means the size
	cout << "Worst case\n";

	for (int n = 100; n <= MAX_sizeArray; n += 100) {

		//Initialize the global variables
		BUBBLE_A = 0;
		BUBBLE_C = 0;
		INSERTION_A = 0;
		INSERTION_C = 0;
		SELECTION_A = 0;
		SELECTION_C = 0;

		//Fill the array with numbers that are ordered descending
		FillRandomArray(v, n, 10, 50000, false, 2);

		cout << n << endl;

		CopyArray(copyOfArray, v, n);
		BubbleSortAlg(copyOfArray, n);
		profiler.countOperation("Bubble_comparisons", n, BUBBLE_C);
		profiler.countOperation("Bubble_assignments", n, BUBBLE_A);

		CopyArray(copyOfArray, v, n);
		InsertionSortAlg(copyOfArray, n);
		profiler.countOperation("Insertion_comparisons", n, INSERTION_C);
		profiler.countOperation("Insertion_assignments", n, INSERTION_A);

		CopyArray(copyOfArray, v, n);
		SelectionSortAlg(copyOfArray, n);
		profiler.countOperation("Selection_comparisons", n, SELECTION_C);
		profiler.countOperation("Selection_assignments", n, SELECTION_A);
		profiler.countOperation("Selection_assignments1", n, SELECTION_A);
	}

	profiler.addSeries("Bubble_total", "Bubble_comparisons", "Bubble_assignments");
	profiler.addSeries("Insertion_total", "Insertion_assignments", "Insertion_comparisons");
	profiler.addSeries("Selection_total", "Selection_assignments", "Selection_comparisons");


	profiler.createGroup("Worst_case_Assignments", "Bubble_assignments", "Insertion_assignments", "Selection_assignments");
	profiler.createGroup("Worst_case_Comparisons", "Bubble_comparisons", "Insertion_comparisons", "Selection_comparisons");
	profiler.createGroup("Worst_case_Comparisons1", "Insertion_comparisons", "Selection_comparisons");
	profiler.createGroup("Worst_case_Total", "Bubble_total", "Insertion_total", "Selection_total");
	profiler.showReport();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
