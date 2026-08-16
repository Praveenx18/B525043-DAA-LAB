#include <stdio.h>
#include <stdlib.h>
int binarySearch(int a[], int n, int x, int *comparisons)
{
    int low = 0, high = n - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        (*comparisons)++;
        if (a[mid] == x)
            return mid;
        if (x < a[mid])
            high = mid - 1;
        else
            low = mid + 1;
    }
    return -1;
}
int ternarySearch(int a[], int n, int x, int *comparisons)
{
    int low = 0, high = n - 1;
    while (low <= high)
    {
        int third = (high - low) / 3;
        int mid1 = low + third;
        int mid2 = high - third;
        (*comparisons)++;
        if (a[mid1] == x)
            return mid1;
        (*comparisons)++;
        if (a[mid2] == x)
            return mid2;
        if (x < a[mid1])
        {
            high = mid1 - 1;
        }
        else if (x > a[mid2])
        {
            low = mid2 + 1;
        }
        else
        {
            low = mid1 + 1;
            high = mid2 - 1;
        }
    }
    return -1;
}
int main()
{
    int n, x;
    int *a;
    printf("Enter number of elements: ");
    scanf("%d", &n);
    a = (int *)malloc(n * sizeof(int));
    printf("Enter %d sorted elements:\n", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);
    printf("Enter element to search: ");
    scanf("%d", &x);
    int binaryComparisons = 0;
    int ternaryComparisons = 0;
    int binaryResult = binarySearch(a, n, x, &binaryComparisons);
    int ternaryResult = ternarySearch(a, n, x, &ternaryComparisons);
    printf("\n--- Binary Search ---\n");
    if (binaryResult != -1)
        printf("Element found at index %d\n", binaryResult);
    else
        printf("Element not found\n");
    printf("Comparisons: %d\n", binaryComparisons);
    printf("\n--- Ternary Search ---\n");
    if (ternaryResult != -1)
        printf("Element found at index %d\n", ternaryResult);
    else
        printf("Element not found\n");
    printf("Comparisons: %d\n", ternaryComparisons);
    printf("\n--- Comparison ---\n");
    if (binaryComparisons < ternaryComparisons)
        printf("Binary search performed fewer comparisons.\n");
    else if (ternaryComparisons < binaryComparisons)
        printf("Ternary search performed fewer comparisons.\n");
    else
        printf("Both performed the same number of comparisons.\n");
    free(a);
    return 0;
}