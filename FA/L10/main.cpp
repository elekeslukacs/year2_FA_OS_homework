/**
*@author Elekes Lukacs-Roland
*@group 30424
*You are required to implement correctly and efficiently the depth­first search
algorithm (DFS) (Chapter 22.3 from the book). For graph representation, you should use
adjacency lists. You also have to:
    ­ Implement the Tarjan algorithm for detecting strongly connected components
    (https://en.wikipedia.org/wiki/Tarjan's_strongly_connected_components_algorithm)
    ­ Implement topological sorting (described in chapter 22.4).
*The time complexity of the DFS algorithm is O(V+E), where V is the number of vertices
in the graph and E is the number of edges.
*The complexity of the Topological Sorting algorithm is O(V+E).
*The complexity of Tarjan's algorithm for strongly connected components is O(V+E);
*/
#include "Profiler.h"
#include <iostream>
#include <stdlib.h>
#define MAX 200
#define CONST_V 100
#define CONST_E 4500


using namespace std;

Profiler profiler("Fixed_nb_of_nodes");

int ops = 0;

struct Stack {
	int top;
	int elements[MAX];
};

Stack* createStack() {
	Stack * s = (Stack*)malloc(sizeof(Stack));
	s->top = -1;
	return s;
}

bool stackEmpty(Stack * s) {
	if (s->top == -1)
		return true;
	else return false;
}

void push(Stack* s, int item) {
	s->top++;
	s->elements[s->top] = item;
}

int pop(Stack* s) {
	int item;
	if (stackEmpty(s)) {
		cout << "stack empty\n";
		return -1;
	}
	else {
		item = s->elements[s->top];
		s->top--;
	}
	return item;
}

int top(Stack* s) {

	if (stackEmpty(s)) {
		cout << "Stack empty\n";
		return -1;
	}
	else return s->elements[s->top];
}

struct Node {
	int value;
	Node * next;
};

Node* createNode(int v) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->value = v;
	newNode->next = NULL;
	return newNode;
}

struct Graph {
	int V;
	Node** adjLists;
};

Graph*  createGraph(int nrOfVertices) {
	Graph * g = (Graph*)malloc(sizeof(Graph));
	g->V = nrOfVertices;
	g->adjLists = (Node**)malloc(nrOfVertices * sizeof(Node));
	//initialize the headers for the adjacency lists
	for (int i = 0; i < nrOfVertices; i++) {
		g->adjLists[i] = NULL;
	}
	return g;
}

bool addEdge(Graph* g, int u, int v) {
	//check if the edge exists

	Node* temp = g->adjLists[u];
	bool exists = false;
	while (temp != NULL) {
		if (temp->value == v) {
			exists = true;
			break;
		}
		else temp = temp->next;
	}

	//if doesn't exist, add the edge (u---v) and also (v---u) because the graph is undirected
	if (!exists) {
		Node* newNode = createNode(v);
		newNode->next = g->adjLists[u];
		g->adjLists[u] = newNode;

		newNode = createNode(u);
		newNode->next = g->adjLists[v];
		g->adjLists[v] = newNode;

		return true;
	}
	else return false;
	//else cout << "edge already exists";
}

bool addEdgeDirected(Graph* g, int u, int v) {
	//check if the edge exists

	Node* temp = g->adjLists[u];
	bool exists = false;
	while (temp != NULL) {
		if (temp->value == v) {
			exists = true;
			break;
		}
		else temp = temp->next;
	}

	//if doesn't exist, add the edge (u---v) and also (v---u) because the graph is undirected
	if (!exists) {
		Node* newNode = createNode(v);
		newNode->next = g->adjLists[u];
		g->adjLists[u] = newNode;

		/*newNode = createNode(u);
		newNode->next = g->adjLists[v];
		g->adjLists[v] = newNode;*/

		return true;
	}
	else return false;
	//else cout << "edge already exists";
}

void DFS_comp(Graph* g, int i, bool visited[]) {
	visited[i] = true;
	cout << "visited " << i << endl;
	Node* temp = g->adjLists[i];
	int neigh;
	while (temp != NULL) {
        ops++;
		neigh = temp->value;
        ops++;
		if (visited[neigh] == false) {
			DFS_comp(g, neigh, visited);
		}
		temp = temp->next;
		ops++;
	}
	ops++;
	//cout << "visited" << i << endl;
}

void DFS(Graph* g) {
	int size = g->V;
	bool *visited = (bool*)malloc(size * sizeof(bool));
	for (int i = 0; i < size; i++) {
		visited[i] = false;
	}

	for (int i = 0; i < size; i++) {
        ops++;
		if (visited[i] == false) {
			DFS_comp(g, i, visited);
		}
	}
}

void topSort_comp(Graph* g, int i, bool visited[], Stack* s) {
	visited[i] = true;
	Node* temp = g->adjLists[i];
	int neigh;
	while (temp != NULL) {
		neigh = temp->value;
		if (visited[neigh] == false) {
			topSort_comp(g, neigh, visited, s);
		}
		temp = temp->next;
	}
	push(s, i);
}

void topSort(Graph* g) {

	int size = g->V;
	bool *visited = (bool*)malloc(size * sizeof(bool));
	for (int i = 0; i < size; i++) {
		visited[i] = false;
	}

	Stack* s = createStack();

	for (int i = 0; i < size; i++) {
		if (visited[i] == false) {
			topSort_comp(g, i, visited, s);
		}

	}


	int a;
	while (!stackEmpty(s)) {
		a = pop(s);
		cout << a << " ---> ";
	}


}

int min(int a, int b) {
	if (a < b)
		return a;
	else return b;
}

/*template<typename T>
void initArray(T *arr, int size, T initValue) {
	*arr = (T*)malloc(size * sizeof(T));
	for (int i = 0; i < size; i++) {
		arr[i] = initValue;
	}
}*/

void SCC_visit(Graph* g, int node, int disc_time[], int low_link[], bool on_stack[], Stack* s) {
	static int step = 0;
	disc_time[node] = low_link[node] = ++step;
	on_stack[node] = true;
	push(s, node);
	Node* temp = g->adjLists[node];
	int neigh;


	while (temp != NULL) {
		neigh = temp->value;
		if (disc_time[neigh] == -1) {
			SCC_visit(g, neigh, disc_time, low_link, on_stack, s);
			low_link[node] = min(low_link[node], low_link[neigh]);
		}
		else if (on_stack[neigh]) {
			low_link[node] = min(low_link[node], disc_time[neigh]);
		}
		temp = temp->next;
	}


	int aux;
	if (low_link[node] == disc_time[node]) {
		cout << "Start of SCC\n";
		while (top(s) != node) {
			aux = top(s);
			cout << aux << "--->";
			on_stack[aux] = false;
			pop(s);
		}

		aux = top(s);
		cout << aux << endl;
		on_stack[aux] = false;
		pop(s);
	}

}

void Tarzan_SCC(Graph* g) {
	int size = g->V;
	int *disc_time = (int*)malloc(size * sizeof(int));
	int *low_link = (int*)malloc(size * sizeof(int));
	bool *on_stack = (bool*)malloc(size * sizeof(bool));
	Stack* s = createStack();

	for (int i = 0; i < size; i++) {
		disc_time[i] = low_link[i] = -1;
		on_stack[i] = false;
	}

	for (int i = 0; i < size; i++) {
		if (disc_time[i] == -1) {
			SCC_visit(g, i, disc_time, low_link, on_stack, s);
		}
	}

}


void fixedNodes() {
	for (int i = 1000; i < 4501; i += 100) {
		cout << i << endl;
		ops = 0;
		for (int k = 0; k < 5; k++) {
			Graph* g = createGraph(CONST_V);
			int j = 0;
			while (j < i) {

				int u = rand() % (CONST_V); //values 0-99
				int v = rand() % (CONST_V);

				if (addEdge(g, u, v)) {
					j++;
				}

			}
			DFS(g);
			free(g);
		}
		ops /= 5;
		profiler.countOperation("Fixed_V_operations", i, ops);

	}
	profiler.reset("Fixed_number_of_edges");

}


void fixedEdges() {
	for (int i = 100; i < 201; i += 10) {
		cout << i << endl;
		ops = 0;
		for (int k = 0; k < 5; k++) {
			Graph* g = createGraph(i);
			int j = 0;
			while (j < CONST_E) {

				int u = rand() % (i); //values 0-99
				int v = rand() % (i);
				if (addEdge(g, u, v)) {
					j++;
				}
			}
			DFS(g);
			free(g);
		}
		ops /= 5;
		profiler.countOperation("Fixed_E_operations", i, ops);

	}
	profiler.reset();
}

void printGraph(Graph * graph){
    for (int i = 0; i < graph->V; i++) {
		Node* temp = graph->adjLists[i];
		printf("\n Adjacency list of vertex %d\n ", i);
		while (temp)
		{
			printf("%d -> ", temp->value);
			temp = temp->next;
		}
		printf("\n");
	}
}

int main()
{
    Graph* graph1 = createGraph(10);
	addEdge(graph1, 0, 2);
	addEdge(graph1, 0, 1);
	addEdge(graph1, 1, 3);
	addEdge(graph1, 3, 4);
	addEdge(graph1, 5, 6);
	addEdge(graph1, 6, 8);
	addEdge(graph1, 8, 9);
	addEdge(graph1, 9, 7);
	//printGraph(graph1);
	cout<<"\n\nStart DFS:\n";
    DFS(graph1);
    cout<<"\n\n\n";

    Graph* graph2 = createGraph(5);
  	addEdgeDirected(graph2, 0, 1);
	addEdgeDirected(graph2, 0, 2);
	addEdgeDirected(graph2, 1, 3);
	addEdgeDirected(graph2, 1, 2);
	addEdgeDirected(graph2, 2, 4);
	addEdgeDirected(graph2, 2, 3);
	//printGraph(graph2);
	cout<<"\n\nStart Topological Sorting:\n";
	topSort(graph2);
	cout<<"\n\n";

	Graph* graph = createGraph(5);
	addEdgeDirected(graph, 0, 1);
	addEdgeDirected(graph, 1, 3);
	addEdgeDirected(graph, 1, 2);
	addEdgeDirected(graph, 2, 0);
	addEdgeDirected(graph, 3, 4);
	cout<<"\n\nStart Tarzan:\n";
    //printGraph(graph);
	Tarzan_SCC(graph);



//	fixedNodes();

//	fixedEdges();

}
