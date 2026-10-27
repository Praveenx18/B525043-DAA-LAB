# Lexicographically Smallest Connected Component Permutation

## Problem Statement

Given a permutation `A` containing the numbers `1` to `n`, and a list of allowed swaps `(u, v)`, treat each allowed swap as an undirected edge in a graph. Find the lexicographically smallest permutation obtainable by performing any number of allowed swaps.

A permutation is lexicographically smaller than another if, at the first position where they differ, it contains the smaller number.

## Objective

Use graph connected components to determine which positions/values can be rearranged among themselves, then produce the smallest possible permutation.

## Approach: Connected Components + Sorting

Every allowed swap is an edge. If two vertices belong to the same connected component, a sequence of allowed swaps along paths lets values move within that component. Values cannot move between different components.

For each connected component:
1. Collect the vertex labels in the component.
2. Collect the values currently present at those positions.
3. Sort both lists in ascending order.
4. Put the smallest value at the smallest position, the next-smallest at the next position, and so on.

This gives the lexicographically smallest result because each component's values can be rearranged internally but not moved outside it.

The implementation uses a simple adjacency matrix and depth-first search (DFS), suitable for a beginner-level lab program. An adjacency list would use less memory for sparse graphs, but is slightly more involved in C.

## Step-by-Step Algorithm

1. Read `n` and the permutation.
2. Read `m`, the number of allowed swaps, followed by the `m` pairs.
3. Create an undirected graph: for each pair `(u, v)`, add edges both ways.
4. Run DFS from every unvisited vertex to identify its connected component.
5. For each component, sort its vertex labels and the values at those positions.
6. Assign the sorted values to the sorted positions.
7. Print the resulting permutation.

## C Program

```c
#include <stdio.h>

#define MAX 100

int graph[MAX + 1][MAX + 1];
int visited[MAX + 1];
int component[MAX + 1];
int size;

void dfs(int vertex, int n)
{
    int i;

    visited[vertex] = 1;
    component[size] = vertex;
    size++;

    for (i = 1; i <= n; i++)
    {
        if (graph[vertex][i] == 1 && visited[i] == 0)
        {
            dfs(i, n);
        }
    }
}

void sort(int arr[], int n)
{
    int i, j, temp;

    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main(void)
{
    int n, m, i, u, v;
    int A[MAX + 1];
    int values[MAX + 1];
    int vertices[MAX + 1];

    printf("Enter n: ");
    scanf("%d", &n);

    printf("Enter the permutation: ");
    for (i = 1; i <= n; i++)
    {
        scanf("%d", &A[i]);
    }

    printf("Enter number of allowed swaps: ");
    scanf("%d", &m);

    printf("Enter each allowed swap (u v):\n");
    for (i = 0; i < m; i++)
    {
        scanf("%d %d", &u, &v);
        graph[u][v] = 1;
        graph[v][u] = 1;
    }

    for (i = 1; i <= n; i++)
    {
        if (visited[i] == 0)
        {
            int j;

            size = 0;
            dfs(i, n);

            for (j = 0; j < size; j++)
            {
                vertices[j] = component[j];
                values[j] = A[component[j]];
            }

            sort(vertices, size);
            sort(values, size);

            for (j = 0; j < size; j++)
            {
                A[vertices[j]] = values[j];
            }
        }
    }

    printf("Lexicographically smallest permutation: ");
    for (i = 1; i <= n; i++)
    {
        printf("%d ", A[i]);
    }
    printf("\n");

    return 0;
}
```

## Important Functions and Variables

- `graph[u][v]`: adjacency matrix; `1` means an allowed swap connects the two vertices.
- `visited[i]`: records whether vertex `i` has already been visited by DFS.
- `component[]`: stores the vertices found in the current connected component.
- `size`: number of vertices currently stored in `component`.
- `A[]`: input permutation and, eventually, the answer.
- `dfs(vertex, n)`: visits all vertices reachable from `vertex`, collecting one connected component.
- `sort(arr, n)`: bubble-sorts an array in ascending order.
- `vertices[]`: sorted positions in the current component.
- `values[]`: sorted values currently found at those positions.

### Why sort both arrays?

The values can only be rearranged within their component. Assigning the smallest value to the smallest index gives the smallest possible value at the earliest position in that component; repeating this produces the lexicographically smallest arrangement.

## Sample Input

```text
5
5 4 3 2 1
2
1 2
3 4
```

## Sample Output

```text
Lexicographically smallest permutation: 4 5 2 3 1
```

The program also prints prompts before reading input, so the exact terminal display includes those prompts.

## Dry Run

Input permutation: `[5, 4, 3, 2, 1]`

Allowed swaps: `(1, 2)` and `(3, 4)`

The graph has three connected components:

| Component | Positions | Current values | Sorted positions | Sorted values | Result at these positions |
|---|---|---|---|---|---|
| 1 | `{1, 2}` | `{5, 4}` | `{1, 2}` | `{4, 5}` | `A[1]=4, A[2]=5` |
| 2 | `{3, 4}` | `{3, 2}` | `{3, 4}` | `{2, 3}` | `A[3]=2, A[4]=3` |
| 3 | `{5}` | `{1}` | `{5}` | `{1}` | `A[5]=1` |

Final permutation: **`[4, 5, 2, 3, 1]`**

The singleton component `{5}` cannot exchange its value with any other position, so its value remains `1`.

## Complexity Analysis

Let `n` be the number of permutation elements and `m` the number of allowed swaps.

### Time complexity

- Building the adjacency matrix from the swap list: `O(m)`.
- Finding components with this adjacency matrix: `O(n²)` in total, because DFS scans up to `n` possible neighbors for each vertex.
- Sorting the component vertices and values: with bubble sort, a component of size `k` costs `O(k²)`. Across components, the total is at most `O(n²)`.
- Overall worst-case time: **`O(n² + m)`**, which is `O(n²)` when `m` is at most on the order of `n²`.

Best case: **`O(n² + m)` for this implementation as well**, because the adjacency-matrix DFS scans all possible neighbors even if the graph has no edges. The amount of sorting work is smaller for many tiny components, but the matrix scans remain. Average case is also bounded by **`O(n² + m)`**; actual sorting work depends on component sizes.

### Space complexity

- Adjacency matrix: `O(n²)`.
- Arrays for the permutation, visited flags, components, and temporary values/vertices: `O(n)`.
- DFS recursion stack: up to `O(n)`.

Overall auxiliary space: **`O(n²)`**, dominated by the adjacency matrix.

### Complexity graph

The included `complexity_graph.png` plots reference growth curves `O(n)`, `O(n log n)`, and `O(n²)` against input size. They are illustrative mathematical growth curves, not measured runtime data. The algorithm's worst-case growth is `O(n² + m)` and is quadratic in `n` for a dense-graph-sized input.

## Advantages

- Uses a clear graph concept: connected components.
- Correctly respects the allowed-swap restrictions.
- DFS and bubble sort are straightforward to explain in a lab viva.
- Works for disconnected graphs and isolated vertices.

## Disadvantages and Limitations

- The adjacency matrix requires `O(n²)` memory, which is inefficient for large sparse graphs.
- Bubble sort can take `O(k²)` time for a component of size `k`.
- `MAX` is set to `100`; increase it carefully if larger inputs are required.
- The input is assumed to be a valid permutation of `1` through `n`, and swap endpoints must be between `1` and `n`.
- Recursive DFS may use a deep call stack for very large graphs.

## Compile and Run

Save the code as `main.c`, then compile with GCC:

```bash
gcc main.c -o main
```

Run it:

```bash
./main
```

On Windows with GCC:

```bash
gcc main.c -o main.exe
main.exe
```

## Viva Preparation

1. **What is the problem asking?**  
   Find the lexicographically smallest permutation possible using only the listed swaps.

2. **What does lexicographically smallest mean?**  
   Compare two arrays from left to right; at the first differing position, the array with the smaller value is smaller.

3. **Why do we use a graph?**  
   Each allowed swap is an edge, so the graph represents which positions can exchange values through sequences of swaps.

4. **What is a connected component?**  
   A maximal set of vertices where every vertex is reachable from every other vertex.

5. **Why can values be rearranged inside a component?**  
   Paths of allowed swaps allow values to move through connected vertices, permitting any rearrangement within that component.

6. **Why can values not move between components?**  
   There is no edge path connecting separate components, so allowed swaps cannot transfer a value between them.

7. **Why do we sort the positions and values?**  
   To put the smallest available value at the smallest index in each component, then the next-smallest value at the next index.

8. **Which graph traversal is used?**  
   Depth-first search (DFS).

9. **What is the time complexity?**  
   `O(n² + m)` for this adjacency-matrix and bubble-sort implementation.

10. **What is the space complexity?**  
    `O(n²)`, mainly because the adjacency matrix contains `n × n` entries.

11. **What if there are no allowed swaps?**  
    Every vertex is its own component, so the permutation stays unchanged.

12. **What if all vertices are connected?**  
    All values can be rearranged across all positions, so sorting the entire permutation gives the answer.

13. **What is one limitation of an adjacency matrix?**  
    It uses quadratic memory even when only a few edges exist.

14. **How could the program be improved?**  
    Use an adjacency list and a more efficient sorting method such as `qsort`, or use a disjoint-set union structure.

15. **Why is DFS called for every unvisited vertex?**  
    The graph may be disconnected; starting DFS from every unvisited vertex ensures all components are found.

### C-code-specific viva questions

1. **Why is `graph[u][v]` and `graph[v][u]` both set to `1`?**  
   The graph is undirected, so the edge works in both directions.

2. **Why does `visited[vertex] = 1` appear in DFS?**  
   It prevents revisiting the same vertex and avoids repeated work or infinite recursion.

3. **What does `size` represent?**  
   It is the number of vertices collected in the current component.

4. **Why are `vertices[]` and `values[]` separate arrays?**  
   We sort the positions independently from the values, then assign sorted values to sorted positions.

5. **Why does the outer loop run from `1` to `n`?**  
   The permutation labels are `1` through `n`; the loop makes sure every vertex is included in some component.

## Short Teacher Explanation (1–2 minutes)

“My program finds the lexicographically smallest permutation using the allowed swaps. I represent each allowed swap as an undirected edge in a graph. First, I read the permutation and all the swap pairs and build an adjacency matrix. Then I use DFS to find the connected components.

The important observation is that values can be rearranged within one connected component, but they cannot move between different components. So, for each component, I collect its vertex positions and the values currently stored at those positions. I sort both arrays in ascending order and put the sorted values into the sorted positions. I repeat this for every component and print the final permutation.

The graph traversal takes `O(n²)` time with an adjacency matrix, and the bubble sorts take at most `O(n²)` time overall. Therefore, the total time complexity is `O(n² + m)`, and the space complexity is `O(n²)` because of the adjacency matrix.”
