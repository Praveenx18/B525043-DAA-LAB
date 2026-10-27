
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
