#include <limits.h>
#include <float.h>
#include <stdlib.h>
#include "List.h"
#include "algorithms.h"

void freeShortestPaths(ShortestPaths* sp) {
	free(sp->prev);
	free(sp->shortest);
	free(sp);
}

ShortestPaths* initializeSP(int V, int index) {
	int i;
	ShortestPaths *sp = (ShortestPaths*)malloc(sizeof(ShortestPaths));
	sp->V = V;
	sp->shortest = (double*)malloc(sizeof(double) * V);

	for (i = 0; i < V; ++i)
		sp->shortest[i] = DBL_MAX;
	sp->shortest[index] = 0;

	sp->prev = (int*)malloc(sizeof(int) * V);

	for (i = 0; i < V; ++i)
		sp->prev[i] = -1;
	return sp;
}

int relax(ShortestPaths* sp, Edge *E, int source) {
	if (sp->shortest[source] != DBL_MAX && sp->shortest[E->V] > sp->shortest[source] + E->weight) {
		sp->shortest[E->V] = sp->shortest[source] + E->weight;
		sp->prev[E->V] = source;
		return 1;
	}
	return 0;
}

void relaxMultiple(ShortestPaths* sp, Edge **E, int source) {
	Edge *edge = E[source];
	while (edge != NULL)
	{
		relax(sp, edge, source);
		edge = edge->next;
	}
}

Edge* topologicalSort(int V, Edge**E) {
	int i;
	Edge *result = NULL, *current, *next = NULL;

	int* indegree = (int*)malloc(sizeof(int) * V);

	for (i = 0; i < V; ++i)
		indegree[i] = 0;

	for (i = 0; i < V; ++i) {
		current = E[i];
		while (current != NULL)
		{
			++indegree[current->V];
			current = current->next;
		}
	}

	for (i = 0; i < V; ++i) {
		if (indegree[i] == 0)
			next = addList(next, i, 0);
	}
	while (next != NULL)
	{
		result = addBackList(result, next->V);
		current = E[next->V];
		next = delList(next, next->V);

		while (current != NULL)
		{
			--indegree[current->V];
			if (indegree[current->V] == 0)
				next = addList(next, current->V, 0);
			current = current->next;
		}
	}

	return result;
}

ShortestPaths* DAGShortestPath(int V, Edge**E, int source) {
	ShortestPaths *sp = initializeSP(V, source);
	Edge *topSort = topologicalSort(V, E);

	Edge *cur = topSort;
	while (cur != NULL)
	{
		relaxMultiple(sp, E, cur->V);
		cur = cur->next;
	}

	freeList(topSort);

	return sp;
}

int findMinArray(ShortestPaths *sp, int *Q, int size) {
	if (size <= 0)
		return -1;

	int min = 0;
	for (int i = 1; i < size; ++i) {
		if (sp->shortest[Q[min]] > sp->shortest[Q[i]])
			min = i;
	}
	return min;
}

ShortestPaths* DejkstraArray(int V, Edge**E, int source) {
	int i = 0;
	ShortestPaths *sp = initializeSP(V, source);

	int *Q = (int*)malloc(sizeof(int) * V);
	for (i = 0; i < V; ++i)
		Q[i] = i;

	int size = V, min, tmp;
	while (size != 0) {
		min = findMinArray(sp, Q, size);
		
		tmp = Q[min];
		Q[min] = Q[size - 1];
		Q[size - 1] = tmp;

		relaxMultiple(sp, E, Q[size - 1]);
		--size;
	}
	free(Q);

	return sp;
}

void swapHeapAndIndexInHeap(int *heap, int *indexInHeap, int i, int j) {
	int tmp = indexInHeap[heap[i]];
	indexInHeap[heap[i]] = indexInHeap[heap[j]];
	indexInHeap[heap[j]] = tmp;

	tmp = heap[i];
	heap[i] = heap[j];
	heap[j] = tmp;
}

int extractMin(int *heap, int *indexInHeap, int size) {
	swapHeapAndIndexInHeap(heap, indexInHeap, 0, size - 1);
	return heap[size - 1];
}

void swim(ShortestPaths* sp, int *heap, int size, int *indexInHeap, int index) {

	int inHeap, child;
	inHeap = indexInHeap[index];

	child = 2 * inHeap + 1;

	if (child >= size)
		return;

	if (child + 1 < size && sp->shortest[heap[child + 1]] < sp->shortest[heap[child]])
		++child;

	if (sp->shortest[heap[child]] < sp->shortest[heap[inHeap]]) {
		swapHeapAndIndexInHeap(heap, indexInHeap, child, inHeap);
		swim(sp, heap, size, indexInHeap, heap[child]);
	}
}

void swap(ShortestPaths* sp, int *heap, int size, int *indexInHeap, int index) {

	int inHeap, parent;
	inHeap = indexInHeap[index];

	parent = ((inHeap + 1) / 2) - 1;

	if (parent < 0)
		return;

	if (sp->shortest[heap[parent]] > sp->shortest[heap[inHeap]]) {
		swapHeapAndIndexInHeap(heap, indexInHeap, parent, inHeap);
		swap(sp, heap, size, indexInHeap, heap[parent]);
	}
}

ShortestPaths* DejkstraHeap(int V, Edge**E, int source) {
	int i = 0, size, *heap, *indexInHeap, min, isRelaxed;
	Edge *edge = NULL;
	ShortestPaths *sp = initializeSP(V, source);

	size = V;
	heap = (int*)malloc(sizeof(int) * V);
	indexInHeap = (int*)malloc(sizeof(int) * V);
	for (i = 0; i < V; ++i)
		heap[i] = indexInHeap[i] = i;

	for (i = V / 2; i >= 0; i--)
		swim(sp, heap, size, indexInHeap, i);

	while (size != 0) {
		min = extractMin(heap, indexInHeap, size);
		--size;

		swim(sp, heap, size, indexInHeap, heap[0]);

		edge = E[min];
		while (edge != NULL) {
			isRelaxed = relax(sp, edge, min);
			if (isRelaxed > 0)
				swap(sp, heap, size, indexInHeap, edge->V);
			edge = edge->next;
		}
	}

	free(heap);
	free(indexInHeap);
	return sp;
}

ShortestPaths* Dejkstra(int V, Edge**E, int source) {
	int i = 0;
	ShortestPaths *sp = NULL;

	int length = 0;
	for (i = 0; i < V; ++i)
		length += getLengthList(E[i]);

	if (length / (double)(V * V) < 0.75) {
		sp = DejkstraHeap(V, E, source);
	}
	else {
		sp = DejkstraArray(V, E, source);
	}
	return sp;
}

ShortestPaths* BellmanFord(int V, Edge**E, int source) {
	int i, j;
	Edge *edge;
	ShortestPaths *sp = initializeSP(V, source);

	for (i = 0; i < V - 1; ++i)
		for (j = 0; j < V; ++j) {
			edge = E[j];
			while (edge != NULL)
			{
				relax(sp, edge, j);
				edge = edge->next;
			}
		}
	return sp;
}

Edge* FindNegativeCycle(int V, Edge**E) {
	int i = 0, inCycle = -1, startCycle = -1;
	Edge *edge = NULL, *cycle = NULL;
	ShortestPaths *sp = BellmanFord(V, E, 0);
	int *visited;

	for (i = 0; i < V; ++i) {
		edge = E[i];
		while (edge != NULL) {
			if (sp->shortest[i] != DBL_MAX && sp->shortest[edge->V] > sp->shortest[i] + edge->weight) {
				inCycle = edge->V;
				break;
			}
			edge = edge->next;
		}
		if (inCycle != -1)
			break;
	}

	if (inCycle >= 0) {
		visited = (int*)malloc(sizeof(int) * V);
		for (i = 0; i < V; ++i)
			visited[i] = 0;

		do {
			visited[inCycle] = 1;
			inCycle = sp->prev[inCycle];
		} while (visited[inCycle] != 1);

		startCycle = inCycle;
		do {
			cycle = addList(cycle, inCycle, 0);
			inCycle = sp->prev[inCycle];
		} while (inCycle != startCycle);
	}

	freeShortestPaths(sp);
	return cycle;
}

ShortestPaths** FloydUorshell(int V, Edge**E) {
	Edge *edge;
	int i = 0, j = 0, k = 0;
	double** shortest = (double**)(malloc(sizeof(double*) * V));
	int** prev = (int**)(malloc(sizeof(int*) * V));
	ShortestPaths** spaths = (ShortestPaths**)(malloc(sizeof(ShortestPaths*) * V));
	for (i = 0; i < V; ++i) {
		shortest[i] = (double*)(malloc(sizeof(double) * V));
		prev[i] = (int*)(malloc(sizeof(int) * V));
		spaths[i] = initializeSP(V, i);
	}

	for (i = 0; i < V; ++i) {
		for (j = 0; j < V; ++j) {
			prev[i][j] = NULL;
			shortest[i][j] = DBL_MAX;
		}
		shortest[i][i] = 0;
	}

	for (i = 0; i < V; ++i) {
		edge = E[i];
		while (edge != NULL) {
			shortest[i][edge->V] = edge->weight;
			prev[i][edge->V] = i;
			edge = edge->next;
		}
	}

	for (k = 0; k < V - 1; ++k) {
		for (i = 0; i < V; ++i) {
			for (j = 0; j < V; ++j) {
				if ((shortest[i][k] != DBL_MAX && shortest[k][j] != DBL_MAX) && shortest[i][j] > shortest[i][k] + shortest[k][j]) {
					shortest[i][j] = shortest[i][k] + shortest[k][j];
					prev[i][j] = prev[k][j];
				}
			}
		}
	}

	for (i = 0; i < V; ++i) {
		for (j = 0; j < V; ++j) {
			spaths[i]->shortest[j] = shortest[i][j];
			spaths[i]->prev[j] = prev[i][j];
		}
	}

	for (i = 0; i < V; ++i) {
		free(shortest[i]);
		free(prev[i]);
	}
	free(shortest);
	free(prev);

	return spaths;
}




