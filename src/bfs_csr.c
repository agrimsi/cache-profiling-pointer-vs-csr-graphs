
#include "graph.h"
#include <stdlib.h>
int bfs_csr(CSRGraph* g, int source, int* dist) {
  if (g == NULL || source < 0 || source >= g->num_vertices) return -1;
  int number_of_vertices = g->num_vertices;
  int* queue = malloc(number_of_vertices * sizeof(int));
  if (queue == NULL) return -1;
  int head = 0;
  int tail = 0;
  for (int i = 0; i < number_of_vertices; i++) {
    dist[i] = -1;
  }
  dist[source] = 0;
  queue[tail++] = source;
  int number_of_visited_nodes = 0;
  while (head < tail) {
    int u = queue[head++];
    number_of_visited_nodes++;
    for (int i = g->row_ptr[u]; i < g->row_ptr[u + 1]; i++) {
      int v = g->col_idx[i];
      if (dist[v] == -1) {
        dist[v] = dist[u] + 1;
        queue[tail++] = v;
      }
    }
  }
  free(queue);
  return number_of_visited_nodes;
}
