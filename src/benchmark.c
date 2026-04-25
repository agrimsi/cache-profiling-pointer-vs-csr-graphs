
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graph.h"
static double now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1e6;
}
int run_benchmark(const char* impl, const char* graph_path, int source, int repeat) {
    Graph* g = load_graph(graph_path);
    if (!g) { fprintf(stderr, "load_graph failed\n"); return 1; }
    if (source < 0 || source >= g->num_vertices) { fprintf(stderr, "invalid source\n"); free_graph(g); return 1; }
    int* dist = (int*)malloc((size_t)g->num_vertices * sizeof(int));
    if (!dist) { fprintf(stderr, "malloc failed\n"); free_graph(g); return 1; }
    int visited = -1;
    double start = now_ms();
    if (strcmp(impl, "pointer") == 0) {
        for (int i = 0; i < repeat; i++) visited = bfs_pointer(g, source, dist);
    } else if (strcmp(impl, "csr") == 0) {
        CSRGraph* csr = convert_to_csr(g);
        if (!csr) { fprintf(stderr, "convert_to_csr failed\n"); free(dist); free_graph(g); return 1; }
        for (int i = 0; i < repeat; i++) visited = bfs_csr(csr, source, dist);
        free_csr(csr);
    } else { fprintf(stderr, "unknown impl\n"); free(dist); free_graph(g); return 1; }
    double end = now_ms();
    printf("visited=%d\n", visited);
    printf("time_ms=%.2f\n", end - start);
    free(dist); free_graph(g); return 0;
}
