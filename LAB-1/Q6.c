#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Method 1: Brute force -- compare every pair -> O(n^2)
 * Returns 1 if a duplicate exists, 0 otherwise.
 */
int hasDuplicateBruteForce(int A[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (A[i] == A[j]) return 1;
        }
    }
    return 0;
}

/* Comparator for qsort */
int cmp(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

/*
 * Method 2: Sort first, then scan adjacent elements -> O(n log n)
 * Sorting brings equal elements next to each other, so a single
 * linear pass after sorting is enough to detect duplicates.
 */
int hasDuplicateSort(int A[], int n) {
    int *B = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) B[i] = A[i];

    qsort(B, n, sizeof(int), cmp);

    int found = 0;
    for (int i = 0; i < n - 1; i++) {
        if (B[i] == B[i + 1]) {
            found = 1;
            break;
        }
    }
    free(B);
    return found;
}

/* Fisher-Yates shuffle of 0..n-1: guarantees NO duplicates, so
   both methods are forced through their true worst-case work. */
void generateUniqueArray(int A[], int n) {
    for (int i = 0; i < n; i++) A[i] = i;
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = A[i]; A[i] = A[j]; A[j] = tmp;
    }
}

int main(void) {
    srand((unsigned) time(NULL));

    int sizes[] = {1000, 5000, 10000, 20000, 40000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("%-8s %-15s %-15s\n", "n", "BruteForce(s)", "SortBased(s)");

    for (int k = 0; k < numSizes; k++) {
        int n = sizes[k];
        int *A = malloc(n * sizeof(int));
        generateUniqueArray(A, n);   /* worst case: no duplicate exists */

        clock_t start = clock();
        int r1 = hasDuplicateBruteForce(A, n);
        double timeBrute = (double)(clock() - start) / CLOCKS_PER_SEC;

        start = clock();
        int r2 = hasDuplicateSort(A, n);
        double timeSort = (double)(clock() - start) / CLOCKS_PER_SEC;

        printf("%-8d %-15.6f %-15.6f\n", n, timeBrute, timeSort);

        if (r1 != r2) {
            printf("  WARNING: methods disagree for n=%d!\n", n);
        }

        free(A);
    }

    return 0;
}