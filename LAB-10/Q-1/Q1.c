
#include <stdio.h>

#define MAX 100

int graph[MAX][MAX];

int isBipartite(int vertices) {
    int color[MAX];
    int queue[MAX];
    int front, rear;
    int i, u, v;

    for (i = 0; i < vertices; i++) {
        color[i] = -1;
    }

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
    int u, v, i, j;

    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++) {
            graph[i][j] = 0;
        }
    }

    printf("Enter number of vertices and edges to process: ");

    if (scanf("%d %d", &vertices, &edges) != 2 ||
        vertices < 1 || vertices > MAX || edges < 0) {
        printf("Invalid input.\n");
        return 1;
    }

    printf("Enter each edge as two vertex numbers (0 to %d):\n",
           vertices - 1);

    for (i = 0; i < edges; i++) {
        printf("Edge %d: ", i + 1);

        if (scanf("%d %d", &u, &v) != 2 ||
            u < 0 || u >= vertices || v < 0 || v >= vertices) {
            printf("Invalid edge.\n");
            return 1;
        }

        if (graph[u][v] == 1) {
            printf("Edge (%d, %d) already exists. Ignored.\n", u, v);
            continue;
        }

        graph[u][v] = 1;
        graph[v][u] = 1;

        if (isBipartite(vertices)) {
            printf("Edge (%d, %d): Accepted. Graph remains bipartite.\n",
                   u, v);
        } else {
            graph[u][v] = 0;
            graph[v][u] = 0;

            printf("Edge (%d, %d): Rejected. It violates bipartiteness.\n",
                   u, v);
        }
    }

    printf("\nFinal accepted edges (adjacency matrix):\n");

    for (i = 0; i < vertices; i++) {
        for (j = 0; j < vertices; j++) {
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }

    return 0;
}
3 3
0 1
1 2
2 0