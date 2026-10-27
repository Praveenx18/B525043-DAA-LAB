# Reverse Time-Step DSU Application

## Problem Statement
Given an undirected graph whose vertices have values, vertices are deleted in a specified order. After each deletion, report the maximum sum of vertex values in any connected component that remains.

## Objective
Use a Disjoint Set Union (DSU) data structure and process the deletions in reverse order, turning deletions into vertex additions.

## Approach: Reverse-Time DSU
A normal DSU efficiently merges connected components but does not naturally support deleting vertices. Instead, mark all vertices as initially inactive, then activate them in reverse deletion order. When a vertex becomes active, merge it with every active neighbor. Keep the sum of each component and a global maximum component sum.

This is suitable because each edge is considered only a constant number of times and DSU with path compression and union by size is very efficient.

## Input Format Used by This Program
- First line: `n m` — number of vertices and edges.
- Second line: `n` vertex weights, numbered `1` through `n`.
- Next `m` lines: edges `u v`.
- Final line: `n` integers giving the deletion order `p1 p2 ... pn`.

The output prints the maximum component weight sum after each deletion, in the original deletion order.

## Algorithm
1. Read the graph, vertex weights, and deletion order.
2. Mark every vertex inactive.
3. Process vertices in reverse order: `pn, pn-1, ..., p1`.
4. Activate the current vertex and set its component sum to its weight.
5. For each neighbor already active, union the two components.
6. Update the largest component sum.
7. Store the answer for that reverse step.
8. Print stored answers in forward deletion order.

## C Program

```c
#include <stdio.h>

#define MAXV 1005
#define MAXE 10005

int parent[MAXV], componentSize[MAXV];
long long componentSum[MAXV];
int active[MAXV];

int findSet(int vertex) {
    if (parent[vertex] != vertex)
        parent[vertex] = findSet(parent[vertex]);
    return parent[vertex];
}

void unionSets(int a, int b) {
    int rootA = findSet(a);
    int rootB = findSet(b);

    if (rootA == rootB)
        return;

    if (componentSize[rootA] < componentSize[rootB]) {
        int temp = rootA;
        rootA = rootB;
        rootB = temp;
    }

    parent[rootB] = rootA;
    componentSize[rootA] += componentSize[rootB];
    componentSum[rootA] += componentSum[rootB];
}

int main(void) {
    int n, m;
    int weight[MAXV], order[MAXV];
    int head[MAXV], to[2 * MAXE], nextEdge[2 * MAXE];
    int edgeCount = 0;
    long long answer[MAXV];
    long long maximum = 0;

    if (scanf("%d %d", &n, &m) != 2)
        return 1;

    for (int i = 1; i <= n; i++)
        scanf("%d", &weight[i]);

    for (int i = 1; i <= n; i++)
        head[i] = -1;

    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);

        to[edgeCount] = v;
        nextEdge[edgeCount] = head[u];
        head[u] = edgeCount++;

        to[edgeCount] = u;
        nextEdge[edgeCount] = head[v];
        head[v] = edgeCount++;
    }

    for (int i = 1; i <= n; i++)
        scanf("%d", &order[i]);

    for (int i = 1; i <= n; i++) {
        active[i] = 0;
        parent[i] = i;
        componentSize[i] = 1;
        componentSum[i] = 0;
    }

    for (int step = n; step >= 1; step--) {
        int vertex = order[step];
        active[vertex] = 1;
        parent[vertex] = vertex;
        componentSize[vertex] = 1;
        componentSum[vertex] = weight[vertex];

        if (componentSum[vertex] > maximum)
            maximum = componentSum[vertex];

        for (int edge = head[vertex]; edge != -1; edge = nextEdge[edge]) {
            int neighbor = to[edge];

            if (active[neighbor]) {
                unionSets(vertex, neighbor);
                int root = findSet(vertex);
                if (componentSum[root] > maximum)
                    maximum = componentSum[root];
            }
        }

        answer[step] = maximum;
    }

    /*
       answer[step] is the state before deleting order[step].
       After deletion order[1], use answer[n - 1], and so on.
       After all n deletions, the graph is empty, so the answer is 0.
    */
    for (int i = n - 1; i >= 1; i--)
        printf("%lld ", answer[i]);

    printf("0\n");
    return 0;
}
```

## Important Function Explanations
- `findSet(vertex)`: returns the representative (root) of the component containing `vertex`. Path compression makes future searches faster.
- `unionSets(a, b)`: merges the components containing `a` and `b`. Union by size keeps the DSU tree shallow and updates the component size and weight sum.
- `main()`: reads input, stores the undirected graph in adjacency lists, activates vertices in reverse deletion order, merges active neighbors, and prints saved answers.

## Sample Input
```text
5 4
4 2 7 1 3
1 2
2 3
3 4
3 5
4 2 5 1 3
```

## Sample Output
```text
16 10 7 7 0
```

## Dry Run
Graph edges: `1-2`, `2-3`, `3-4`, `3-5`; weights: `[4,2,7,1,3]`; deletion order: `[4,2,5,1,3]`.

The reverse activation order is `3, 1, 5, 2, 4`.

| Reverse step | Activated vertex | Active components after unions | Maximum sum |
|---|---:|---|---:|
| 1 | 3 | `{3}` = 7 | 7 |
| 2 | 1 | `{1}` = 4, `{3}` = 7 | 7 |
| 3 | 5 | `{1}` = 4, `{3,5}` = 10 | 10 |
| 4 | 2 | `{1,2,3,5}` = 16 | 16 |
| 5 | 4 | `{1,2,3,4,5}` = 17 | 17 |

The reverse activation states correspond to the graph before each matching deletion. The program therefore prints the saved values in reverse index order, skipping the original full graph, and appends `0` for the empty graph after the final deletion.

### Important Interpretation Note
The statement says “after each vertex deletion.” For deletion order `4,2,5,1,3`, the graph states are:
- Delete 4: remaining component `{1,2,3,5}` has sum 16.
- Delete 2: components `{1}` and `{3,5}` have maximum 10.
- Delete 5: remaining component `{1}` and `{3}` have maximum 7.
- Delete 1: remaining component `{3}` has sum 7.
- Delete 3: no vertices remain, so define the maximum as 0.

Thus, the sample output is:
```text
16 10 7 7 0
```

## Complexity Analysis
Let `V` be the number of vertices and `E` the number of edges.
- Graph construction: `O(V + E)`.
- Each vertex is activated once and each undirected edge is checked from both adjacency lists.
- DSU operations with path compression and union by size take amortized `O(alpha(V))` each, where `alpha` is the inverse Ackermann function.
- Total time: `O(V + E alpha(V))`, commonly described as nearly linear.
- Space: `O(V + E)` for adjacency lists, DSU arrays, weights, order, and answers.

For a fixed graph, best, average, and worst cases share the same asymptotic upper bound; the exact number of successful unions can vary.

## Complexity Graph
`complexity_graph.png` illustrates the sparse-graph case `O(n alpha(n))` against an `O(n)` reference curve. The inverse Ackermann function `alpha(n)` grows extremely slowly, so the two curves look nearly linear. This is a conceptual illustration, not measured benchmark data. The full bound is `O(V + E alpha(V))`; for dense graphs, `E` can be `O(V^2)`, so the graph is specifically labeled as the sparse-graph case.

## Advantages
- Efficient for large sparse or moderately dense graphs.
- Avoids trying to delete components directly.
- DSU implementation is suitable for a DAA lab.

## Limitations
- This reverse-time technique works when the operation sequence can be reversed into additions.
- The program uses fixed-size arrays; increase `MAXV` and `MAXE` if needed.
- The program assumes valid vertex IDs, a valid deletion permutation, and nonnegative weights for the simple running maximum logic.

## Viva Questions and Answers
1. **What is the main idea?** Reverse the deletion order and add vertices back.
2. **Why use DSU?** It efficiently finds and merges connected components.
3. **Why can't ordinary DSU handle deletion directly?** DSU supports merging, not splitting a component after removal.
4. **What is reverse-time processing?** Solving operations backward so difficult deletions become easy additions.
5. **What does `parent[]` store?** The parent pointer used to find a component representative.
6. **What is path compression?** It makes visited nodes point closer to the root during `findSet`.
7. **What is union by size?** Attach the smaller DSU tree under the larger one.
8. **What does `componentSum[]` store?** The sum of weights in a component, stored at its root.
9. **Why use an adjacency list?** It stores only actual edges and uses `O(V+E)` space.
10. **What is the time complexity?** `O(V + E alpha(V))` amortized.
11. **What is the space complexity?** `O(V+E)`.
12. **What if the graph has no edges?** Each active vertex is its own component.
13. **Why store answers in an array?** Reverse processing computes results in reverse order, so answers must be saved and printed in the required order.
14. **Why is `active[]` needed?** It prevents merging with vertices that have not yet been added back.
15. **Why use `long long` for sums?** Component sums can exceed the range of a 32-bit `int`.

### C-Code Viva Questions
1. **Why is each undirected edge stored twice?** Once for `u -> v` and once for `v -> u`.
2. **Why does the adjacency list use `head`, `to`, and `nextEdge`?** They form a compact linked-list representation using arrays.
3. **Why does `findSet` call itself recursively?** It follows parent pointers until it reaches the root.
4. **Why does `unionSets` return when roots are equal?** Both vertices are already in the same component.
5. **Why is `answer[]` indexed by `step`?** It restores results to the original deletion-order positions.

## Compile and Run
Save the code as `main.c`, then run:
```bash
gcc main.c -o main
./main
```
On Windows with GCC:
```bash
gcc main.c -o main.exe
main.exe
```

## Conclusion
Reverse-time DSU turns vertex deletions into vertex activations. By maintaining connected-component sums while merging active neighbors, the program computes the maximum component weight sum efficiently.
