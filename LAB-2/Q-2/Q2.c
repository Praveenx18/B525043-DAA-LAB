#include <stdio.h>
#include <stdlib.h>
#include <time.h>
long long operations = 0;
void merge(int a[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));
    for (int i = 0; i < n1; i++)
        L[i] = a[left + i];
    for (int i = 0; i < n2; i++)
        R[i] = a[mid + 1 + i];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        operations++;
        if (L[i] <= R[j])
            a[k++] = L[i++];
        else
            a[k++] = R[j++];
    }
    while (i < n1)
        a[k++] = L[i++];
    while (j < n2)
        a[k++] = R[j++];
    free(L);
    free(R);
}
void mergeSort(int a[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(a, left, mid);
        mergeSort(a, mid + 1, right);
        merge(a, left, mid, right);
    }
}
void threeWayMerge(int a[], int left, int mid1, int mid2, int right)
{
    int n1 = mid1 - left;
    int n2 = mid2 - mid1;
    int n3 = right - mid2;
    int *A = malloc(n1 * sizeof(int));
    int *B = malloc(n2 * sizeof(int));
    int *C = malloc(n3 * sizeof(int));
    for (int i = 0; i < n1; i++)
        A[i] = a[left + i];
    for (int i = 0; i < n2; i++)
        B[i] = a[mid1 + i];
    for (int i = 0; i < n3; i++)
        C[i] = a[mid2 + i];
    int i = 0, j = 0, k = 0;
    int p = left;
    while (i < n1 || j < n2 || k < n3)
    {
        if (i < n1 && j < n2 && k < n3)
        {
            operations += 2;
            if (A[i] <= B[j] && A[i] <= C[k])
                a[p++] = A[i++];
            else if (B[j] <= A[i] && B[j] <= C[k])
                a[p++] = B[j++];
            else
                a[p++] = C[k++];
        }
        else if (i < n1 && j < n2)
        {
            operations++;
            if (A[i] <= B[j])
                a[p++] = A[i++];
            else
                a[p++] = B[j++];
        }
        else if (i < n1 && k < n3)
        {
            operations++;
            if (A[i] <= C[k])
                a[p++] = A[i++];
            else
                a[p++] = C[k++];
        }
        else if (j < n2 && k < n3)
        {
            operations++;
            if (B[j] <= C[k])
                a[p++] = B[j++];
            else
                a[p++] = C[k++];
        }
        else if (i < n1)
        {
            a[p++] = A[i++];
        }
        else if (j < n2)
        {
            a[p++] = B[j++];
        }
        else
        {
            a[p++] = C[k++];
        }
    }
    free(A);
    free(B);
    free(C);
}
void threeWayMergeSort(int a[], int left, int right)
{
    if (right - left <= 1)
        return;
    int third = (right - left) / 3;
    int mid1 = left + third;
    int mid2 = left + 2 * third;
    if (third == 0)
        mid1 = left + 1;
    if (mid2 <= mid1)
        mid2 = mid1 + 1;
    if (mid2 > right)
        mid2 = right;
    threeWayMergeSort(a, left, mid1);
    threeWayMergeSort(a, mid1, mid2);
    threeWayMergeSort(a, mid2, right);
    threeWayMerge(a, left, mid1, mid2, right);
}
int main()
{
    FILE *csv = fopen("merge_sort_comparison.csv", "w");
    if (csv == NULL)
    {
        printf("Error creating CSV file.\n");
        return 1;
    }
    fprintf(csv, "n,Normal Merge Sort,Three-Way Merge Sort\n");
    srand(time(NULL));
    for (int n = 1000; n <= 10000; n += 1000)
    {
        int *a = malloc(n * sizeof(int));
        int *b = malloc(n * sizeof(int));
        if (a == NULL || b == NULL)
        {
            printf("Memory allocation failed.\n");
            fclose(csv);
            return 1;
        }
        for (int i = 0; i < n; i++)
        {
            a[i] = rand();
            b[i] = a[i];
        }
        operations = 0;
        mergeSort(a, 0, n - 1);
        long long normal_operations = operations;
        operations = 0;
        threeWayMergeSort(b, 0, n);
        long long three_way_operations = operations;
        fprintf(csv, "%d,%lld,%lld\n",
                n,
                normal_operations,
                three_way_operations);
        printf("n = %d | Normal = %lld | Three-Way = %lld\n",
               n,
               normal_operations,
               three_way_operations);
        free(a);
        free(b);
    }
    fclose(csv);
    printf("\nCSV file created: merge_sort_comparison.csv\n");
    return 0;
}