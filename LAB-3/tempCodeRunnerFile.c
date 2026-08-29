#include <stdio.h>
long long comparisons = 0;
typedef struct {
    int max;
    int min;
} MaxMin;
MaxMin findMaxMin(int arr[], int low, int high) {
    MaxMin result, left, right;
    if (low == high) {
        result.max = arr[low];
        result.min = arr[low];
        return result;
    }
    if (high == low + 1) {
        comparisons++;
        if (arr[low] < arr[high]) {
            result.min = arr[low];
            result.max = arr[high];
        } else {
            result.min = arr[high];
            result.max = arr[low];
        }
        return result;
    }
    int mid = (low + high) / 2;
    left = findMaxMin(arr, low, mid);
    right = findMaxMin(arr, mid + 1, high);
    comparisons++;
    result.max = (left.max > right.max) ? left.max : right.max;
    comparisons++;
    result.min = (left.min < right.min) ? left.min : right.min;
    return result;
}
int main() {
    int arr[] = {22, 13, 45, 8, 71, 34, 5, 99, 27, 60};
    int n = sizeof(arr) / sizeof(arr[0]);
    MaxMin res = findMaxMin(arr, 0, n - 1);
    printf("Array size (n) = %d\n", n);
    printf("Maximum = %d\n", res.max);
    printf("Minimum = %d\n", res.min);
    printf("Total comparisons = %lld\n", comparisons);
    printf("Upper bound (3n/2) = %.1f\n", 1.5 * n);
    return 0;
}