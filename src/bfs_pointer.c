
#include "graph.h"
#include <stdlib.h>

int bfs_pointer(Graph* g, int source, int* dist) {
  if (g == NULL || source < 0 || source >= g->num_vertices) return -1;
  int number_of_vertices = g->num_vertices;
  int visited_count = 0;
  int* queue = malloc(number_of_vertices * sizeof(int));
  if (queue == NULL) return -1;
  int head = 0;
  int tail = 0;
  for (int i = 0; i < number_of_vertices; i++) {
    dist[i] = -1;
  }
  dist[source] = 0;
  queue[tail++] = source;
  Vertex* vertices = g->vertices;
  while (head < tail) {
    int u = queue[head++];
    visited_count++;
    Edge* cur = vertices[u].head;
    while (cur != NULL) {
      if (dist[cur->dst] == -1) {
        dist[cur->dst] = dist[u] + 1;
        queue[tail++] = cur->dst;
      }
      cur = cur->next;
    }
  }

  free(queue);
  return visited_count;
}
