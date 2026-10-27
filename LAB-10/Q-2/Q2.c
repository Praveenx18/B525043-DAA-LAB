
#include <stdio.h>

#define MAX 100

int parent[MAX * MAX];
int rankValue[MAX * MAX];
int land[MAX * MAX];

/* Find the representative of a set using path compression. */
int findParent(int x) {
    if (parent[x] != x) {
        parent[x] = findParent(parent[x]);
    }
    return parent[x];
}

/* Join two sets. Return 1 if merged, otherwise 0. */
int unionSets(int a, int b) {
    int rootA = findParent(a);
    int rootB = findParent(b);

    if (rootA == rootB) {
        return 0;
    }

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

    if (rows <= 0 || cols <= 0 ||
        rows * cols > MAX * MAX ||
        operations < 0 || operations > rows * cols) {
        printf("Invalid input. Maximum grid size is %d x %d.\n",
               MAX, MAX);
        return 1;
    }

    /* Initially, all cells are water. */
    for (int i = 0; i < rows * cols; i++) {
        parent[i] = i;
        rankValue[i] = 0;
        land[i] = 0;
    }

    printf("Enter each land position as row column (0-based):\n");

    for (int i = 0; i < operations; i++) {
        scanf("%d %d", &r, &c);

        if (r < 0 || r >= rows || c < 0 || c >= cols) {
            printf("Invalid coordinate.\n");
            return 1;
        }

        id = r * cols + c;

        /* Ignore a cell that is already land. */
        if (land[id] == 1) {
            printf("Islands after operation %d: %d\n",
                   i + 1, islands);
            continue;
        }

        land[id] = 1;
        islands++;

        /* Check up, down, left and right. */
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];

            if (nr >= 0 && nr < rows &&
                nc >= 0 && nc < cols) {

                int neighborId = nr * cols + nc;

                if (land[neighborId] == 1) {
                    if (unionSets(id, neighborId) == 1) {
                        islands--;
                    }
                }
            }
        }

        printf("Islands after operation %d: %d\n",
               i + 1, islands);
    }

    return 0;
}
