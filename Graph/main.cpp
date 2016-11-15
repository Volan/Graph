#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include "List.h"
#include "algorithms.h"

void printList(Edge* header) {
	Edge* current = header;
	while (current != NULL)
	{
		printf("%d ", current->V);
		current = current->next;
	}
	printf("\n");
}

int main()
{
	int V = 6, i, j;
	Edge**E = (Edge**)malloc(sizeof(Edge*) * V);
	for (i = 0; i < V; ++i)
		E[i] = NULL;

	E[0] = addList(E[0], 1, 1);

	E[1] = addList(E[1], 2, 3);

	E[2] = addList(E[2], 3, 3);

	E[3] = addList(E[3], 4, -8);
	E[3] = addList(E[3], 5, 2);

	E[4] = addList(E[4], 1, 1);


	ShortestPaths **paths = FloydUorshell(V, E);
	for (i = 0; i < V; ++i) {
		for (j = 0; j < V; ++j) {
			if (paths[i]->shortest[j] == DBL_MAX)
				printf("X ");
			else
				printf("%3.3lf ", paths[i]->shortest[j]);
		}
		printf("\n");
	}

	for (i = 0; i < V; ++i)
		freeList(E[i]);
}