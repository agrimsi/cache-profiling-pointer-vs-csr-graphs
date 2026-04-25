
CC=gcc
CFLAGS=-O2 -std=gnu11 -Wall -Wextra -pedantic -Iinclude
SRC=src/main.c src/benchmark.c src/graph_loader.c src/bfs_pointer.c src/graph_csr.c src/bfs_csr.c
OBJ=$(SRC:.c=.o)
all: graph_bench
graph_bench: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)
clean:
	rm -f graph_bench driver_check driver_csr_dump graph_bench_asan $(OBJ) src/driver_check.c src/driver_csr_dump.c
.PHONY: all clean
