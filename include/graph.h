
#ifndef GRAPH_H
#define GRAPH_H
typedef struct Edge { int dst; struct Edge* next; } Edge;
typedef struct Vertex { Edge* head; } Vertex;
typedef struct Graph { int num_vertices; Vertex* vertices; } Graph;
typedef struct { int num_vertices; int num_edges; int* row_ptr; int* col_idx; } CSRGraph;
Graph* load_graph(const char* filename);
void free_graph(Graph* g);
CSRGraph* convert_to_csr(Graph* g);
void free_csr(CSRGraph* g);
int bfs_pointer(Graph* g, int source, int* dist);
int bfs_csr(CSRGraph* g, int source, int* dist);
#endif
