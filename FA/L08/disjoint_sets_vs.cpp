/**
*@author Elekes Lukacs-Roland
*@group 30424
*You are required to implement correctly and efficiently the base operations for disjoint set
(chapter 21.1 from book ) and the Kruskal’s algorithm (searching for the minimum spanning 1
tree - chapter 23.2) using disjoint sets.
You have to use a tree as the representation of a disjoint set. Each tree holds, besides the
necessary information, also the rank field (i.e. the height of the tree).* The complexity of Kruskal's algorithm is of O(nlogn), MAKE_SET is of O(1), while FIND_SET can be
O(n) in worst case.
*/
#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include "Profiler.h"

using namespace std;

Profiler profiler("Disjoint_Sets");

int ops = 0;

struct Node {
	int key;
	int rank;
	Node* parent;
};

struct Edge {
	int weight;
	Node* s;
	Node* d;
};

struct Graph {
	int V;
	int E;
	Edge *edges;
	Node **nodes;
};

Graph *createGraph(int V, int E) {
	Graph* newGraph = new Graph;
	newGraph->E = E;
	newGraph->V = V;
	newGraph->nodes = (Node**)malloc(V * sizeof(Node));
	newGraph->edges = (Edge*)malloc(E * sizeof(Edge));
	return newGraph;
}

/*void initEdges(Edge* edges[], int size) {
	for (int i = 0; i < size; i++) {
		edges[i] = new Edge;
	}
}*/
Node* make_set(int x) {
	Node* p = new Node;
	p->key = x;
	p->rank = 0;
	p->parent = p;
	ops = ops + 3;
	return p;
}

Node* find_set(Node* x) {
	ops++;
	if (x != x->parent) {
		ops++;
		x->parent = find_set(x->parent);
	}
	return x->parent;
}

void link(Node *x, Node *y) {
	ops++;
	if (x->rank > y->rank) {
		y->parent = x;
		ops++;
	}
	else {
		x->parent = y;
		ops += 2;
		if (x->rank == y->rank) {
			ops++;
			y->rank++;
		}
	}
}

void unite(Node *x, Node *y) {
	link(find_set(x), find_set(y));
}

int partition(Edge v[], int start, int end) {
	Edge x = v[end]; 
	//ops++;	
	int index = start - 1;
	for (int i = start; i < end; i++) {
		//ops++;
		if (v[i].weight < x.weight) {
			index++;
			swap(*(v + i), *(v + index));
			//ops += 3;
		}
	}
	swap(*(v + end), *(v + index + 1));
	//ops += 3;
	return index + 1;
}

void quickSort(Edge v[], int start, int end) {
	if (start < end) {
		int p = partition(v, start, end);
		quickSort(v, start, p - 1);
		quickSort(v, p + 1, end);
	}
}




/*void Kruskal(Node * nodes[], int numberOfNodes, Edge * edges[], int numberOfEdges, Edge *mst_edges[]) {
	int mstCurrent = 0;
	Node *tempNodes[10000];
	for (int i = 0; i < numberOfNodes; i++) {
		tempNodes[i] = new Node;
		tempNodes[i] = make_set(nodes[i]->key);
	}
	quickSort(*edges, 0, numberOfEdges - 1);

	for (int i = 0; i < numberOfNodes; i++) {
		Node* source = find_set(edges[i]->s);
		Node* dest = find_set(edges[i]->d);

		if (source != dest) {
			mst_edges[mstCurrent] = edges[i];
		}
	}
}*/
;

/*void Kruskal(Graph * g) {
	mSize = 0;
	int V = g->V;
	Edge ** mst = (Edge**)malloc(V * sizeof(Edge));
	int current = 0;
	int v = 0;
	quickSort(g->edges, 0, g->E - 1);

	Node ** nodes = (Node**)malloc(V * sizeof(Node));

	for (int i = 0; i < V; i++) {
		nodes[i] = make_set(i);
	}

	while (current < V - 1) {
		Edge *next = &g->edges[v];
		v++;

		Node *source = find_set(next->s);
		Node *dest = find_set(next->d);

		if (source != dest) {
			mst[current] = next;
			current++;
			unite(source, dest);
		}
	}
	for (int i = 0; i < V - 1; i++) {
		cout << mst[i]->d->key << "---" << mst[i]->s->key << " weight " << mst[i]->weight;
	}

}*/

Graph* Kruskal(Graph * g) {
	Graph * mstTree;
	int mSize;

	mSize = 0;
	//int V = g->V;
	mstTree = createGraph(g->V, g->V - 1);

	//int current = 0;
	int v = 0;
	quickSort(g->edges, 0, g->E - 1);

	//Node** nodes = (Node**)malloc(V*sizeof(Node));

	for (int i = 1; i <= g->V; i++) {
		mstTree->nodes[i] = make_set(i);
	}

	for (int i = 0; i < g->V; i++) {
		Node* source = find_set(g->edges[i].s);
		Node* dest = find_set(g->edges[i].d);
		ops++;
		if (source != dest) {
			ops++;
			mstTree->edges[mSize] = g->edges[i];
			mSize++;
			unite(source, dest);
		}
	}
	mstTree->E = g->V - 1;
	ops++;
	return mstTree;
}

int main()
{
	/*int n;
	cout << "give number of nodes";
	cin >> n;
	Node *nodes1[100];
	for (int i = 0; i < n; i++) {
		nodes1[i] = make_set(i);
		cout << nodes1[i]->key << " -> " << find_set(nodes1[i])->key << endl;
	}

	cout << "\nAFTER UNION\n";
	unite(nodes1[0], nodes1[2]);
	unite(nodes1[3], nodes1[2]);
	unite(nodes1[2], nodes1[9]);
	unite(nodes1[4], nodes1[8]);
	unite(nodes1[9], nodes1[1]);
	
	for (int i = 0; i < n; i++) {
		cout << nodes1[i]->key << " -> " << find_set(nodes1[i])->key << endl;
	}
	*/
	cout << "\nKRUSKAL\n";

	int V = 5;
	int E = 7;

	Node* nodes[5];
	for (int i = 0; i < 5; i++) {
		nodes[i] = make_set(i);
	}

	Graph * g = createGraph(V, E);

	for (int v = 1; v <= g->V; v++) {
		g->nodes[v] = make_set(v);
	}
	
	g->edges[0].s = g->nodes[1];
	g->edges[0].d = g->nodes[2];
	g->edges[0].weight = 10;

	g->edges[1].s = g->nodes[2];
	g->edges[1].d = g->nodes[3];
	g->edges[1].weight = 9;

	g->edges[2].s = g->nodes[3];
	g->edges[2].d = g->nodes[4];
	g->edges[2].weight = 2;

	g->edges[3].s = g->nodes[4];
	g->edges[3].d = g->nodes[5];
	g->edges[3].weight = 7;

	g->edges[4].s = g->nodes[5];
	g->edges[4].d = g->nodes[1];
	g->edges[4].weight = 3;

	g->edges[5].s = g->nodes[1];
	g->edges[5].d = g->nodes[4];
	g->edges[5].weight = 5;

	g->edges[6].s = g->nodes[2];
	g->edges[6].d = g->nodes[4];
	g->edges[6].weight = 12;

	g = Kruskal(g);

	

	for (int i = 0; i < g->E; i++) {
		cout << g->edges[i].s->key << "---" << g->edges[i].d->key << "; weight: " << g->edges[i].weight << "\n";
	}


	int weights[40000];

	for (int n = 100; n <= 10000; n += 100) {
		Graph* G = createGraph(n, n * 4);
		FillRandomArray(weights, n*4, 10, 50000);
		ops = 0;
		cout << n << endl;
		for (int i = 1; i <= n; i++) {
			G->nodes[i] = make_set(i);
		}

		for (int i = 0; i < 4 * n; i++) {
			Node* source;
			Node* dest;
			do {
				source = G->nodes[rand() % n + 1];
				dest = G->nodes[rand() % n + 1];
			} while (source == dest);
			G->edges[i].s = source;
			G->edges[i].d = dest;
			G->edges[i].weight = weights[i];
		}
		Kruskal(G);
		profiler.countOperation("Total_operations", n, ops);
	}
	profiler.showReport();
}


