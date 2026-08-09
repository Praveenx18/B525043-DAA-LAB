#include <stdio.h>
#include <stdlib.h>
#include <time.h>
long long operations = 0;
void merge(int a[], int n1, int b[], int n2, int result[])
{
    int i = 0, j = 0, k = 0;
    while (i < n1 && j < n2)
    {
        operations++;
        if (a[i] <= b[j])
            result[k++] = a[i++];
        else
            result[k++] = b[j++];
    }
    while (i < n1)
        result[k++] = a[i++];
    while (j < n2)
        result[k++] = b[j++];
}
void method1(int **arrays, int k, int n, int result[])
{
    int current_size = n;
    int *current = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        current[i] = arrays[0][i];
    for (int i = 1; i < k; i++)
    {
        int new_size = current_size + n;
        int *temp = malloc(new_size * sizeof(int));
        merge(current, current_size,
              arrays[i], n,
              temp);
        free(current);
        current = temp;
        current_size = new_size;
    }
    for (int i = 0; i < current_size; i++)
        result[i] = current[i];
    free(current);
}
void method2(int **arrays, int k, int n, int result[])
{
    int count = k;
    int size = n;
    int **current = arrays;
    while (count > 1)
    {
        int new_count = count / 2;
        int **next = malloc(new_count * sizeof(int *));
        for (int i = 0; i < new_count; i++)
        {
            int new_size = size * 2;
            next[i] = malloc(new_size * sizeof(int));
            merge(current[2 * i], size,
                  current[2 * i + 1], size,
                  next[i]);
        }
        if (current != arrays)
        {
            for (int i = 0; i < count; i++)
                free(current[i]);
            free(current);
        }
        current = next;
        count = new_count;
        size *= 2;
    }
    for (int i = 0; i < size; i++)
        result[i] = current[0][i];
    free(current[0]);
    free(current);
}
int main()
{
    int n = 1000;
    printf("\nMerging k Sorted Arrays\n\n");
    printf("%-8s %-22s %-18s %-22s %-18s\n",
           "k",
           "Method 1 Operations",
           "Method 1 Time",
           "Method 2 Operations",
           "Method 2 Time");

    printf("--------------------------------------------------------------------------------\n");
    for (int k = 2; k <= 64; k *= 2)
    {
        int **arrays = malloc(k * sizeof(int *));
        for (int i = 0; i < k; i++)
        {
            arrays[i] = malloc(n * sizeof(int));

            for (int j = 0; j < n; j++)
                arrays[i][j] = j * k + i;
        }
        int *result1 = malloc(k * n * sizeof(int));
        int *result2 = malloc(k * n * sizeof(int));
        clock_t start, end;
        long long method1_operations;
        long long method2_operations;
        double method1_time;
        double method2_time;
        operations = 0;
        start = clock();
        method1(arrays, k, n, result1);
        end = clock();
        method1_operations = operations;
        method1_time =
            (double)(end - start) / CLOCKS_PER_SEC;
        operations = 0;
        start = clock();
        method2(arrays, k, n, result2);
        end = clock();
        method2_operations = operations;
        method2_time =
            (double)(end - start) / CLOCKS_PER_SEC;
        printf("%-8d %-22lld %-18.6f %-22lld %-18.6f\n",
               k,
               method1_operations,
               method1_time,
               method2_operations,
               method2_time);
        free(result1);
        free(result2);
        for (int i = 0; i < k; i++)
            free(arrays[i]);
        free(arrays);
    }
    return 0;
}