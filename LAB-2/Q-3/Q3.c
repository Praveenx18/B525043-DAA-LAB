#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ---------- Basic merge subroutine: merges two sorted arrays ---------- */
/* a[0..na-1] and b[0..nb-1] -> result[0..na+nb-1]. O(na+nb) */
int* merge(int *a, int na, int *b, int nb) {
    int *result = malloc((na + nb) * sizeof(int));
    int i = 0, j = 0, k = 0;
    while (i < na && j < nb)
        result[k++] = (a[i] <= b[j]) ? a[i++] : b[j++];
    while (i < na) result[k++] = a[i++];
    while (j < nb) result[k++] = b[j++];
    return result;
}

/* ---------- Method 1: Sequential merging ---------- */
/* Merge arr[0] with arr[1], result with arr[2], ... Worst case O(k^2 * n) */
int* merge_k_sequential(int **arrays, int k, int n, int *out_len) {
    int *result = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) result[i] = arrays[0][i];
    int result_len = n;

    for (int i = 1; i < k; i++) {
        int *merged = merge(result, result_len, arrays[i], n);
        free(result);
        result = merged;
        result_len += n;
    }
    *out_len = result_len;
    return result;
}

/* ---------- Method 2: Pairwise (divide and conquer) merging ---------- */
/* Running time: O(kn log k) */
int* merge_k_pairwise(int **arrays, int k, int n, int *out_len) {
    /* current[i] = pointer to i-th sorted chunk, lens[i] = its length */
    int **current = malloc(k * sizeof(int*));
    int *lens = malloc(k * sizeof(int));
    int count = k;

    for (int i = 0; i < k; i++) {
        current[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) current[i][j] = arrays[i][j];
        lens[i] = n;
    }

    while (count > 1) {
        int new_count = (count + 1) / 2;
        int **next = malloc(new_count * sizeof(int*));
        int *next_lens = malloc(new_count * sizeof(int));
        int idx = 0;

        for (int i = 0; i + 1 < count; i += 2) {
            next[idx] = merge(current[i], lens[i], current[i+1], lens[i+1]);
            next_lens[idx] = lens[i] + lens[i+1];
            free(current[i]);
            free(current[i+1]);
            idx++;
        }
        if (count % 2 == 1) {              /* leftover odd array carried forward */
            next[idx] = current[count - 1];
            next_lens[idx] = lens[count - 1];
            idx++;
        }

        free(current);
        free(lens);
        current = next;
        lens = next_lens;
        count = new_count;
    }

    *out_len = lens[0];
    int *result = current[0];
    free(current);
    free(lens);
    return result;
}

/* ---------- Helpers for validation ---------- */
int cmp_int(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int** make_test_arrays(int k, int n) {
    int **arrays = malloc(k * sizeof(int*));
    for (int i = 0; i < k; i++) {
        arrays[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
            arrays[i][j] = rand() % 100000;
        qsort(arrays[i], n, sizeof(int), cmp_int);
    }
    return arrays;
}

void free_arrays(int **arrays, int k) {
    for (int i = 0; i < k; i++) free(arrays[i]);
    free(arrays);
}

int arrays_equal(int *a, int la, int *b, int lb) {
    if (la != lb) return 0;
    for (int i = 0; i < la; i++)
        if (a[i] != b[i]) return 0;
    return 1;
}

/* ---------- Validation ---------- */
void validate(int k, int n, int trials) {
    for (int t = 0; t < trials; t++) {
        int **arrays = make_test_arrays(k, n);

        /* expected: flatten + sort */
        int total = k * n;
        int *expected = malloc(total * sizeof(int));
        int idx = 0;
        for (int i = 0; i < k; i++)
            for (int j = 0; j < n; j++)
                expected[idx++] = arrays[i][j];
        qsort(expected, total, sizeof(int), cmp_int);

        int len1, len2;
        int *r1 = merge_k_sequential(arrays, k, n, &len1);
        int *r2 = merge_k_pairwise(arrays, k, n, &len2);

        if (!arrays_equal(r1, len1, expected, total)) {
            printf("Method 1 FAILED on trial %d\n", t);
        }
        if (!arrays_equal(r2, len2, expected, total)) {
            printf("Method 2 FAILED on trial %d\n", t);
        }

        free(r1); free(r2); free(expected);
        free_arrays(arrays, k);
    }
    printf("Validation passed for k=%d, n=%d (%d trials)\n", k, n, trials);
}

/* ---------- Timing comparison ---------- */
void benchmark(int k, int n) {
    int **arrays = make_test_arrays(k, n);

    clock_t t0 = clock();
    int len1; int *r1 = merge_k_sequential(arrays, k, n, &len1);
    clock_t t1 = clock();
    int len2; int *r2 = merge_k_pairwise(arrays, k, n, &len2);
    clock_t t2 = clock();

    printf("k=%5d, n=%5d | Sequential: %8.4fs | Pairwise: %8.4fs\n",
           k, n,
           (double)(t1 - t0) / CLOCKS_PER_SEC,
           (double)(t2 - t1) / CLOCKS_PER_SEC);

    free(r1); free(r2);
    free_arrays(arrays, k);
}

int main(void) {
    srand((unsigned) time(NULL));

    validate(8, 15, 20);
    validate(7, 20, 20);   /* odd k, tests leftover-array handling */

    printf("\nBenchmark (sequential vs pairwise):\n");
    benchmark(10, 1000);
    benchmark(100, 1000);
    benchmark(500, 500);
    benchmark(1000, 200);

    return 0;
}