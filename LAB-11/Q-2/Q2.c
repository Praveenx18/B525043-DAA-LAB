
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

    /* Insert all hubs into the queue */
    for (i = 0; i < k; i++) {
        u = hubs[i];
        dist[u] = 0;
        owner[u] = i + 1;
        queue[rear++] = u;
    }

    while (front < rear) {
        u = queue[front++];

        for (edge = head[u]; edge != -1;
             edge = nextEdge[edge]) {

            v = to[edge];

            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                owner[v] = owner[u];
                queue[rear++] = v;
            }
            else if (dist[v] == dist[u] + 1 &&
                     owner[u] < owner[v]) {
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

    if (n <= 0 || n >= MAXV ||
        m < 0 || 2 * m >= MAXE) {
        printf("Invalid graph size.\n");
        return 0;
    }

    for (i = 1; i <= n; i++)
        head[i] = -1;

    printf("Enter the edges (u v):\n");

    for (i = 0; i < m; i++) {
        if (scanf("%d %d", &u, &v) != 2)
            return 0;

        if (u < 1 || u > n || v < 1 || v > n) {
            printf("Invalid edge.\n");
            return 0;
        }

        addEdge(u, v);
    }

    printf("Enter number of hubs: ");
    if (scanf("%d", &k) != 1 ||
        k <= 0 || k > n) {
        printf("Invalid number of hubs.\n");
        return 0;
    }

    printf("Enter hub vertices in hub-index order: ");

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
        printf("Invalid radius.\n");
        return 0;
    }

    multiSourceBFS(n, k, hubs);

    printf("\nVertex\tDistance\tOwner\n");

    for (i = 1; i <= n; i++) {
        if (dist[i] == -1 || dist[i] > radius)
            owner[i] = -1;

        printf("%d\t%d\t\t%d\n",
               i, dist[i], owner[i]);
    }

    return 0;
}
