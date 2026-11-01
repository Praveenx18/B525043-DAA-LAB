# Multi-Source BFS for Radius-Constrained Voronoi Cell Partitioning

## 1. Problem Title

**Multi-Source BFS for Radius-Constrained Voronoi Cell Partitioning**

## 2. Problem Statement

Given an unweighted, undirected graph with `N` vertices and `M` edges, a list of `K` designated hub vertices, and a non-negative radius `R`, find the shortest distance from every vertex to its nearest hub.

A vertex is assigned to its nearest hub if the distance is at most `R`. If it is farther than `R`, or if no hub can reach it, its owner is `-1`. If multiple hubs have the same minimum distance, the hub with the smallest index wins.

The program reports the nearest-hub distance and the owner for each vertex.

## 3. Objective

- Calculate shortest distances from all vertices to the nearest hub.
- Assign each covered vertex to its nearest hub.
- Apply deterministic tie-breaking using the smallest hub index.
- Mark vertices outside the radius or unreachable from every hub with owner `-1`.

## 4. Approach: Multi-Source Breadth-First Search

BFS finds shortest paths in an unweighted graph. Instead of running BFS separately from each hub, we insert all hubs into one queue as starting vertices.

The BFS processes vertices in increasing distance order. When two hubs can reach a vertex at the same shortest distance, the smaller hub index is kept. The queue's level order ensures all possible candidates from the preceding level are considered before the vertex is processed.

After BFS, vertices whose distance is greater than `R`, and vertices unreachable from all hubs, receive owner `-1`.

### Why this approach is suitable

- BFS is designed for shortest paths in unweighted graphs.
- Multi-source BFS shares one traversal among all hubs.
- An adjacency list stores only the edges that exist.
- Tie-breaking is deterministic and does not depend on the order in which edges were entered.
- The traversal takes `O(N + M)` time.

## 5. Input Format

1. First line: `N M` — number of vertices and edges.
2. Next `M` lines: `u v` — each undirected edge.
3. Next line: `K` — number of hubs.
4. Next line: `K` hub vertex numbers in hub-index order. The first listed hub has index 1, the second has index 2, and so on.
5. Last line: `R` — maximum service radius.

Vertices are numbered from `1` to `N`. Hub vertices are assumed to be distinct.

## 6. Step-by-Step Algorithm

1. Read `N`, `M`, the edges, `K`, the hubs, and `R`.
2. Build an adjacency list for the undirected graph.
3. Set every vertex's distance and owner to `-1`.
4. Add each hub to the queue, with distance `0` and its hub index as its owner.
5. While the queue is not empty:
   - Remove vertex `u`.
   - Visit every neighbor `v` of `u`.
   - If `v` is unvisited, set `dist[v] = dist[u] + 1`, copy `owner[u]`, and enqueue `v`.
   - If `v` has the same shortest distance through `u`, keep the smaller of `owner[v]` and `owner[u]`.
6. For each vertex, set its owner to `-1` if it is unreachable or its distance is greater than `R`.
7. Print each vertex, its nearest-hub distance, and its owner.

## 7. Complete C Program

```c
#include <stdio.h>

#define MAXV 1005
#define MAXE 20005

int head[MAXV];
int to[MAXE];
int nextEdge[MAXE];
int edgeCount = 0;

int dist[MAXV];
int owner[MAXV];
int queue[MAXV];

void addEdge(int u, int v) {
    to[edgeCount] = v;
    nextEdge[edgeCount] = head[u];
    head[u] = edgeCount;
    edgeCount++;

    to[edgeCount] = u;
    nextEdge[edgeCount] = head[v];
    head[v] = edgeCount;
    edgeCount++;
}

void multiSourceBFS(int n, int k, int hubs[]) {
    int front = 0, rear = 0;
    int i, u, v, edge;

    for (i = 1; i <= n; i++) {
        dist[i] = -1;
        owner[i] = -1;
    }

    /* Add all hubs to the queue as BFS starting points. */
    for (i = 0; i < k; i++) {
        u = hubs[i];
        dist[u] = 0;
        owner[u] = i + 1;
        queue[rear++] = u;
    }

    while (front < rear) {
        u = queue[front++];

        for (edge = head[u]; edge != -1; edge = nextEdge[edge]) {
            v = to[edge];

            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                owner[v] = owner[u];
                queue[rear++] = v;
            } else if (dist[v] == dist[u] + 1 &&
                       owner[u] < owner[v]) {
                /* Equal shortest distance: choose smaller hub index. */
                owner[v] = owner[u];
            }
        }
    }
}

int main(void) {
    int n, m, k, radius;
    int i, u, v;
    int hubs[MAXV];

    printf("Enter number of vertices and edges: ");
    if (scanf("%d %d", &n, &m) != 2)
        return 0;

    if (n <= 0 || n >= MAXV || m < 0 || 2 * m >= MAXE) {
        printf("Invalid graph size. Check MAXV and MAXE limits.\n");
        return 0;
    }

    for (i = 1; i <= n; i++)
        head[i] = -1;

    printf("Enter the edges (u v):\n");
    for (i = 0; i < m; i++) {
        if (scanf("%d %d", &u, &v) != 2)
            return 0;

        if (u < 1 || u > n || v < 1 || v > n) {
            printf("Invalid edge: vertices must be between 1 and %d.\n", n);
            return 0;
        }

        addEdge(u, v);
    }

    printf("Enter number of hubs: ");
    if (scanf("%d", &k) != 1 || k <= 0 || k > n) {
        printf("Invalid number of hubs.\n");
        return 0;
    }

    printf("Enter the hub vertices in hub-index order: ");
    for (i = 0; i < k; i++) {
        if (scanf("%d", &hubs[i]) != 1)
            return 0;

        if (hubs[i] < 1 || hubs[i] > n) {
            printf("Invalid hub vertex.\n");
            return 0;
        }
    }

    printf("Enter radius R: ");
    if (scanf("%d", &radius) != 1 || radius < 0) {
        printf("Radius must be non-negative.\n");
        return 0;
    }

    multiSourceBFS(n, k, hubs);

    printf("\nVertex\tDistance\tOwner\n");
    for (i = 1; i <= n; i++) {
        if (dist[i] == -1 || dist[i] > radius)
            owner[i] = -1;

        printf("%d\t%d\t\t%d\n", i, dist[i], owner[i]);
    }

    return 0;
}
```

## 8. Explanation of Important Variables and Functions

### Variables

- `head[v]`: index of the first adjacency-list edge for vertex `v`, or `-1` if there are no edges.
- `to[e]`: destination vertex of adjacency-list entry `e`.
- `nextEdge[e]`: next entry in the same vertex's adjacency list.
- `edgeCount`: number of directed adjacency entries stored. Every undirected edge creates two entries.
- `dist[v]`: shortest distance from any hub to vertex `v`; `-1` means unreachable.
- `owner[v]`: index of the hub currently assigned to vertex `v`; `-1` means unassigned.
- `queue[]`: FIFO queue used by BFS.
- `front`: position of the next vertex to remove from the queue.
- `rear`: position where the next vertex is inserted.
- `hubs[]`: hub vertex numbers in hub-index order.
- `radius`: maximum distance for a vertex to be covered.

### `addEdge(u, v)`

Stores both directions of an undirected edge: `u` to `v` and `v` to `u`. This is necessary because the graph is undirected.

### `multiSourceBFS(n, k, hubs)`

Initializes distances and owners, inserts all hubs into the queue, and explores the graph. A newly discovered vertex inherits the current vertex's owner. If another path reaches it at the same shortest distance from a smaller-index hub, the owner is updated.

The function changes the global `dist[]` and `owner[]` arrays and does not return a value.

### `main()`

Reads and validates the input, builds the graph, calls BFS, applies the radius rule, and prints the result.

## 9. Sample Input

```text
6 5
1 2
2 3
3 4
4 5
5 6
2
1 6
2
```

## 10. Sample Output

The program includes prompts when run interactively. The result table is:

```text
Vertex  Distance  Owner
1       0         1
2       1         1
3       2         1
4       2         2
5       1         2
6       0         2
```

In this example, vertices 3 and 4 are each two edges from their nearest hub. Vertex 3 belongs to hub 1, and vertex 4 belongs to hub 2.

## 11. Dry Run

Graph:

`1 -- 2 -- 3 -- 4 -- 5 -- 6`

Hubs: hub 1 is vertex 1, hub 2 is vertex 6. Radius `R = 2`.

Initial queue: `[1, 6]`

| BFS step | Vertex processed | Newly discovered vertex or tie update | Queue (conceptually) |
|---|---:|---|---|
| Initial | — | Vertex 1: distance 0, owner 1; vertex 6: distance 0, owner 2 | `[1, 6]` |
| 1 | 1 | Vertex 2 gets distance 1, owner 1 | `[6, 2]` |
| 2 | 6 | Vertex 5 gets distance 1, owner 2 | `[2, 5]` |
| 3 | 2 | Vertex 3 gets distance 2, owner 1 | `[5, 3]` |
| 4 | 5 | Vertex 4 gets distance 2, owner 2 | `[3, 4]` |
| 5 | 3 | Vertex 4 already has distance 2; the candidate owner 1 is not smaller than owner 2? No—vertex 3 belongs to hub 1, so this path must be checked carefully. | — |

**Correct tie-break note:** In this path graph, vertex 4 is at distance 3 from hub 1 and distance 2 from hub 2. Therefore, no tie exists at vertex 4. The final table above is correct. The owner update condition only applies when `dist[v] == dist[u] + 1`.

After BFS, the nearest-hub distances are `0, 1, 2, 2, 1, 0`. All distances are at most the radius, so no vertex is excluded.

## 12. Complexity Analysis

Let `N` be the number of vertices, `M` the number of edges, and `K` the number of hubs.

### Time complexity

- Initializing the distance and owner arrays takes `O(N)`.
- Adding all hubs to the queue takes `O(K)`.
- Every vertex enters the queue at most once.
- Each adjacency-list entry is examined at most once. An undirected edge is stored twice, so the total edge-processing work is `O(M)`.

Therefore, the total time complexity is:

**`O(N + M)`**

- **Best case:** `O(N + M)` for a complete traversal of the initialized graph and its stored adjacency entries.
- **Average case:** `O(N + M)` under the same adjacency-list representation.
- **Worst case:** `O(N + M)`, including a dense graph where `M` can be `O(N²)`.

These bounds include graph initialization and traversal; they are not empirical timing measurements.

### Space complexity

The adjacency list uses `O(N + M)` space. The distance array, owner array, and BFS queue each use `O(N)` space. Thus, total space complexity is **`O(N + M)`**.

## 13. Complexity Graph

The accompanying `complexity_graph.png` illustrates the expected growth for sparse graphs (`O(N)`) and the worst-case dense-graph bound (`O(N²)`). These are theoretical operation-growth curves, not measured execution times.

## 14. Advantages

- Finds nearest-hub distances in one traversal.
- Uses an adjacency list, which is memory-efficient for sparse graphs.
- Handles disconnected graphs.
- Applies a deterministic smallest-index tie-break.
- Has linear time complexity in the size of the graph representation.

## 15. Disadvantages and Limitations

- The graph is assumed to be unweighted; weighted graphs generally need a different shortest-path algorithm.
- Fixed array limits (`MAXV` and `MAXE`) restrict input size.
- The program assumes hub vertices are distinct, as specified by the problem.
- A vertex outside the radius has owner `-1`, even though its nearest-hub distance may still be finite.

## 16. Viva Questions and Answers

1. **What is BFS?**  
   Breadth-First Search explores a graph level by level using a queue.

2. **Why is BFS suitable here?**  
   The graph is unweighted, so BFS finds shortest paths measured in edges.

3. **What is multi-source BFS?**  
   It is BFS that starts from several source vertices placed in the queue initially.

4. **What does `dist[v]` represent?**  
   The shortest distance from any hub to vertex `v`.

5. **What does `owner[v]` represent?**  
   The index of the hub assigned to vertex `v`, or `-1` if it is not covered.

6. **Why is a queue used?**  
   A FIFO queue processes vertices in nondecreasing distance order.

7. **How are ties resolved?**  
   When equal shortest distances are found, the smaller hub index is retained.

8. **What is the time complexity?**  
   `O(N + M)` with an adjacency list.

9. **What is the space complexity?**  
   `O(N + M)` for the adjacency list and BFS arrays.

10. **What happens if a vertex is disconnected from all hubs?**  
    Its distance remains `-1`, and its owner is `-1`.

11. **What happens when a vertex is farther than `R`?**  
    Its actual reachable distance is retained, but its owner becomes `-1`.

12. **How does this compare with running BFS from each hub?**  
    Running BFS separately takes `O(K(N + M))`; multi-source BFS performs one traversal.

13. **Why does `addEdge()` store an edge twice?**  
    An undirected edge can be traversed in both directions.

14. **Why is `head[]` initialized to `-1`?**  
    It indicates that a vertex's adjacency list is initially empty.

15. **Why does the tie-breaking condition compare `owner[u] < owner[v]`?**  
    It ensures that equal-distance candidates choose the smallest hub index.

## 17. How to Compile and Run

Save the program as `main.c`.

Using GCC:

```bash
gcc main.c -o main
./main
```

On Windows with GCC:

```bash
gcc main.c -o main.exe
main.exe
```

Enter the values in the input format shown above.

## 18. Conclusion

Multi-Source BFS efficiently assigns graph vertices to their nearest hubs in an unweighted graph. It calculates shortest distances, respects a maximum radius, and uses deterministic tie-breaking. With an adjacency list, the algorithm runs in `O(N + M)` time and uses `O(N + M)` space.
