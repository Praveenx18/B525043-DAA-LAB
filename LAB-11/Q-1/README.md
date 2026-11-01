# Shortest Path on Parity-Constrained Graphs via 0–1 BFS

## Problem statement

Given a directed graph with edge weights 0 or 1, a source vertex `S`, and a non-negative penalty `C`, find the minimum effective cost from `S` to every vertex.

For a path `P`:
- `W(P)` is the sum of its edge weights.
- `k1(P)` is the number of weight-1 edges.
- Its effective cost is `W(P)` if `k1(P)` is even, otherwise `W(P) + C`.

If no path reaches a vertex, report it as unreachable.

## Objective

Compute the minimum effective path cost from the source to every vertex, taking into account whether the path uses an even or odd number of weight-1 edges.

## Algorithm / approach

Use **0–1 BFS on a state-expanded graph**.

A normal vertex is not enough to represent the situation: reaching the same vertex with an even number of weight-1 edges and reaching it with an odd number may lead to different final penalties. Therefore, represent each original vertex `v` using two states:
- `(v, 0)`: an even number of weight-1 edges has been used.
- `(v, 1)`: an odd number of weight-1 edges has been used.

When traversing an edge of weight `w`, update parity using `newParity = parity XOR w`. The edge adds `w` to the path weight. Since every edge weight is 0 or 1, 0–1 BFS finds the minimum weight for every state efficiently.

For each vertex `v`, the final answer is:

`min(dist[v][0], dist[v][1] + C)`

An unreachable state is ignored. If both states are unreachable, the vertex is unreachable.

## Why this approach is suitable

- 0–1 BFS is designed for graphs whose edge weights are only 0 or 1.
- The two-state representation records the parity condition without enumerating all possible paths.
- It runs in linear time in the size of the graph, `O(V + E)`.

## Step-by-step algorithm

1. Read the number of vertices `n`, edges `m`, source `S`, and penalty `C`.
2. Store each directed edge in an adjacency list.
3. Set all state distances to infinity, except `dist[S][0] = 0`.
4. Put the source-even state into a deque.
5. Remove a state `(u, parity)` from the front.
6. For every outgoing edge `(u, v, w)`, compute `newParity = parity XOR w`.
7. If `dist[u][parity] + w` improves `dist[v][newParity]`, update it.
8. If `w == 0`, push the new state to the front of the deque; otherwise, push it to the back.
9. After the deque is empty, compare the even-state distance with the odd-state distance plus `C`.
10. Print the minimum effective cost, or `Unreachable` if neither state can be reached.

## Complete C program

```c
#include <stdio.h>

#define MAXV 1005
#define MAXE 10005
#define MAXQ (4 * MAXE + 2 * MAXV + 10)
#define INF 4000000000000000000LL

int head[MAXV], to[2 * MAXE], edgeWeight[2 * MAXE], nextEdge[2 * MAXE];
int edgeCount = 0;

int deque[MAXQ];
int front = MAXQ / 2, back = MAXQ / 2, dequeSize = 0;

long long dist[MAXV][2];

void addEdge(int u, int v, int w) {
    to[edgeCount] = v;
    edgeWeight[edgeCount] = w;
    nextEdge[edgeCount] = head[u];
    head[u] = edgeCount;
    edgeCount++;
}

void pushFront(int state) {
    front = (front - 1 + MAXQ) % MAXQ;
    deque[front] = state;
    dequeSize++;
}

void pushBack(int state) {
    deque[back] = state;
    back = (back + 1) % MAXQ;
    dequeSize++;
}

int popFront(void) {
    int state = deque[front];
    front = (front + 1) % MAXQ;
    dequeSize--;
    return state;
}

void zeroOneBFS(int n, int source) {
    int i, u, parity, v, w, newParity, state;
    for (i = 1; i <= n; i++) {
        dist[i][0] = INF;
        dist[i][1] = INF;
    }

    front = back = MAXQ / 2;
    dequeSize = 0;

    dist[source][0] = 0;
    pushFront(2 * (source - 1));

    while (dequeSize > 0) {
        state = popFront();
        u = state / 2 + 1;
        parity = state % 2;

        for (i = head[u]; i != -1; i = nextEdge[i]) {
            v = to[i];
            w = edgeWeight[i];
            newParity = parity ^ w;

            if (dist[u][parity] + w < dist[v][newParity]) {
                dist[v][newParity] = dist[u][parity] + w;
                state = 2 * (v - 1) + newParity;

                if (w == 0)
                    pushFront(state);
                else
                    pushBack(state);
            }
        }
    }
}

int main(void) {
    int n, m, source, u, v, w, i;
    long long penalty, evenCost, oddCost, answer;

    printf("Enter number of vertices, edges, source, and penalty C: ");
    if (scanf("%d %d %d %lld", &n, &m, &source, &penalty) != 4)
        return 0;

    if (n < 1 || n >= MAXV || m < 0 || m > MAXE ||
        source < 1 || source > n || penalty < 0) {
        printf("Invalid input limits or values.\n");
        return 0;
    }

    for (i = 1; i <= n; i++)
        head[i] = -1;

    printf("Enter each directed edge as: from to weight(0 or 1)\n");
    for (i = 0; i < m; i++) {
        if (scanf("%d %d %d", &u, &v, &w) != 3)
            return 0;
        if (u < 1 || u > n || v < 1 || v > n || (w != 0 && w != 1)) {
            printf("Invalid edge input.\n");
            return 0;
        }
        addEdge(u, v, w);
    }

    zeroOneBFS(n, source);

    printf("\nMinimum effective cost from source %d:\n", source);
    for (i = 1; i <= n; i++) {
        evenCost = dist[i][0];
        oddCost = (dist[i][1] == INF) ? INF : dist[i][1] + penalty;

        if (evenCost == INF && oddCost == INF) {
            printf("Vertex %d: Unreachable\n", i);
        } else {
            answer = (evenCost < oddCost) ? evenCost : oddCost;
            printf("Vertex %d: %lld\n", i, answer);
        }
    }

    return 0;
}
```

## Important functions

- `addEdge(u, v, w)`: adds a directed edge to the adjacency list.
- `pushFront(state)`: inserts a state at the front of the deque. It is used for weight-0 edges.
- `pushBack(state)`: inserts a state at the back of the deque. It is used for weight-1 edges.
- `popFront()`: removes and returns the front state.
- `zeroOneBFS(n, source)`: calculates the shortest base weight for each vertex-parity state.
- `main()`: reads and validates input, builds the graph, runs the algorithm, and prints the final effective costs.

## Important variables

- `head[u]`: index of the first outgoing edge for vertex `u`.
- `to[]`, `edgeWeight[]`, `nextEdge[]`: arrays forming the adjacency list.
- `dist[v][0]`: minimum base weight to `v` using an even number of weight-1 edges.
- `dist[v][1]`: minimum base weight to `v` using an odd number of weight-1 edges.
- `penalty`: extra cost `C` added only to odd-parity paths.
- `deque[]`: stores states waiting to be processed.
- `parity`: `0` means even; `1` means odd.

## Sample input

```text
4 5 1 3
1 2 0
1 3 1
2 3 1
2 4 1
3 4 0
```

## Sample output

```text
Enter number of vertices, edges, source, and penalty C: Enter each directed edge as: from to weight(0 or 1)

Minimum effective cost from source 1:
Vertex 1: 0
Vertex 2: 0
Vertex 3: 4
Vertex 4: 4
```

The prompt text and output prompts may appear on the same line depending on the terminal.

## Dry run

Use this graph with source `1` and penalty `C = 3`:

- `1 -> 2` with weight 0
- `1 -> 3` with weight 1
- `2 -> 3` with weight 1
- `2 -> 4` with weight 1
- `3 -> 4` with weight 0

Initially, `dist[1][0] = 0`; all other state distances are infinity.

| State reached | Base distance | Explanation |
|---|---:|---|
| `(1, even)` | 0 | Start at source |
| `(2, even)` | 0 | Edge `1 -> 2` has weight 0 |
| `(3, odd)` | 1 | Edge `1 -> 3` has weight 1 |
| `(3, odd)` | 1 | Also reachable by `1 -> 2 -> 3`, cost `0 + 1` |
| `(4, odd)` | 1 | Path `1 -> 2 -> 4`, cost `0 + 1` |
| `(4, odd)` | 1 | Path `1 -> 3 -> 4`, cost `1 + 0` |

Final comparison for each vertex:

| Vertex | Best even base cost | Best odd base cost + penalty | Answer |
|---|---:|---:|---:|
| 1 | 0 | Unreachable | 0 |
| 2 | 0 | Unreachable | 0 |
| 3 | Unreachable | `1 + 3 = 4` | 4 |
| 4 | Unreachable | `1 + 3 = 4` | 4 |

For vertices 3 and 4, the best reachable parity is odd with base cost 1. Adding the penalty `C = 3` gives an effective cost of 4 for each.

## Complexity analysis

The expanded graph has `2V` states and `2E` state transitions (each original edge can be used from each parity state). 0–1 BFS processes these states and transitions in `O(V + E)` time.

- **Best case:** `O(V + E)` for the algorithm's initialization and graph traversal; even if few relaxations succeed, the adjacency lists and distance array still need initialization/inspection.
- **Average case:** `O(V + E)`.
- **Worst case:** `O(V + E)`, the standard bound for 0–1 BFS with edge weights 0 and 1.
- **Space:** `O(V + E)` for adjacency lists, two distances per vertex, and the deque.

Here `V` is the number of vertices and `E` is the number of directed edges. If a single input-size variable `n` represents the total graph size, the growth is linear in `V + E`, not merely in `V`.

## Complexity graph

The file `complexity_graph.png` plots normalized linear growth, representing the `O(V + E)` bound when input size is treated as the combined graph size. The `O(n log n)` curve is included only as a visual reference. These are theoretical growth curves, not measured run-time data.

## Advantages

- Efficient for graphs with 0/1 edge weights.
- Handles directed graphs.
- Correctly distinguishes even and odd counts of weight-1 edges.
- Reports unreachable vertices.
- Avoids trying every possible path.

## Disadvantages / limitations

- The graph must have edge weights exactly 0 or 1 for 0–1 BFS to apply.
- The two-state model is a little less intuitive than ordinary BFS.
- This sample program uses fixed array limits (`MAXV` and `MAXE`); increase them if larger input is needed.
- The extra penalty is assumed to be a non-negative integer in this C implementation.

## Viva preparation: questions and answers

1. **What is the main idea?**  
   Run 0–1 BFS on two states for every vertex: even and odd parity.

2. **Why use 0–1 BFS?**  
   Because each edge weight is either 0 or 1, so a deque can process zero-cost edges before one-cost edges efficiently.

3. **Why are there two states per vertex?**  
   The final penalty depends on whether the number of weight-1 edges is even or odd.

4. **What does `dist[v][0]` mean?**  
   The minimum sum of edge weights to `v` with even parity.

5. **What does `dist[v][1]` mean?**  
   The minimum sum of edge weights to `v` with odd parity.

6. **How is parity updated?**  
   With `newParity = parity XOR w`. A weight-0 edge preserves parity; a weight-1 edge flips it.

7. **When is the penalty added?**  
   Only when calculating the final answer for the odd-parity state.

8. **What if a vertex is unreachable?**  
   Both parity-state distances remain infinity, so the program prints `Unreachable`.

9. **What is the time complexity?**  
   `O(V + E)` for 0–1 BFS on the expanded graph.

10. **What is the space complexity?**  
    `O(V + E)` for the adjacency list, distances, and deque.

11. **How is this better than checking all paths?**  
    Enumerating paths can be exponential; 0–1 BFS reuses the best known distance to each state.

12. **Can ordinary BFS be used?**  
    Ordinary BFS assumes all edges have equal cost. Here edge weights differ, so 0–1 BFS is suitable.

13. **Why push weight-0 edges to the front?**  
    They do not increase distance, so processing them first maintains the 0–1 BFS ordering.

14. **What is the role of `addEdge()`?**  
    It stores a directed edge in the adjacency list.

15. **What does `^` mean in `parity ^ w`?**  
    In C, `^` is bitwise XOR. For values 0 and 1, it flips parity when `w` is 1.

### Extra C-code viva questions

1. **Why use `long long` for distances?**  
   It supports larger costs than a typical 32-bit `int`.

2. **Why initialize `head[]` to `-1`?**  
   It marks that a vertex has no outgoing edge yet.

3. **Why use `nextEdge[]`?**  
   It links edges in the adjacency list without needing a separate linked-list structure.

4. **Why use `dequeSize`?**  
   It tells the loop whether any states remain to process.

5. **Why check the input with `scanf()`?**  
   It helps stop if the expected input values were not provided.

## How to compile and run

Save the code as `main.c`, then compile:

```bash
gcc main.c -o main
```

Run on Linux/macOS:

```bash
./main
```

On Windows with GCC:

```bash
main.exe
```

Enter `n m source C`, followed by `m` lines containing `u v w`.

## Conclusion

This solution uses a two-state representation to track parity and 0–1 BFS to find minimum base weights efficiently. Adding the penalty to the odd-parity result gives the minimum effective cost for every reachable vertex.
