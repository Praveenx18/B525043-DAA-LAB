#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
static long long comparisons; /* global comparison counter, reset before each sort */
/* ---------------------------------------------------------------------
 * Standard 2-way merge sort
 * ------------------------------------------------------------------- */
static void merge2(int *arr, int lo, int mid, int hi, int *tmp) {
    int i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        comparisons++;
        if (arr[i] <= arr[j]) tmp[k++] = arr[i++];
        else                  tmp[k++] = arr[j++];
    }
    while (i < mid) tmp[k++] = arr[i++];
    while (j < hi)  tmp[k++] = arr[j++];
    memcpy(arr + lo, tmp + lo, (hi - lo) * sizeof(int));
}

static void mergeSort2(int *arr, int lo, int hi, int *tmp) {
    if (hi - lo <= 1) return;
    int mid = lo + (hi - lo) / 2;
    mergeSort2(arr, lo, mid, tmp);
    mergeSort2(arr, mid, hi, tmp);
    merge2(arr, lo, mid, hi, tmp);
}
/* ---------------------------------------------------------------------
 * Modified 3-way merge sort
 * ------------------------------------------------------------------- */
/* 3-way merge of arr[lo..m1), arr[m1..m2), arr[m2..hi) into tmp, copied back */
static void merge3(int *arr, int lo, int m1, int m2, int hi, int *tmp) {
    int i = lo, j = m1, k = m2, t = lo;
    while (i < m1 && j < m2 && k < hi) {
        /* find minimum of arr[i], arr[j], arr[k] using pairwise comparisons */
        comparisons++;
        if (arr[i] <= arr[j]) {
            comparisons++;
            if (arr[i] <= arr[k]) tmp[t++] = arr[i++];
            else                  tmp[t++] = arr[k++];
        } else {
            comparisons++;
            if (arr[j] <= arr[k]) tmp[t++] = arr[j++];
            else                  tmp[t++] = arr[k++];
        }
    }
    /* at most one of the three segments remains; finish with 2-way merges */
    while (i < m1 && j < m2) {
        comparisons++;
        if (arr[i] <= arr[j]) tmp[t++] = arr[i++];
        else                  tmp[t++] = arr[j++];
    }
    while (j < m2 && k < hi) {
        comparisons++;
        if (arr[j] <= arr[k]) tmp[t++] = arr[j++];
        else                  tmp[t++] = arr[k++];
    }
    while (i < m1 && k < hi) {
        comparisons++;
        if (arr[i] <= arr[k]) tmp[t++] = arr[i++];
        else                  tmp[t++] = arr[k++];
    }
    while (i < m1) tmp[t++] = arr[i++];
    while (j < m2) tmp[t++] = arr[j++];
    while (k < hi) tmp[t++] = arr[k++];

    memcpy(arr + lo, tmp + lo, (hi - lo) * sizeof(int));
}

static void mergeSort3(int *arr, int lo, int hi, int *tmp) {
    int n = hi - lo;
    if (n <= 1) return;
    int m1 = lo + n / 3;
    int m2 = lo + 2 * n / 3;
    mergeSort3(arr, lo, m1, tmp);
    mergeSort3(arr, m1, m2, tmp);
    mergeSort3(arr, m2, hi, tmp);
    merge3(arr, lo, m1, m2, hi, tmp);
}

/* ---------------------------------------------------------------------
 * Helpers
 * ------------------------------------------------------------------- */

static void fillRandom(int *arr, int n, unsigned seed) {
    srand(seed);
    for (int i = 0; i < n; i++) arr[i] = rand();
}

int main(void) {
    FILE *fp = fopen("results.csv", "w");
    if (!fp) { perror("fopen"); return 1; }
    fprintf(fp, "n,comparisons_2way,comparisons_3way,n_log2_n,n_log3_n\n");

    int sizes[] = {100, 500, 1000, 2000, 4000, 8000, 16000, 32000,
                    64000, 128000, 256000, 512000, 1000000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("%10s %18s %18s %18s\n", "n", "cmp (2-way)", "cmp (3-way)", "ratio 3way/2way");

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        int *arr = malloc(n * sizeof(int));
        int *tmp = malloc(n * sizeof(int));

        /* --- 2-way merge sort --- */
        fillRandom(arr, n, 42);
        comparisons = 0;
        mergeSort2(arr, 0, n, tmp);
        long long cmp2 = comparisons;

        /* --- 3-way merge sort (same input) --- */
        fillRandom(arr, n, 42);
        comparisons = 0;
        mergeSort3(arr, 0, n, tmp);
        long long cmp3 = comparisons;

        double nlog2n = n * (log((double)n) / log(2.0));
        double nlog3n = n * (log((double)n) / log(3.0));

        printf("%10d %18lld %18lld %18.3f\n", n, cmp2, cmp3, (double)cmp3 / cmp2);
        fprintf(fp, "%d,%lld,%lld,%.3f,%.3f\n", n, cmp2, cmp3, nlog2n, nlog3n);

        free(arr);
        free(tmp);
    }

    fclose(fp);
    printf("\nResults written to results.csv\n");
    printf("\nTo plot with gnuplot, run e.g.:\n");
    printf("  gnuplot -persist -e \"set datafile separator ','; set key top left; \\\n");
    printf("    set xlabel 'n'; set ylabel 'comparisons'; \\\n");
    printf("    plot 'results.csv' using 1:2 with linespoints title '2-way merge sort', \\\n");
    printf("         'results.csv' using 1:3 with linespoints title '3-way merge sort'\"\n");

    return 0;
}