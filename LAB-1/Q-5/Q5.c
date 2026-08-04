#include <stdio.h>

/*
 * Array A has a run of 0's followed by a run of 1's.
 * Find the index of the FIRST 1 (i.e., the transition point).
 * If the array is all 0's, return n (no 1's present).
 *
 * Approach: Binary search -> O(log n)
 * We look for the smallest index i such that A[i] == 1.
 */
int findPartitionPoint(int A[], int n) {
    int low = 0, high = n - 1;
    int result = n;  /* default: no 1 found */

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (A[mid] == 1) {
            result = mid;       /* possible answer, look further left */
            high = mid - 1;
        } else {
            low = mid + 1;      /* still in the 0's region, look right */
        }
    }

    return result;
}

/* Simple O(n) linear scan, for comparison */
int findPartitionPointLinear(int A[], int n) {
    for (int i = 0; i < n; i++) {
        if (A[i] == 1) return i;
    }
    return n;
}

int main(void) {
    int A[] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1};
    int n = sizeof(A) / sizeof(A[0]);

    int idxBinary = findPartitionPoint(A, n);
    int idxLinear  = findPartitionPointLinear(A, n);

    printf("Array size: %d\n", n);
    printf("Partition point (binary search): index %d\n", idxBinary);
    printf("Partition point (linear scan)  : index %d\n", idxLinear);

    /* Edge case tests */
    int allZeros[] = {0, 0, 0, 0};
    int allOnes[]   = {1, 1, 1, 1};

    printf("\nAll zeros -> partition index: %d\n",
           findPartitionPoint(allZeros, 4));
    printf("All ones  -> partition index: %d\n",
           findPartitionPoint(allOnes, 4));

    return 0;
}