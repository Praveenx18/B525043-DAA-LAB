# Dynamic Connected Components in a Grid (Number of Islands II)

## Problem statement
An `R x C` grid initially contains only water. Land cells are added one at a time. After every addition, report the number of 4-directionally connected islands. Two land cells are connected when one can be reached from the other by moving up, down, left, or right through land cells.

## Objective
Maintain the number of islands efficiently after each land addition.

## Approach: Disjoint Set Union (DSU)
DSU, also called Union-Find, maintains groups of connected cells. Each cell is converted to a one-dimensional ID using `id = row * columns + column`.

- `land[id]` says whether that cell has become land.
- `parent[id]` stores the parent of the cell in the DSU forest.
- `rankValue[id]` helps keep trees shallow.
- `findParent()` uses path compression.
- `unionSets()` joins two different components and reports whether a merge happened.

For every new land cell, increase the island count by one. Check its four neighbors. For each neighbor that is already land, union the two cells. If the union actually joins two different sets, decrease the island count by one. If the same coordinate is added again, the count does not change.

DSU is suitable because it supports repeated connectivity checks and merges without rebuilding the entire grid's connected components after every operation.

## Algorithm steps
1. Read rows, columns, and number of operations.
2. Initialize each cell as its own DSU set and mark every cell as water.
3. For each coordinate:
   1. Convert it to a cell ID.
   2. If it is already land, print the current island count and continue.
   3. Mark it as land and increase the island count.
   4. Check up, down, left, and right neighbors.
   5. For each in-bounds land neighbor, union the two sets. Decrease the count only if two different sets were merged.
   6. Print the island count.

## C program
```c
#include <stdio.h>

#define MAX 100

int parent[MAX * MAX];
int rankValue[MAX * MAX];
int land[MAX * MAX];

int findParent(int x) {
    if (parent[x] != x) {
        parent[x] = findParent(parent[x]);
    }
    return parent[x];
}

int unionSets(int a, int b) {
    int rootA = findParent(a);
    int rootB = findParent(b);

    if (rootA == rootB) return 0;

    if (rankValue[rootA] < rankValue[rootB]) {
        parent[rootA] = rootB;
    } else if (rankValue[rootA] > rankValue[rootB]) {
        parent[rootB] = rootA;
    } else {
        parent[rootB] = rootA;
        rankValue[rootA]++;
    }
    return 1;
}

int main(void) {
    int rows, cols, operations;
    int r, c, id, islands = 0;
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    printf("Enter rows, columns and number of operations: ");
    scanf("%d %d %d", &rows, &cols, &operations);

    if (rows <= 0 || cols <= 0 || rows * cols > MAX * MAX ||
        operations < 0 || operations > rows * cols) {
        printf("Invalid input. Maximum grid size is %d x %d.\\n", MAX, MAX);
        return 1;
    }

    for (int i = 0; i < rows * cols; i++) {
        parent[i] = i;
        rankValue[i] = 0;
        land[i] = 0;
    }

    printf("Enter each land position as row column (0-based):\\n");
    for (int i = 0; i < operations; i++) {
        scanf("%d %d", &r, &c);
        if (r < 0 || r >= rows || c < 0 || c >= cols) {
            printf("Invalid coordinate.\\n");
            return 1;
        }

        id = r * cols + c;
        if (land[id] == 1) {
            printf("Islands after operation %d: %d\\n", i + 1, islands);
            continue;
        }

        land[id] = 1;
        islands++;

        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                int neighborId = nr * cols + nc;
                if (land[neighborId] == 1 &&
                    unionSets(id, neighborId) == 1) {
                    islands--;
                }
            }
        }
        printf("Islands after operation %d: %d\\n", i + 1, islands);
    }
    return 0;
}
```

## Sample input
```text
3 3 5
0 0
0 1
1 2
2 1
1 1
```

## Sample output
```text
Enter rows, columns and number of operations: Enter each land position as row column (0-based):
Islands after operation 1: 1
Islands after operation 2: 1
Islands after operation 3: 2
Islands after operation 4: 3
Islands after operation 5: 1
```

## Dry run
Grid size is `3 x 3`. Coordinates are zero-based.

| Operation | Added cell | What happens | Island count |
|---:|---|---|---:|
| 1 | `(0,0)` | No land neighbors; create one island | 1 |
| 2 | `(0,1)` | Adjacent to `(0,0)`; merge the two cells | 1 |
| 3 | `(1,2)` | No adjacent land cell | 2 |
| 4 | `(2,1)` | No adjacent land cell | 3 |
| 5 | `(1,1)` | Joins the components containing `(0,1)`, `(1,2)`, and `(2,1)` | 1 |

## Function explanations
- `findParent(x)`: returns the representative (root) of the set containing `x`. Path compression makes future searches faster.
- `unionSets(a, b)`: merges the sets containing `a` and `b`. Returns `1` if a merge happened, otherwise `0`. Union by rank keeps the DSU tree shallow.
- `main()`: reads input, initializes arrays, processes each cell addition, checks the four neighbors, updates the count, and prints the result.

## Complexity analysis
Let `N = R * C` be the number of grid cells and `K` the number of additions.

- Initialization: `O(N)`.
- Each addition checks exactly four neighbors and performs at most four DSU unions.
- With path compression and union by rank, each DSU operation takes amortized `O(α(N))`, where `α` is the inverse Ackermann function and grows extremely slowly.
- Total time: `O(N + K α(N))` (often described as nearly `O(N + K)`).
- Space: `O(N)` for the parent, rank, and land arrays.
- Best/average/worst: the asymptotic bound is the same amortized bound for this DSU implementation; individual operations may differ.

The included graph compares common theoretical growth curves (`O(n)`, `O(n log n)`, and `O(n²)`) to help visualize growth. These are illustrative curves, not measured timings. The algorithm's more precise bound is near-linear, `O(RC + K α(RC))`.

## Advantages
- Efficiently updates connectivity after each insertion.
- Checks only four neighbors for each new cell.
- Avoids recalculating all connected components after every update.
- Uses simple arrays and works well for a fixed grid size.

## Limitations
- This implementation supports at most a `100 x 100` grid; increase `MAX` carefully if needed.
- Coordinates are zero-based.
- It supports adding land, not removing land.
- It assumes each operation adds one cell; duplicate additions are handled as no-ops.

## Compile and run
Save the code as `dynamic_islands.c`, then run:
```bash
gcc dynamic_islands.c -o dynamic_islands
./dynamic_islands
```
On Windows with GCC, run `dynamic_islands.exe` after compiling.

## Viva questions and answers
1. **What is an island?** A group of land cells connected by up, down, left, or right moves.
2. **What is DSU?** A data structure that maintains disjoint sets and supports finding and merging sets.
3. **Why use DSU here?** Because each operation only adds land, so components merge but never split.
4. **What does `parent[]` store?** The parent pointer for each DSU element.
5. **What is a representative?** The root that identifies a set.
6. **What is path compression?** It makes nodes point closer to the root during `findParent()`.
7. **What is union by rank?** It attaches the shallower tree under the deeper tree.
8. **Why is `islands` increased for a new cell?** A new land cell initially forms a new component.
9. **Why is `islands` decreased after a successful union?** Two previously separate components become one.
10. **Why check only four neighbors?** The problem defines 4-directional connectivity.
11. **What if the same cell is added twice?** The code prints the unchanged count.
12. **What is the time complexity?** `O(RC + K α(RC))`, where `K` is the number of additions.
13. **What is the space complexity?** `O(RC)`.
14. **Why use `id = r * cols + c`?** It maps a 2D cell to a single array index.
15. **What do `dr[]` and `dc[]` do?** They store row and column offsets for the four directions.
16. **Why does `unionSets()` return an integer?** The caller needs to know whether two components were actually merged.
17. **What does `continue` do in the duplicate-cell case?** It skips the rest of that iteration and moves to the next operation.

## Teacher explanation (1–2 minutes)
“My program finds the number of islands after every land cell is added to a grid. I use Disjoint Set Union, also called Union-Find, because it efficiently maintains connected groups. At the beginning, every cell is water and the island count is zero. When a new cell becomes land, I increase the count by one. Then I check its four neighbors. If a neighbor is already land, I use `unionSets()` to combine their components. If they were separate components, the island count decreases by one. `findParent()` finds the component representative, using path compression, and union by rank keeps the trees shallow. I convert row and column into one array index using `row * columns + column`. After processing each position, I print the current island count. The time complexity is `O(RC + K α(RC))`, and the space complexity is `O(RC)`. This is more efficient than recalculating all islands from scratch after every addition.”
