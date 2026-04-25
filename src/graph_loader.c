
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
static void die(const char* msg) { fprintf(stderr, "%s\n", msg); exit(1); }
Graph* load_graph(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) die("Failed to open graph file");
    int n;
    if (fscanf(f, "%d", &n) != 1 || n < 0) die("Invalid graph header");
    Graph* g = (Graph*)malloc(sizeof(Graph));
    if (!g) die("malloc failed");
    g->num_vertices = n;
    g->vertices = (Vertex*)calloc((size_t)n, sizeof(Vertex));
    if (!g->vertices) die("calloc failed");
    for (int v = 0; v < n; v++) {
        int deg;
        if (fscanf(f, "%d", &deg) != 1 || deg < 0) die("Invalid degree line");
        Edge* head = NULL; Edge* tail = NULL;
        for (int i = 0; i < deg; i++) {
            int u;
            if (fscanf(f, "%d", &u) != 1 || u < 0 || u >= n) die("Invalid neighbor id");
            Edge* e = (Edge*)malloc(sizeof(Edge));
            if (!e) die("malloc failed");
            e->dst = u; e->next = NULL;
            if (!head) head = tail = e; else { tail->next = e; tail = e; }
        }
        g->vertices[v].head = head;
    }
    fclose(f);
    return g;
}
void free_graph(Graph* g) {
    if (!g) return;
    for (int v = 0; v < g->num_vertices; v++) {
        Edge* cur = g->vertices[v].head;
        while (cur) { Edge* nxt = cur->next; free(cur); cur = nxt; }
    }
    free(g->vertices); free(g);
}
