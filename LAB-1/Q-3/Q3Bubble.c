#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Version (i): optimized bubble sort with early termination
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
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
    free(a);
    return comparisons;
}

// Version (ii): naive bubble sort, always completes n-1 passes
long bubbleSortNaive(int arr[], int n) {
    int *a = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) a[i] = arr[i];
    long comparisons = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparisons++;
            if (a[j] > a[j + 1]) {
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }
    free(a);
    return comparisons;
}

int main(void) {
    srand(42); // fixed seed for reproducibility

    FILE *fp = fopen("results.csv", "w");
    if (!fp) {
        printf("Error opening file\n");
        return 1;
    }
    fprintf(fp, "n,optimized_comparisons,naive_comparisons\n");

    int sizes[] = {10, 50, 100, 200, 300, 400, 500, 700, 900, 1100,
                   1300, 1500, 1700, 2000, 2300, 2600, 3000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    int trials = 5; // average over multiple random arrays per size

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        long totalOpt = 0, totalNaive = 0;
        for (int t = 0; t < trials; t++) {
            int *arr = malloc(n * sizeof(int));
            for (int i = 0; i < n; i++) arr[i] = rand() % 100000;

            totalOpt += bubbleSortOptimized(arr, n);
            totalNaive += bubbleSortNaive(arr, n);

            free(arr);
        }
        long avgOpt = totalOpt / trials;
        long avgNaive = totalNaive / trials;
        fprintf(fp, "%d,%ld,%ld\n", n, avgOpt, avgNaive);
        printf("n=%5d  optimized_avg=%10ld  naive_avg=%10ld\n", n, avgOpt, avgNaive);
    }

    fclose(fp);
    printf("\nResults written to results.csv\n");
    return 0;
}