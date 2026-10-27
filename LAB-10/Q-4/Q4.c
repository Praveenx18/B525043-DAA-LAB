
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

    printf("Enter number of vertices and edges: ");
    if (scanf("%d %d", &n, &m) != 2)
        return 1;

    if (n < 1 || n >= MAXV || m < 0 || m >= MAXE) {
        printf("Invalid graph size.\n");
        return 1;
    }

    printf("Enter vertex weights: ");
    for (int i = 1; i <= n; i++) {
        if (scanf("%d", &weight[i]) != 1 || weight[i] < 0) {
            printf("Invalid vertex weight.\n");
            return 1;
        }
    }

    for (int i = 1; i <= n; i++)
        head[i] = -1;

    printf("Enter edges (u v):\n");
    for (int i = 0; i < m; i++) {
        int u, v;

        if (scanf("%d %d", &u, &v) != 2)
            return 1;

        if (u < 1 || u > n || v < 1 || v > n) {
            printf("Invalid edge.\n");
            return 1;
        }

        to[edgeCount] = v;
        nextEdge[edgeCount] = head[u];
        head[u] = edgeCount++;

        to[edgeCount] = u;
        nextEdge[edgeCount] = head[v];
        head[v] = edgeCount++;
    }

    printf("Enter deletion order: ");
    int seen[MAXV] = {0};

    for (int i = 1; i <= n; i++) {
        if (scanf("%d", &order[i]) != 1)
            return 1;

        if (order[i] < 1 || order[i] > n || seen[order[i]]) {
            printf("Invalid deletion order.\n");
            return 1;
        }

        seen[order[i]] = 1;
    }

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

        for (int edge = head[vertex]; edge != -1;
             edge = nextEdge[edge]) {
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

    printf("Maximum component sums after each deletion:\n");

    for (int i = 2; i <= n; i++)
        printf("%lld ", answer[i]);

    printf("0\n");

    return 0;
}

// 3 2
// 4 2 5
// 1 2
// 2 3
// 2 1 3
