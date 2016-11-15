#pragma once

#include "List.h"

struct ShortestPaths {
	int V;
	double *shortest;
	int *prev;
};


Edge* topologicalSort(int V, Edge**E);
ShortestPaths* DAGShortestPath(int V, Edge**E, int source);
ShortestPaths* Dejkstra(int V, Edge**E, int source);
ShortestPaths* BellmanFord(int V, Edge**E, int source);
Edge* FindNegativeCycle(int V, Edge**E);
ShortestPaths** FloydUorshell(int V, Edge**E);