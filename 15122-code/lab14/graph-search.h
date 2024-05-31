/*
 * Graph Search
 *
 * 15-122 Principles of Imperative Computation
 */

#include <stdbool.h>
#include "graph.h"

#ifndef GRAPH_SEARCH_H
#define GRAPH_SEARCH_H

bool dfs(graph_t G, vertex start, vertex target);
//@requires G != NULL;
//@requires start < graph_size(G) && target < graph_size(G);

bool bfs(graph_t G, vertex start, vertex target);
//@requires G != NULL;
//@requires start < graph_size(G) && target < graph_size(G);

bool connected(graph_t G);
//@requires G != NULL;

#endif
