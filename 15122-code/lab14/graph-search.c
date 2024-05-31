/*
 * Graph Search - version 2: with adjacency matrix
 *
 * 15-122 Principles of Imperative Computation
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "graph-search.h"
#include "lib/contracts.h"
#include "lib/uint-queue.h"
#include "lib/uint-stack.h"
#include "lib/xalloc.h"

/* DFS Search using system stack (i.e. recursive) */

bool dfs_helper(graph_t G, bool *mark, vertex start, vertex target) {
  REQUIRES(G != NULL && mark != NULL);
  REQUIRES(start < graph_size(G) && target < graph_size(G));
  REQUIRES(!mark[start]);

  mark[start] = true;
  printf("    Visiting %u\n", start);

  if (target == start) return true;
  neighbors_t nbors = graph_get_neighbors(G, start);
  while (graph_hasmore_neighbors(nbors)) {
    vertex v = graph_next_neighbor(nbors);
    if (!mark[v] && dfs_helper(G, mark, v, target)) {
      graph_free_neighbors(nbors);
      return true;
    }
  }
  graph_free_neighbors(nbors);
  return false;
}

bool dfs(graph_t G, vertex start, vertex target) {
  REQUIRES(G != NULL);
  REQUIRES(start < graph_size(G) && target < graph_size(G));

  printf("DFS:\n");
  bool mark[graph_size(G)];
  for (vertex i = 0; i < graph_size(G); i++)
    mark[i] = false;
  return dfs_helper(G, mark, start, target);
}


/* BFS Search using queue */

bool bfs(graph_t G, vertex start, vertex target) {
  REQUIRES(G != NULL);
  REQUIRES(start < graph_size(G) && target < graph_size(G));

  // printf("BFS:\n");

  if (start == target) return true;

  // mark is an array containing only the start
  bool mark[graph_size(G)];
  for (vertex i = 0; i < graph_size(G); i++)
    mark[i] = false;
  mark[start] = true;

  queue_t Q = queue_new();
  enq(Q, start);

  while (!queue_empty(Q)) {
    vertex v = deq(Q);     // v is the current node
    // printf("    Visiting %u\n", v);
    neighbors_t nbors = graph_get_neighbors(G, v);
    while (graph_hasmore_neighbors(nbors)) {
      vertex w = graph_next_neighbor(nbors);  // w is one of v's neighbors
      if (w == target) {   // if w is the target return true
        // printf("    Found    %u !!!\n", w);
        queue_free(Q);
        graph_free_neighbors(nbors);
        return true;
      }

      if (!mark[w]) {      // if w is not already marked
        mark[w] = true;       // Insert it into the marks
        enq(Q, w);            // Enqueue it onto the queue
      }
    }
    graph_free_neighbors(nbors);
  }
  queue_free(Q);
  return false;
}


bool connected(graph_t G) {
  REQUIRES(G != NULL);
  unsigned int size = graph_size(G);

  // TO BE COMPLETED
  for (unsigned int i = 0; i < size; i++) {
    for(unsigned int j = i + 1; j < size; j++) {
      if(!bfs(G, i, j)) return false;
    }
  }

  return true;
}
