# Cache-Friendly Graph Processing: Pointer Adjacency Lists vs. CSR Format

## Overview

In modern computer architecture, graph algorithms like Breadth-First Search (BFS) are frequently bottlenecked by memory latency rather than computational complexity. Standard pointer-based graph representations suffer from severe pointer chasing and memory fragmentation, causing low spatial locality and frequent CPU cache stalls.

In this project, I evaluated the performance implications of data layout by implementing and benchmarking the same BFS traversal algorithm across two distinct graph representations:

1. **Pointer-Based Adjacency List**: Linked list structure with heap-allocated edge objects.
2. **Compressed Sparse Row (CSR)**: Contiguous array layout (`row_ptr` and `col_idx`) optimized for hardware prefetching and spatial locality.

I built an automated benchmarking framework to collect timing distributions and ran architectural cache simulations using Valgrind's Cachegrind to quantify D1 and LLd cache miss rates across various cache sizes, line sizes, and associativities.


## Completed Implementation

The core algorithm logic and data structure transformations are implemented in the following files:

* **`src/bfs_pointer.c`**: Implemented standard Breadth-First Search over a pointer-based adjacency list graph. Traversal requires dereferencing head and next pointers across heap-allocated `Edge` nodes.
* **`src/graph_csr.c`**: Implemented the conversion utility `convert_to_csr()` that computes vertex degrees, allocates contiguous arrays, and flattens adjacency lists into `row_ptr` (offsets) and `col_idx` (neighbor indices) arrays.
* **`src/bfs_csr.c`**: Implemented cache-friendly BFS traversal over the CSR layout, transforming linked-list traversals into bounded loops over contiguous slices of memory.


## Methodology & Experimental Design

To isolate traversal efficiency from fixed program overheads and system noise:

* **Core Pinning**: All experiments were pinned to a single CPU core to prevent thread migration and OS scheduler interference.
* **Differential Timing**: I used a differential statistical timing formula $(T_{\text{large}} - T_{\text{small}}) / (R_{\text{large}} - R_{\text{small}})$ across varying run counts to mathematically eliminate graph loading and initialization costs.
* **Phase Isolation**: Conversion latency from pointer to CSR format was explicitly isolated using custom timing hooks so that one-time preprocessing costs did not contaminate pure traversal measurements.
* **Synthetic Workloads**: Generated synthetic graphs across multiple topologies:
* **Erdős-Rényi (ER) Random Graphs**: $N \in [30\text{k}, 100\text{k}]$, degrees $d=8$ (sparse) and $d=32$ (dense).
* **Grid Graph**: $316 \times 316$ (~$100\text{k}$ nodes) to evaluate structured spatial locality.
* **Chain & Star Graphs**: $100\text{k}$ nodes to test extreme search depths vs. branching factors.



## Key Performance & Architectural Insights

### 1. Execution Speedup & Runtime Stability

* **CSR Speedup**: CSR consistently outperformed pointer-based BFS across all graph topologies, yielding speedups from **1.18x** on a chain graph up to **7.04x** on a dense random graph ($50\text{k}$ nodes, degree 32).
* **Variance**: Pointer-based BFS exhibited high variance due to heap fragmentation and random DRAM row-buffer hits. CSR delivered tight, deterministic runtime distributions by maximizing spatial locality and hardware prefetching.

### 2. Architectural Cache Profiling (Cachegrind)

* **L1 Data Cache (D1) Misses**: CSR drastically reduced D1 miss rates across all topologies. In the dense ER graph ($50\text{k}$ nodes, degree 32), D1 misses dropped from **2.33 million** (pointer) to **1.59 million** (CSR).
* **Line Size Sweeps ($32\text{B} \rightarrow 128\text{B}$)**: Increasing cache line size yielded the largest performance improvement for CSR. Because neighbor indices are stored sequentially in `col_idx`, a $128\text{B}$ cache line loads up to 32 neighbor IDs in a single memory fetch. Pointer-based layouts could not effectively leverage larger cache lines due to scattered heap allocations.
* **Associativity Sweeps ($2\text{-way} \rightarrow 8\text{-way}$)**: Varying associativity had negligible impact on miss rates, proving that conflict misses are not the primary bottleneck in BFS. Memory layout selection is far more critical than associativity tuning.
* **Cache Size Sweeps ($16\text{KB} \rightarrow 64\text{KB}$)**: Larger cache capacities reduced misses primarily in graphs with broad traversal frontiers (Grid and dense ER graphs), while streaming patterns (Chain) showed minimal sensitivity to capacity changes.


## Instructions to Run & Profile

### 1. Build the Project

Compile the benchmarking binary using the included Makefile:

```bash
make
```

### 2. Generate Graph Workloads

Generate synthetic graph text files using the Python generator script:

```bash
python3 scripts/gen_graph.py --kind <er|grid|chain|star> --n <num_nodes> --deg <avg_degree> --out data/<graph_name>.txt
```

*Example:*

```bash
python3 scripts/gen_graph.py --kind er --n 50000 --deg 32 --out data/random_n50k_d32.txt
```

### 3. Run Benchmark

Execute BFS traversals for both representations with repeated trials:

```bash
# Pointer-based BFS
./graph_bench --impl=pointer --graph=data/random_n50k_d32.txt --source=0 --repeat=100

# CSR-based BFS
./graph_bench --impl=csr --graph=data/random_n50k_d32.txt --source=0 --repeat=100
```

### 4. Architectural Profiling with Cachegrind

Profile L1 and Last-Level cache behavior:

```bash
# Baseline profiling
valgrind --tool=cachegrind ./graph_bench --impl=csr --graph=data/random_n50k_d32.txt --source=0

# Parametric sweep (Cache Size = 32KB, Associativity = 8-way, Line Size = 64B)
valgrind --tool=cachegrind --D1=32768,8,64 ./graph_bench --impl=csr --graph=data/random_n50k_d32.txt --source=0
```
