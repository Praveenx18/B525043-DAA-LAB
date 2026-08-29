// ALGORITHM:
// 1. Choose a pivot.
// 2. Partition the array around the pivot.
// 3. After partitioning, the pivot reaches its correct position.
// 4. Check whether this position is the position of the median.
// 5. If yes, we have found the median.
// 6. If the median position is on the left, continue on the left part.
// 7. If it is on the right, continue on the right part.
// 8. Repeat until the required element is found.

#include <stdio.h>

int partition(int a[], int low, int high)
{
    int pivot = a[high];
    int i = low - 1;
    int j, temp;

    for (j = low; j < high; j++)
    {
        if (a[j] <= pivot)
        {
            i++;

            temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }

    temp = a[i + 1];
    a[i + 1] = a[high];
    a[high] = temp;

    return i + 1;
}

int quickSelect(int a[], int low, int high, int k)
{
    int pos;

    if (low == high)
        return a[low];

    pos = partition(a, low, high);

    if (k == pos)
        return a[pos];

    if (k < pos)
        return quickSelect(a, low, pos - 1, k);

    return quickSelect(a, pos + 1, high, k);
}

int main()
{
    int a[100], n, i;
    int middle1, middle2;
    float median;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter elements:\n");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &a[i]);
    }

    if (n % 2 == 1)
    {
        middle1 = n / 2;

        median = quickSelect(a, 0, n - 1, middle1);

        printf("Median = %.2f\n", median);
    }
    else
    {
        middle1 = n / 2 - 1;
        middle2 = n / 2;

        int value1 = quickSelect(a, 0, n - 1, middle1);
        int value2 = quickSelect(a, 0, n - 1, middle2);

        median = (value1 + value2) / 2.0;

        printf("Median = %.2f\n", median);
    }

    return 0;
}