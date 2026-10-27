# Maximum Weight Component with Size Constraints

## 1. Problem Statement

A graph has `n` vertices. Each vertex has a weight. Edges are added one by one in a specified order. After each insertion, find the maximum total vertex weight among connected components whose number of vertices is at most `K`.

For a component `C`:

- Total weight: `sum of the weights of all vertices in C`
- Size: `number of vertices in C`
- Eligible component: a component with size `<= K`

The objective uses vertex weights. Any edge weights do not affect the answer unless a different objective is specified.

## 2. Objective

Implement the algorithm in C, report the answer after each edge insertion, and analyze its time and space complexity.

## 3. Approach: Disjoint Set Union (DSU)

DSU, also called Union-Find, maintains connected components as edges are inserted.

For each component root, we maintain:
- `parent[root]`: identifies the representative/root of a component.
- `componentSize[root]`: number of vertices in the component.
- `componentWeight[root]`: sum of vertex weights in the component.

When an edge `(u, v)` is inserted, find the roots of `u` and `v`. If they differ, merge the two components. We use union by size and path compression to make DSU operations efficient.

After each insertion, scan all vertices and inspect only component roots. Among roots whose component size is at most `K`, choose the greatest component weight. If none qualifies, print `No eligible component`.

## 4. Step-by-Step Algorithm

1. Read `n`, `m`, and `K`.
2. Read every vertex weight.
3. Initially, each vertex is its own component:
   - `parent[i] = i`
   - `componentSize[i] = 1`
   - `componentWeight[i] = weight[i]`
4. Read the `m` edges in insertion order.
5. For each edge `(u, v)`:
   - Find the roots of `u` and `v`.
   - If the roots differ, merge the components.
   - Scan all component roots.
   - Consider only components with size `<= K`.
   - Print the largest eligible component weight, or report that no eligible component exists.
6. Stop after processing all edges.

## 5. Complete C Program

```c
#include <stdio.h>

#define MAX 1000

int parent[MAX];
int componentSize[MAX];
long long componentWeight[MAX];

int find(int vertex) {
    if (parent[vertex] == vertex) {
        return vertex;
    }

    parent[vertex] = find(parent[vertex]);
    return parent[vertex];
}

void unionComponents(int u, int v) {
    int rootU = find(u);
    int rootV = find(v);

    if (rootU == rootV) {
        return;
    }

    if (componentSize[rootU] < componentSize[rootV]) {
        int temp = rootU;
        rootU = rootV;
        rootV = temp;
    }

    parent[rootV] = rootU;
    componentSize[rootU] += componentSize[rootV];
    componentWeight[rootU] += componentWeight[rootV];
}

int getMaximumWeight(int n, int K, long long *maximumWeight) {
    int found = 0;
    long long best = 0;

    for (int i = 1; i <= n; i++) {
        if (find(i) == i) {
            if (componentSize[i] <= K) {
                if (!found || componentWeight[i] > best) {
                    best = componentWeight[i];
                    found = 1;
                }
            }
        }
    }

    if (found) {
        *maximumWeight = best;
    }

    return found;
}

int main(void) {
    int n, m, K;
    int u, v;
    long long maximumWeight;

    printf("Enter number of vertices, edges, and K: ");
    scanf("%d %d %d", &n, &m, &K);

    if (n < 1 || n >= MAX || m < 0 || K < 1 || K > n) {
        printf("Invalid input. Use 1 <= n < %d, m >= 0, and 1 <= K <= n.\n", MAX);
        return 1;
    }

    printf("Enter the weight of each vertex (1 to %d): ", n);
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &componentWeight[i]);
        parent[i] = i;
        componentSize[i] = 1;
    }

    printf("Enter the edges in insertion order (u v):\n");
    for (int i = 1; i <= m; i++) {
        scanf("%d %d", &u, &v);

        if (u < 1 || u > n || v < 1 || v > n) {
            printf("Invalid edge. Vertex numbers must be between 1 and %d.\n", n);
            return 1;
        }

        unionComponents(u, v);

        printf("After edge %d (%d, %d): ", i, u, v);
        if (getMaximumWeight(n, K, &maximumWeight)) {
            printf("Maximum eligible component weight = %lld\n", maximumWeight);
        } else {
            printf("No eligible component\n");
        }
    }

    return 0;
}
```

## 6. Important Functions and Variables

### `find(vertex)`

Returns the root representing the component containing `vertex`. Path compression makes future searches faster by connecting visited vertices directly to the root.

### `unionComponents(u, v)`

Merges the components containing `u` and `v`. If both already have the same root, no merge is needed. Otherwise, union by size attaches the smaller component to the larger one and updates its size and total weight.

### `getMaximumWeight(n, K, &maximumWeight)`

Scans all vertices, identifies roots, and considers only components with size at most `K`. It returns `1` if a qualifying component exists and stores its maximum weight through the pointer; otherwise it returns `0`.

### Main variables

- `parent[]`: DSU parent links.
- `componentSize[]`: number of vertices in each root's component.
- `componentWeight[]`: total weight of each root's component.
- `n`: number of vertices.
- `m`: number of edge insertions.
- `K`: maximum allowed component size.
- `u`, `v`: endpoints of the current edge.
- `found`: whether an eligible component has been found during the scan.
- `best`: largest eligible weight found so far.

Only root entries of `componentSize[]` and `componentWeight[]` are meaningful after components are merged.

## 7. Sample Input

```text
5 4 3
5 4 8 6 3
1 2
2 3
4 5
3 4
```

The first line gives `n = 5`, `m = 4`, and `K = 3`. The second line gives the five vertex weights. The remaining lines give edges in insertion order.

## 8. Sample Output

```text
Enter number of vertices, edges, and K: Enter the weight of each vertex (1 to 5): Enter the edges in insertion order (u v):
After edge 1 (1, 2): Maximum eligible component weight = 9
After edge 2 (2, 3): Maximum eligible component weight = 17
After edge 3 (4, 5): Maximum eligible component weight = 17
After edge 4 (3, 4): No eligible component
```

Prompts may appear on the same line as typed input in a terminal. The important results are the four lines beginning with `After edge`.

## 9. Dry Run

Vertex weights are:

| Vertex | Weight |
|---|---:|
| 1 | 5 |
| 2 | 4 |
| 3 | 8 |
| 4 | 6 |
| 5 | 3 |

`K = 3`.

| Step | Inserted edge | Components after insertion | Eligible component weights | Answer |
|---|---|---|---|---:|
| 1 | (1, 2) | `{1,2}`, `{3}`, `{4}`, `{5}` | 9, 8, 6, 3 | 9 |
| 2 | (2, 3) | `{1,2,3}`, `{4}`, `{5}` | 17, 6, 3 | 17 |
| 3 | (4, 5) | `{1,2,3}`, `{4,5}` | 17, 9 | 17 |
| 4 | (3, 4) | `{1,2,3,4,5}` | None; size is 5, greater than 3 | No eligible component |

For example, after `(2, 3)`, vertices 1, 2, and 3 form a component with size 3 and weight `5 + 4 + 8 = 17`. It is eligible because its size equals `K`.

After `(3, 4)`, all five vertices belong to one component. Its size is 5, so it is not eligible. There are no other components, so no answer exists for that insertion.

## 10. Complexity Analysis

Let:
- `n` be the number of vertices.
- `m` be the number of edge insertions.
- `α(n)` be the inverse Ackermann function, which grows extremely slowly.

### Time complexity

- Initializing DSU arrays: `O(n)`.
- Each insertion and DSU merge: amortized `O(α(n))`.
- Scanning for the maximum after each edge: `O(n)` per insertion.
- All `m` scans: `O(mn)`.

Total time:

`O(n + m α(n) + mn)`

For a connected simple graph, `m` can be `O(n²)`, so the scan-based implementation can take `O(n³)` in the general dense-graph bound. If `m = O(n)`, the bound becomes `O(n²)`.

### Best, average, and worst cases

For this implementation, every edge insertion triggers a scan of all `n` vertices. Therefore, for fixed `n` and `m`, the scanning cost is `Θ(mn)` in the best, average, and worst cases. DSU work varies slightly, but does not change that dominant bound when `m > 0`.

If there are no edges (`m = 0`), initialization takes `Θ(n)` time and no answer scans occur.

### Space complexity

The DSU arrays use `O(n)` space. The code processes edges one at a time, so it does not store the whole graph. Auxiliary space is `O(n)`.

### Complexity graph

The included PNG plots reference growth curves `O(n)`, `O(n log n)`, and `O(n²)`. The quadratic curve represents the scan-based algorithm when the number of edge insertions is proportional to the number of vertices (`m = Θ(n)`). These are theoretical operation-growth curves, not measured timings.

## 11. Advantages

- Beginner-friendly and suitable for a DAA lab.
- DSU maintains components without rebuilding the graph.
- Path compression and union by size make component operations efficient.
- Processes edges in insertion order.
- Uses `O(n)` memory because edges are not stored.

## 12. Disadvantages and Limitations

- Scans all vertices after every edge, which can be slow for large inputs.
- Uses fixed-size arrays, with `MAX` set to 1000.
- Supports edge additions only; removing edges would require a different approach.
- If no component has size at most `K`, it reports no eligible component.
- Edge weights are ignored because the objective concerns vertex weights.

## 13. Compile and Run

Save the program as `main.c`. Using GCC:

```bash
gcc main.c -o main
./main
```

On Windows with GCC:

```bash
gcc main.c -o main.exe
main.exe
```

Enter the number of vertices, number of edges, `K`, the vertex weights, and then each edge in insertion order.

## 14. Viva Questions and Answers

1. **What is the main idea?**  
   Maintain connected components using DSU and find the maximum weight among components of size at most `K`.

2. **What is DSU?**  
   Disjoint Set Union is a data structure that maintains separate sets and supports finding and merging sets.

3. **Why use DSU?**  
   Edges are only added, so DSU can efficiently maintain components as the graph grows.

4. **What does `find()` do?**  
   It returns the representative root of a vertex's component.

5. **What does `unionComponents()` do?**  
   It merges two different components and updates their size and total weight.

6. **What is path compression?**  
   It makes visited vertices point closer to the root, speeding up future `find()` operations.

7. **What is union by size?**  
   It attaches the smaller component's root below the larger component's root.

8. **Why store component size?**  
   To check whether a component satisfies `size <= K`.

9. **Why store component weight?**  
   To obtain the total vertex weight without adding all vertex weights again after every merge.

10. **Why scan after each edge?**  
    The maximum eligible component may change after every insertion.

11. **What is the time complexity?**  
    `O(n + m α(n) + mn)`, dominated by scanning all vertices after each edge.

12. **What is the space complexity?**  
    `O(n)`, for the DSU arrays.

13. **What happens if an edge joins vertices already in the same component?**  
    No merge occurs because both vertices have the same root.

14. **What if every component is larger than `K`?**  
    The program prints `No eligible component`.

15. **Can this implementation handle edge deletions?**  
    Not directly. Standard DSU is designed for merging components, not arbitrary deletions.

### C-code-specific viva questions

16. **Why use `long long` for weights?**  
    The sum of many vertex weights may exceed the range of a regular `int`.

17. **Why does `getMaximumWeight()` receive a pointer?**  
    It returns a status value and uses the pointer to pass the maximum weight back to `main()`.

18. **Why check `find(i) == i`?**  
    This identifies component roots and avoids considering the same component multiple times.

19. **Why is `found` initialized to zero?**  
    It indicates that no eligible component has been found yet and allows the first valid weight to be negative.

20. **Why initialize `parent[i] = i`?**  
    Initially, each vertex forms its own component and is its own root.

## 15. Teacher-Explanation Version

“My program finds the maximum total vertex weight among connected components whose size does not exceed `K`, after each edge insertion.

I used Disjoint Set Union, or DSU, because it is useful for maintaining connected components when edges are added. Initially, every vertex is a separate component. I store the parent, size, and total weight of each component.

Whenever an edge is added, I find the roots of its two endpoints. If the roots are different, I merge the components and update their size and weight. I use path compression and union by size to make these operations efficient.

After every insertion, I scan the vertices and consider only component roots whose size is at most `K`. I print the largest total weight among them. If no component satisfies the size constraint, I print that no eligible component exists.

The DSU operations take almost constant amortized time, but scanning all vertices after every edge takes `O(n)` per insertion. Therefore, the total time complexity is `O(n + m α(n) + mn)`, and the space complexity is `O(n)`.”

## 16. Conclusion

This program combines DSU with a simple scan to solve the maximum-weight connected-component problem under a size constraint. It is easy to implement and explain, while also demonstrating a useful graph algorithm and its complexity trade-off.
