// ALGORITHM
// Step-by-step

// 1.Read n, k, and T.
// 2.Read the n integers.
// 3.Sort the array.
// 4.Select elements recursively until k-1 elements have been selected.
// 5.Calculate their sum.
// 6.Calculate:
//      required = T - sum
// 7.Perform binary search for required.
// 8.If found, return Yes.
// 9.If all combinations are checked and nothing is found, return No.

#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int binarySearch(int arr[], int n, int target)
{
    int low = 0, high = n - 1, mid;

    while (low <= high)
    {
        mid = (low + high) / 2;

        if (arr[mid] == target)
            return 1;
        else if (arr[mid] < target)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return 0;
}

int findKSum(int arr[], int n, int k, int target, int start, int count, int sum)
{
    int i;

    if (count == k - 1)
    {
        return binarySearch(arr, n, target - sum);
    }

    for (i = start; i < n; i++)
    {
        if (findKSum(arr, n, k, target, i + 1, count + 1, sum + arr[i]))
            return 1;
    }

    return 0;
}

int main()
{
    int n, k, target, i, result;
    int *arr;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter value of k: ");
    scanf("%d", &k);

    printf("Enter target T: ");
    scanf("%d", &target);

    arr = (int *)malloc(n * sizeof(int));

    printf("Enter the elements:\n");
    for (i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    qsort(arr, n, sizeof(int), compare);

    if (k == 1)
        result = binarySearch(arr, n, target);
    else if (k > n)
        result = 0;
    else
        result = findKSum(arr, n, k, target, 0, 0, 0);

    if (result)
        printf("Yes, %d elements can add up to %d.\n", k, target);
    else
        printf("No, %d elements cannot add up to %d.\n", k, target);

    free(arr);

    return 0;
}