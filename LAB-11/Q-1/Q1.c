
#include <stdio.h>

#define MAXV 1005
#define MAXE 10005
#define MAXQ (4 * MAXE + 2 * MAXV + 10)
#define INF 4000000000000000000LL

int head[MAXV], to[2 * MAXE];
int edgeWeight[2 * MAXE], nextEdge[2 * MAXE];
int edgeCount = 0;

int deque[MAXQ];
int front = MAXQ / 2, back = MAXQ / 2;
int dequeSize = 0;

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

    printf("Enter each directed edge as: from to weight (0 or 1)\n");

    for (i = 0; i < m; i++) {
        if (scanf("%d %d %d", &u, &v, &w) != 3)
            return 0;

        if (u < 1 || u > n || v < 1 || v > n ||
            (w != 0 && w != 1)) {
            printf("Invalid edge input.\n");
            return 0;
        }

        addEdge(u, v, w);
    }

    zeroOneBFS(n, source);

    printf("\nMinimum effective cost from source %d:\n", source);

    for (i = 1; i <= n; i++) {
        evenCost = dist[i][0];

        oddCost = (dist[i][1] == INF)
                    ? INF : dist[i][1] + penalty;

        if (evenCost == INF && oddCost == INF) {
            printf("Vertex %d: Unreachable\n", i);
        } else {
            answer = (evenCost < oddCost) ? evenCost : oddCost;
            printf("Vertex %d: %lld\n", i, answer);
        }
    }

    return 0;
}
