
#include "graph.h"
#include <stdlib.h>
CSRGraph* convert_to_csr(Graph* g) {
  if (g == NULL) return NULL;
  int n = g->num_vertices;
  CSRGraph* csrg = (CSRGraph*)malloc(sizeof(CSRGraph));

  if (csrg == NULL) return NULL;
  csrg->num_vertices = n;
  int* row_ptr = malloc((n + 1) * sizeof(int));
  if (row_ptr == NULL) {
    free(csrg);
    return NULL;
  }

  int number_of_edges = 0;

  for (int i = 0; i < n; i++) {
    row_ptr[i] = number_of_edges;
    Edge* cur = g->vertices[i].head;
    while (cur != NULL) {
      number_of_edges++;
      cur = cur->next;
    }
  }

  row_ptr[n] = number_of_edges;
  csrg->num_edges = number_of_edges;
  int* col_idx = (int*)malloc(number_of_edges * sizeof(int));
  if (!col_idx && number_of_edges) {
    free(row_ptr);
    free(csrg);
    return NULL;
  };

  int cur_pos = 0;
  for (int i = 0; i < n; i++) {
    Edge* cur = g->vertices[i].head;
    while (cur != NULL) {
      col_idx[cur_pos++] = cur->dst;
      cur = cur->next;
    }
  }
  csrg->row_ptr = row_ptr;
  csrg->col_idx = col_idx;
  return csrg;
}
void free_csr(CSRGraph* g) {
  if (!g) return;
  free(g->row_ptr);
  free(g->col_idx);
  free(g);
}
