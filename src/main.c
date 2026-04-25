
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int run_benchmark(const char* impl, const char* graph_path, int source, int repeat);
static const char* get_arg_value(int argc, char** argv, const char* prefix) {
    size_t len = strlen(prefix);
    for (int i = 1; i < argc; i++) if (strncmp(argv[i], prefix, len) == 0) return argv[i] + len;
    return NULL;
}
int main(int argc, char** argv) {
    const char* impl = get_arg_value(argc, argv, "--impl=");
    const char* graph = get_arg_value(argc, argv, "--graph=");
    const char* source_s = get_arg_value(argc, argv, "--source=");
    const char* repeat_s = get_arg_value(argc, argv, "--repeat=");
    if (!impl || !graph) { fprintf(stderr, "Usage: ./graph_bench --impl=pointer|csr --graph=FILE [--source=0] [--repeat=1]\n"); return 1; }
    int source = source_s ? atoi(source_s) : 0;
    int repeat = repeat_s ? atoi(repeat_s) : 1;
    if (repeat <= 0) repeat = 1;
    return run_benchmark(impl, graph, source, repeat);
}
