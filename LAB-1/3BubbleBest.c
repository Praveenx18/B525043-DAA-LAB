#include <stdio.h>
#include <stdlib.h>

long bubbleSortOptimized(int arr[], int n) {
    int *a = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) a[i] = arr[i];
    long comparisons = 0;
    int swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = 0;
        for (int j = 0; j < n - i - 1; j++) {
            comparisons++;
            if (a[j] > a[j + 1]) {
                int tmp = a[j]; a[j] = a[j + 1]; a[j + 1] = tmp;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
    free(a);
    return comparisons;
}

long bubbleSortNaive(int arr[], int n) {
    int *a = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) a[i] = arr[i];
    long comparisons = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparisons++;
            if (a[j] > a[j + 1]) {
                int tmp = a[j]; a[j] = a[j + 1]; a[j + 1] = tmp;
            }
        }
    }
    free(a);
    return comparisons;
}

int main(void) {
    FILE *fp = fopen("results_bestcase.csv", "w");
    fprintf(fp, "n,optimized_comparisons,naive_comparisons\n");

    int sizes[] = {10, 50, 100, 200, 300, 400, 500, 700, 900, 1100,
                   1300, 1500, 1700, 2000, 2300, 2600, 3000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        int *arr = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) arr[i] = i; // already sorted -> best case

        long opt = bubbleSortOptimized(arr, n);
        long naive = bubbleSortNaive(arr, n);
        fprintf(fp, "%d,%ld,%ld\n", n, opt, naive);
        free(arr);
    }
    fclose(fp);
    printf("Best-case results written to results_bestcase.csv\n");
    return 0;
}