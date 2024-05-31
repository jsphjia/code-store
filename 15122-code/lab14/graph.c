/*
 * Graphs - version 2: Adjacency Matrix
 *
 * 15-122 Principles of Imperative Computation
 */

#include <stdlib.h>
#include "graph.h"
#include "lib/xalloc.h"
#include "lib/contracts.h"
#include "lib/uint-stack.h"

typedef struct graph_header graph;
struct graph_header {
  unsigned int size;
  bool **adj;        // Pointer to adjacency matrix
};

/* Data structure invariants */

bool is_vertex(graph *G, vertex v) {
  REQUIRES(G != NULL);
  return v < G->size;
}

bool is_graph(graph *G) {
  if (G == NULL) return false;
  if (G->adj == NULL) return false;
  for (unsigned int i = 0; i < G->size; i++) {
    if (G->adj[i] == NULL) return false;
    if (G->adj[i][i]) return false;
  }

  // TO BE COMPLETED:
  for (unsigned int i = 0; i < G->size; i++) {
    for (unsigned int j = i + 1; j < G->size; j++) {
      if(G->adj[i][j] != G->adj[j][i]) return false;
    }
  }

  return true;
}

graph *graph_new(unsigned int size) {
  graph *G = xmalloc(sizeof(graph));
  G->adj = xcalloc(size, sizeof(bool *));
  G->size = size;

  for (unsigned int i = 0; i < size; i++) {
    G->adj[i] = xcalloc(size, sizeof(bool));
  }

  ENSURES(is_graph(G));
  return G;
}

void graph_free(graph *G) {
  REQUIRES(is_graph(G));

  for (unsigned int i = 0; i < G->size; i++) {
    free(G->adj[i]);
  }

  free(G->adj);
  free(G);
}

unsigned int graph_size(graph *G) {
  REQUIRES(is_graph(G));
  return G->size;
}

bool graph_hasedge(graph *G, vertex v, vertex w) {
  REQUIRES(is_graph(G) && is_vertex(G, v) && is_vertex(G, w));

  return G->adj[v][w];
}

void graph_addedge(graph *G, vertex v, vertex w) {
  REQUIRES(is_graph(G) && is_vertex(G, v) && is_vertex(G, w));
  REQUIRES(v != w && !graph_hasedge(G, v, w));

  G->adj[v][w] = true;
  G->adj[w][v] = true;

  ENSURES(is_graph(G));
}




// EXTRA CHALLENGE:
// . define the type struct neighbor_header (typedef'ed neighbors) and
//   a suitable representation invariant function is_neighbors for it
// . implement graph_get_neighbors, graph_hasmore_neighbors,
//   graph_next_neighbor and graph_free_neighbors
// If you are not up for it, their code is at the bottom of this file


// bool is_neighbors(neighbors *nbors) {

    // TO BE COMPLETED OPTIONALLY (you may need helper functions)

// }


// neighbors* graph_get_neighbors(graph *G, vertex v) {
//  REQUIRES(is_graph(G) && is_vertex(G, v));

    // TO BE COMPLETED OPTIONALLY

// }


// bool graph_hasmore_neighbors(neighbors *nbors) {
//  REQUIRES(is_neighbors(nbors));

    // TO BE COMPLETED OPTIONALLY

// }


// vertex graph_next_neighbor(neighbors *nbors) {
//   REQUIRES(is_neighbors(nbors));
//   REQUIRES(graph_hasmore_neighbors(nbors));

    // TO BE COMPLETED OPTIONALLY

// }


// void graph_free_neighbors(neighbors* nbors) {
//  REQUIRES(is_neighbor(nbors));

    // TO BE COMPLETED OPTIONALLY

// }

// IF YOU ATTEMPTED THESE FUNCTION, COMMENT THE CODE BELOW





















/**************** Neighbors ****************/

struct neighbor_header {
  stack_t data;
};
typedef struct neighbor_header neighbors;


bool is_neighbors(neighbors *nbors) {
  return nbors != NULL && nbors->data != NULL;
}

/* Interface functions */

neighbors *graph_get_neighbors(graph *G, vertex v) {
  REQUIRES(is_graph(G) && is_vertex(G, v));
  neighbors *nbors = xmalloc(sizeof(neighbors));
  nbors->data = stack_new();

  unsigned int size = graph_size(G);
  for (vertex w = 0; w < size; w++) {
    if (graph_hasedge(G, v, w)) {
      push(nbors->data, w);
    }
  }
  ENSURES(is_neighbors(nbors));
  return nbors;
}

bool graph_hasmore_neighbors(neighbors *nbors) {
  REQUIRES(is_neighbors(nbors));
  return !stack_empty(nbors->data);
}

vertex graph_next_neighbor(neighbors *nbors) {
  REQUIRES(is_neighbors(nbors));
  REQUIRES(graph_hasmore_neighbors(nbors));

  return pop(nbors->data);
}

void graph_free_neighbors(neighbors *nbors) {
  REQUIRES(is_neighbors(nbors));
  stack_free(nbors->data);
  free(nbors);
}
