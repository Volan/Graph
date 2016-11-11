#pragma once

struct Edge {
	int V;
	double weight;
	Edge* next;
};

Edge* addList(Edge* header, int V, double weight);
Edge* delList(Edge* header, int V);
void freeList(Edge* header);
Edge* addBackList(Edge* header, int V);
int getLengthList(Edge* header);