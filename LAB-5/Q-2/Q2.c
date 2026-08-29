#include <stdio.h>

/* Partition the array around a pivot */
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

/* Find the kth smallest element using Quickselect */
int quickSelect(int a[], int low, int high, int k)
{
    int pos;

    if (low <= high)
    {
        pos = partition(a, low, high);

        /* pos is zero-based, so compare with k - 1 */
        if (pos == k - 1)
            return a[pos];

        if (pos > k - 1)
            return quickSelect(a, low, pos - 1, k);

        return quickSelect(a, pos + 1, high, k);
    }

    return -1;
}

int main()
{
    int a[100], n, k, i, answer;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter %d elements:\n", n);
    for (i = 0; i < n; i++)
        scanf("%d", &a[i]);

    printf("Enter k: ");
    scanf("%d", &k);

    if (k < 1 || k > n)
    {
        printf("Invalid value of k.\n");
        return 0;
    }

    answer = quickSelect(a, 0, n - 1, k);

    printf("%dth smallest element = %d\n", k, answer);

    return 0;
}
