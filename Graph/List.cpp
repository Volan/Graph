#include "List.h"
#include <stdlib.h>
#include <stdio.h>

Edge* addList(Edge* header, int V, double weight) {
	Edge* edge = (Edge*)malloc(sizeof(Edge));
	edge->V = V;
	edge->next = header;
	edge->weight = weight;
	return edge;
}

Edge* addBackList(Edge* header, int V) {
	Edge *current = header, *prev = NULL;
	while (current != NULL) {
		prev = current;
		current = current->next;
	}

	Edge* edge = (Edge*)malloc(sizeof(Edge));
	edge->V = V;
	edge->next = NULL;

	if (prev == NULL) {
		return edge;
	}
	else {
		prev->next = edge;
		return header;
	}
}

Edge* delList(Edge* header, int V) {
	Edge *current = header, *prev = NULL;
	while (current != NULL)
	{
		if (current->V == V) {
			if (prev != NULL)
				prev->next = current->next;
			else
				header = current->next;
			free(current);
			break;
		}
		prev = current;
		current = current->next;
	}
	return header;
}

void freeList(Edge* header) {
	Edge* current;
	while (header != NULL)
	{
		current = header;
		header = header->next;
		free(current);
	}
}

int getLengthList(Edge* header) {
	int length = 0;
	while (header != NULL)
	{
		++length;
		header = header->next;
	}
	return length;
}