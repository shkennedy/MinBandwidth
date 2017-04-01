// Shane Kennedy
// 110066543

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int nnodes;
int nedges;
int lb;

int *best_perm;
int min_bandwidth;

typedef struct edge {
    int from;
    int to;
} egde;

int process_solution(int *nodes) {
    // Check if matches lower bound
    for (int i = 0; i < nnodes; ++i) {

    }
}

// Finds the cost of adding used_node to the permutation
int find_cost(int *nodes, int cursor, int used_node, int adj_m[nnodes][nnodes]) {
    for (int i = 0; i < cursor; ++i) {
        if (adj_m[nodes[i]][used_node] == 1) {
            return cursor - i;
        }
    }
    return 0;
}

// Makes available candidates for use in the current permutation
void make_candidates(int *nodes, int cursor, int candidates[]) {
    // Find all used nodes
    int used_nodes[nnodes];
    for (int i = 0; i < nnodes; ++i) {
        used_nodes[i] = 0;
    }
    for (int i = 0; i < cursor; ++i) {
        used_nodes[nodes[i]] = 1;
    }

    // Fill candidates array
    int cand_size = nnodes - cursor;
    for (int i = 0, j = 0; i < nnodes; ++i) {
        if (!used_nodes[i])
            candidates[j++] = i; 
    }
}

// Recursively contructs permutations, pruning out bad branches
void make_permutations(int nodes[nnodes], int cursor, int adj_m[nnodes][nnodes],
int max) {
    if (cursor == nnodes) {
        // Solution found
        if (process_solution(nodes)) {}
        memcpy(best_perm, nodes, nnodes * sizeof(int));
        min_bandwidth = max;
        return;
    }
    }

    int nrem = nnodes - cursor, bandwidth;
    int candidates[nrem];
    make_candidates(nodes, cursor, candidates); 

    for (int i = 0; i < nrem; ++i) {
        // Check if upper bound is being exceeded
        bandwidth = find_cost(nodes, cursor, candidates[i], adj_m);
        if (bandwidth > min_bandwidth) {
            continue;
        }
        if (bandwidth > max) {
            max = bandwidth;
        }
        nodes[cursor] = candidates[i];
        //printf("%d ", nodes[cursor]);
        make_permutations(nodes, cursor + 1, adj_m, max);
    }
}

// Makes adjacency matrix for graph
void fill_adj_matrix(struct edge edges[], int adj_m[nnodes][nnodes]) {
    for (int i = 0; i < nnodes; ++i) {
        for (int j = 0; j < nnodes; ++j) {
            adj_m[i][j] = 0;
        }
    }
    for (int i = 0; i < nedges; ++i) {
        adj_m[edges[i].from - 1][edges[i].to - 1] = 1;
        adj_m[edges[i].to -1 ][edges[i].from - 1] = 1;
    }
}

// Finds upper and lower bounds for bandwidth
int find_bounds(struct edge edges[], int adj_m[nnodes][nnodes]) {
    // Lower bound - node degree
    int max_deg = 0;
    for (int i = 0, count = 0; i < nnodes; ++i) {
        for (int j = 0; j < nnodes; ++j) {
            if (adj_m[i][j]) {
                ++count;
            }
        }
        if (count > max_deg)
            max_deg = count;
    }
    lb = max_deg / 2;

    // Upper bound - graph diameter
    min_bandwidth = 1;
    for (int i = 0; i < nedges; ++i) {
        if (edges[i].to - edges[i].from > min_bandwidth)
            min_bandwidth = edges[i].to - edges[i].from;
    }
}

int main(int argc, char **argv) {
    FILE *fd;
    if ((fd = fopen("./10v", "r")) == NULL) {
        printf("Invalid file\n");
        exit(1);
    }

    fscanf(fd, "%d\n%d", &nnodes, &nedges);
    if (nnodes == 0 || nedges == 0) {
        printf("Invalid file\n");
        exit(1);
    }

    egde edges[nedges];
    int i = 0;
    while (i < nedges) {
        fscanf(fd, "%d %d\n", &edges[i].from, &edges[i].to);
        ++i;
    }
    
    // Make adjacency matrix for graph
    int adj_m[nnodes][nnodes];
    fill_adj_matrix(edges, adj_m);
    
    // Find lower and upper bounds of bandwidth
    find_bounds(edges);
    
    // Make permutations, looking for lower bound, pruning out upper bound
    best_perm = malloc(nnodes * sizeof(int));
    int nodes[nnodes];
    make_permutations(nodes, 0, adj_m, 0);

    printf("File: \nPermutation: ");
    for (int i = 0; i < nnodes; ++i) {
        printf("%d ", best_perm[i] + 1);
    }
    printf("\nBandwidth: %d\n", min_bandwidth);

    return EXIT_SUCCESS;
}