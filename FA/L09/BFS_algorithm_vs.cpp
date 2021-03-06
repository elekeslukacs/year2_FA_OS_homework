/**
*@author Elekes Lukacs-Roland
*@group 30424
*
* You are required to implement correctly and efficiently the Breadth-First Search (BFS) graph algorithm
(Section 22.2 from the book1). For graph representation, you should useadjacency lists.
*
* The complexity of the BFS algorithm is O(V+E), where V marks the number of the vertices and E the
number of the edges, and E is V multiplied with a constant, so we can say that the complexity is O(n) linear.
When V is fixed, complexity is O(E) and when E is fixed, complexity is O(V).
*/
#include "pch.h"
#include <iostream>
#include <stdlib.h>
#define MAX 200
#define CONST_V 100
#define CONST_E 4500

#include "Profiler.h"
using namespace std;

Profiler profiler("Fixed _number_of_nodes");

int ops = 0;

struct Queue {
	int elements[MAX];
	int front, rear;
};

Queue* createQueue() {
	Queue* q = (Queue*)malloc(sizeof(Queue));
	q->front = -1;
	q->rear = -1;
	return q;
}

bool queueEmpty(Queue* q) {
	if (q->rear == -1 || q->front > q->rear)
		return true;
	else return false;
}

void enqueue(Queue* q, int item) {
	if (q->rear + 1 == MAX) {
		cout << "error";
		return;
	}
	else {
		if (q->front == -1) {
			q->front = 0;
		}
		q->rear++;
		q->elements[q->rear] = item;
	}
}

int dequeue(Queue* q) {
	int item;
	if (queueEmpty(q)) {
		cout << "Queue emptynow";
		return -1;
	}
	else {
		item = q->elements[q->front];
		q->front++;
		//cout << q->front << endl;
		if (q->front > q->rear) {
			//reset
			q->front = -1;
			q->rear = -1;
		}
	}
	return item;
}

struct Node {
	int value;
	//bool visited;
	Node * next;
};

Node* createNode(int v) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->value = v;
	//newNode->visited = false;
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



void BFS_comp(Graph * g, int start, bool visited[]) {

	Queue * Q = createQueue();
	enqueue(Q, start);
	visited[start] = true;
	ops++;

	while (!queueEmpty(Q)) {
		ops++;
		int current = dequeue(Q);
		cout << "visited " << current << endl;

		Node* temp = g->adjLists[current];
		//ops++;
		while (temp != NULL) {
			ops += 2;
			if (visited[temp->value] == false) {

				visited[temp->value] = true;
				enqueue(Q, temp->value);
				//ops++;
			}
			temp = temp->next;
			//ops++;
		}
		ops++;
	}
	ops++;
}


void BFS(Graph* g, int start) {
	int size = g->V;
	bool *visited = (bool*)malloc(size * sizeof(bool));
	for (int i = 0; i < size; i++) {
		visited[i] = false;
	}

	BFS_comp(g, start, visited);

	//check if we have nodes that were not discovered, if so, it means that there is another connected component
	for (int i = 0; i < size; i++) {

		//ops++;
		if (visited[i] == false) {
			cout << "next component" << endl;
			BFS_comp(g, i, visited);
		}
	}


}

void fixedNodes() {
	for (int i = 1000; i < 4501; i += 100) {
		cout << i << endl;
		ops = 0;
		Graph* g = createGraph(CONST_V);
		int j = 0;
		while (j < i) {
			//srand(time(NULL));
			int u = rand() % (CONST_V); //values 0-99
			int v = rand() % (CONST_V);

			if (addEdge(g, u, v)) {
				j++;
			}

		}
		BFS(g, 0);
		profiler.countOperation("Fixed_V_operations", i, ops);

	}
	profiler.reset("Fixed_number_of_edges");

}

void fixedEdges() {
	for (int i = 100; i < 201; i += 10) {
		cout << i << endl;
		ops = 0;
		Graph* g = createGraph(i);
		int j = 0;
		while (j < CONST_E) {

			int u = rand() % (i); //values 0-99
			int v = rand() % (i);
			if (addEdge(g, u, v)) {
				j++;
			}
		}
		BFS(g, 0);
		profiler.countOperation("Fixed_E_operations", i, ops);
		free(g);
	}
	profiler.reset();
}

int main()
{

	//Testing the implementation of the graph

	Graph* graph = createGraph(10);
	addEdge(graph, 0, 1);
	addEdge(graph, 0, 4);
	addEdge(graph, 1, 2);
	addEdge(graph, 1, 3);
	addEdge(graph, 5, 6);
	addEdge(graph, 5, 7);
	addEdge(graph, 6, 8);
	addEdge(graph, 8, 9);
	cout << graph->adjLists[0]->value << endl;

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
	cout << endl;

	BFS(graph, 0);

    /*Number of nodes is fixed*/
	//fixedNodes();

	/*Number of edges is fixed*/
	//fixedEdges();

	return 0;
}
