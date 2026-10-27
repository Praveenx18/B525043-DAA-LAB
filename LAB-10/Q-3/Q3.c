
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

