# Online Graph Bipartiteness Testing

## 1. Problem Statement

An undirected graph receives edges one by one. For each incoming edge `(u, v)`, accept it if adding it keeps the graph bipartite. Otherwise, report that the edge violates bipartiteness and do not keep it.

A graph is bipartite when its vertices can be divided into two sets such that every edge connects vertices from different sets. Equivalently, an undirected graph is bipartite if and only if it contains no odd-length cycle.

## 2. Objective

To implement online bipartiteness testing in C using an adjacency matrix and Breadth-First Search (BFS) coloring.

## 3. Approach

We use two colors, represented by `0` and `1`.

For each new edge:
1. Temporarily add the edge to the adjacency matrix.
2. Run BFS to color every connected component.
3. When visiting an edge, give an uncolored neighbor the opposite color.
4. If two adjacent vertices have the same color, the graph is not bipartite.
5. Keep the edge if the graph is bipartite; otherwise, remove it.

We check every connected component because the graph may be disconnected.

### Why this approach?

BFS coloring is easy to understand and correctly detects whether a graph is bipartite. The adjacency matrix also makes insertion and removal of an edge simple. The trade-off is that scanning the matrix takes O(V²) time per check, so this is a simple teaching implementation rather than an optimized online data structure.

## 4. Step-by-Step Algorithm

1. Read the number of vertices `V` and the number of incoming edges `E`.
2. Initialize every entry of the `V × V` adjacency matrix to zero.
3. For each incoming edge `(u, v)`:
   - If it already exists, report that it is ignored.
   - Otherwise, add it in both directions.
   - Call `isBipartite(V)`.
   - If the function returns `1`, keep the edge.
   - If it returns `0`, remove the edge in both directions.
4. Print the final adjacency matrix.

## 5. C Program

```c
#include <stdio.h>

#define MAX 100

int graph[MAX][MAX];

/* Returns 1 if the graph is bipartite, otherwise returns 0. */
int isBipartite(int vertices) {
    int color[MAX];
    int queue[MAX];
    int front, rear;
    int i, u, v;

    /* -1 means that the vertex has not been colored yet. */
    for (i = 0; i < vertices; i++) {
        color[i] = -1;
    }

    /* Check every component, including disconnected components. */
    for (i = 0; i < vertices; i++) {
        if (color[i] == -1) {
            front = 0;
            rear = 0;

            queue[rear++] = i;
            color[i] = 0;

            while (front < rear) {
                u = queue[front++];

                for (v = 0; v < vertices; v++) {
                    if (graph[u][v] == 1) {
                        if (color[v] == -1) {
                            color[v] = 1 - color[u];
                            queue[rear++] = v;
                        } else if (color[v] == color[u]) {
                            return 0;
                        }
                    }
                }
            }
        }
    }

    return 1;
}

int main(void) {
    int vertices, edges;
    int u, v, i;

    for (i = 0; i < MAX; i++) {
        int j;
        for (j = 0; j < MAX; j++) {
            graph[i][j] = 0;
        }
    }

    printf("Enter number of vertices and edges to process: ");
    if (scanf("%d %d", &vertices, &edges) != 2 ||
        vertices < 1 || vertices > MAX || edges < 0) {
        printf("Invalid input. Vertices must be between 1 and %d.\n", MAX);
        return 1;
    }

    printf("Enter each edge as two vertex numbers (0 to %d):\n", vertices - 1);

    for (i = 0; i < edges; i++) {
        printf("Edge %d: ", i + 1);

        if (scanf("%d %d", &u, &v) != 2 ||
            u < 0 || u >= vertices || v < 0 || v >= vertices) {
            printf("Invalid edge. Use vertex numbers from 0 to %d.\n",
                   vertices - 1);
            return 1;
        }

        if (graph[u][v] == 1) {
            printf("Edge (%d, %d) already exists. Ignored.\n", u, v);
            continue;
        }

        /* Temporarily add the undirected edge. */
        graph[u][v] = 1;
        graph[v][u] = 1;

        if (isBipartite(vertices)) {
            printf("Edge (%d, %d): Accepted. Graph remains bipartite.\n",
                   u, v);
        } else {
            /* Reject the edge by removing it from the matrix. */
            graph[u][v] = 0;
            graph[v][u] = 0;

            printf("Edge (%d, %d): Rejected. It violates bipartiteness.\n",
                   u, v);
        }
    }

    printf("\nFinal accepted edges (adjacency matrix):\n");
    for (i = 0; i < vertices; i++) {
        int j;
        for (j = 0; j < vertices; j++) {
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }

    return 0;
}
```

## 6. Important Functions and Variables

### `isBipartite(int vertices)`

- **Parameter:** `vertices`, the number of vertices.
- **Returns:** `1` if the graph is bipartite; `0` otherwise.
- Initializes each color to `-1` (uncolored).
- Starts BFS from each uncolored vertex so disconnected components are checked too.
- Assigns the opposite color to each uncolored neighbor.
- Returns `0` immediately if adjacent vertices have the same color.

### Important variables

- `graph[MAX][MAX]`: adjacency matrix; `graph[u][v] == 1` means an edge exists.
- `color[MAX]`: stores each vertex's color (`0`, `1`, or `-1` for uncolored).
- `queue[MAX]`: stores vertices waiting to be processed by BFS.
- `front`: points to the next queue item to process.
- `rear`: indicates where the next vertex is inserted.
- `u`, `v`: vertices at the ends of the current edge.

### Main insertion logic

The edge is added in both directions because the graph is undirected. If BFS detects a conflict, both matrix entries are reset to zero. Therefore, a rejected edge does not remain in the graph.

## 7. Sample Input

```text
4 5
0 1
1 2
2 0
2 3
0 3
```

There are 4 vertices (`0` to `3`) and 5 incoming edges.

## 8. Sample Output

```text
Enter number of vertices and edges to process: 4 5
Enter each edge as two vertex numbers (0 to 3):
Edge 1: 0 1
Edge (0, 1): Accepted. Graph remains bipartite.
Edge 2: 1 2
Edge (1, 2): Accepted. Graph remains bipartite.
Edge 3: 2 0
Edge (2, 0): Rejected. It violates bipartiteness.
Edge 4: 2 3
Edge (2, 3): Accepted. Graph remains bipartite.
Edge 5: 0 3
Edge (0, 3): Accepted. Graph remains bipartite.

Final accepted edges (adjacency matrix):
0 1 0 1
1 0 1 0
0 1 0 1
1 0 1 0
```

## 9. Dry Run

Initially, all matrix entries are zero.

| Incoming edge | Decision | Reason |
|---|---|---|
| `(0, 1)` | Accept | Two vertices can receive opposite colors. |
| `(1, 2)` | Accept | The path can be colored `0, 1, 0`. |
| `(2, 0)` | Reject | It would form triangle `0–1–2–0`, an odd cycle. |
| `(2, 3)` | Accept | Vertex 3 can be assigned the opposite color to vertex 2. |
| `(0, 3)` | Accept | Vertices 0 and 3 have opposite colors; this makes an even cycle. |

After the rejected edge `(2, 0)`, it is removed from the matrix. The remaining accepted edges form the path `0–1–2–3`. Adding `(0, 3)` creates a cycle of length 4, which is even, so it is accepted.

One possible coloring after all accepted insertions is:
- Color 0: vertices `0` and `2`
- Color 1: vertices `1` and `3`

Every accepted edge connects opposite colors.

## 10. Complexity Analysis

Let:
- `V` = number of vertices
- `E` = number of incoming edges

### Time complexity per incoming edge

`isBipartite()` initializes the color array in O(V). BFS may process up to V vertices. For each processed vertex, the adjacency-matrix loop scans V possible neighbors. Therefore, one complete check takes **O(V²)** time.

- Best case for a particular check: **O(V²)** for this implementation, because the code initializes colors and may scan rows even when few edges exist.
- Average case: **O(V²)** as a conservative bound; actual work can be lower if a color conflict is found early.
- Worst case: **O(V²)**.

### Total time for all insertions

Each of the E incoming edges can trigger one full check. Thus, the total upper bound is **O(EV²)**.

For a simple undirected graph, `E` can be O(V²). In the dense worst case, the total becomes **O(V⁴)**.

### Space complexity

- Adjacency matrix: O(V²)
- Color array: O(V)
- BFS queue: O(V)

The matrix dominates, so total auxiliary storage is **O(V²)**.

## 11. Complexity Graph

The accompanying `complexity_graph.png` shows theoretical growth, not measured execution times:

- `O(V²)`: one bipartiteness check per incoming edge.
- `O(V⁴)`: worst-case total when there can be O(V²) incoming edges and each check takes O(V²).

The curves are illustrative operation-growth indicators used to compare asymptotic rates. They are not benchmark measurements.

## 12. Advantages

- Easy to understand and implement in C.
- BFS detects odd-cycle conflicts through two-coloring.
- Correctly handles disconnected graphs.
- Rejected edges are removed immediately.
- The adjacency matrix makes edge insertion and rejection simple.

## 13. Disadvantages and Limitations

- Each insertion can require checking the entire graph again.
- The adjacency matrix uses O(V²) memory, even for sparse graphs.
- The program supports at most 100 vertices; increase `MAX` if needed, while considering memory use.
- It ignores duplicate edges rather than processing them again.
- It is less efficient than specialized online approaches based on disjoint-set union with parity or other dynamic graph techniques.

## 14. Viva Questions and Answers

1. **What is a bipartite graph?**  
   A graph whose vertices can be divided into two sets such that every edge connects different sets.

2. **What theorem connects bipartite graphs and cycles?**  
   An undirected graph is bipartite if and only if it has no odd-length cycle.

3. **Which algorithm is used here?**  
   BFS with two-coloring.

4. **Why are two colors sufficient?**  
   Every edge must connect vertices in opposite sets, represented by colors 0 and 1.

5. **Why do we use BFS?**  
   BFS visits vertices level by level and lets us assign opposite colors to adjacent vertices.

6. **Why check every uncolored vertex?**  
   The graph may contain disconnected components.

7. **What does color `-1` mean?**  
   The vertex has not yet been assigned a color.

8. **Why is the graph checked after temporarily adding an edge?**  
   We need to test whether that particular insertion causes a conflict.

9. **What happens when the edge is rejected?**  
   Both corresponding adjacency-matrix entries are reset to zero.

10. **What is the time complexity of one check?**  
    O(V²), because the adjacency-matrix neighbor loop scans V entries for each of up to V vertices.

11. **What is the total time complexity?**  
    O(EV²); in the dense worst case, O(V⁴).

12. **What is the space complexity?**  
    O(V²), mainly due to the adjacency matrix.

13. **Why use an adjacency matrix instead of an adjacency list?**  
    It makes insertion and removal easy to express, though it consumes more memory on sparse graphs.

14. **Why do we set both `graph[u][v]` and `graph[v][u]`?**  
    Because an undirected edge works in both directions.

15. **How could the solution be optimized?**  
    Specialized online algorithms, such as a disjoint-set union structure with parity, can avoid recoloring the entire graph after every insertion.

### C-code-specific viva questions

- **Why is `queue[MAX]` used?**  
  BFS needs a queue to process discovered vertices in first-in, first-out order.

- **Why is the condition `color[v] == -1` checked?**  
  It prevents assigning a color to a vertex more than once during BFS.

- **What does `color[v] = 1 - color[u]` do?**  
  It assigns the opposite color: if `u` is 0, `v` becomes 1, and vice versa.

- **Why does `isBipartite()` return 0 immediately on a conflict?**  
  One same-colored adjacent pair is enough to prove that the current graph is not bipartite.

- **Why does the program validate vertex numbers?**  
  To prevent invalid array indexing and memory errors.

## 15. Compile and Run

Save the source code as `online_bipartite.c`.

Using GCC:

```bash
gcc online_bipartite.c -o online_bipartite
./online_bipartite
```

Enter the number of vertices and incoming edges, then enter each edge using vertex labels from `0` to `V-1`.

## 16. Conclusion

This program demonstrates online graph bipartiteness testing by temporarily inserting each edge and checking the graph using BFS two-coloring. An edge is kept only if the graph remains bipartite. The method is easy to understand and validate, although repeatedly scanning an adjacency matrix makes it less efficient for large graphs.
