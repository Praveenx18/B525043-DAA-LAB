#include <stdio.h>

struct Interval
{
    int start;
    int end;
};
void merge(struct Interval arr[], int low, int mid, int high)
{
    struct Interval temp[100];
    int i, j, k;

    i = low;
    j = mid + 1;
    k = low;

    while (i <= mid && j <= high)
    {
        if (arr[i].start <= arr[j].start)
        {
            temp[k] = arr[i];
            i++;
        }
        else
        {
            temp[k] = arr[j];
            j++;
        }

        k++;
    }

    while (i <= mid)
    {
        temp[k] = arr[i];
        i++;
        k++;
    }

    while (j <= high)
    {
        temp[k] = arr[j];
        j++;
        k++;
    }

    for (i = low; i <= high; i++)
    {
        arr[i] = temp[i];
    }
}
void mergeSort(struct Interval arr[], int low, int high)
{
    int mid;

    if (low < high)
    {
        mid = (low + high) / 2;

        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);

        merge(arr, low, mid, high);
    }
}



int mergeIntervals(struct Interval arr[], int n, struct Interval result[])
{
    int i, count;

    if (n == 0)
        return 0;

    result[0] = arr[0];
    count = 1;

    for (i = 1; i < n; i++)
    {
        if (arr[i].start <= result[count - 1].end)
        {
            if (arr[i].end > result[count - 1].end)
            {
                result[count - 1].end = arr[i].end;
            }
        }
        else
        {
            result[count] = arr[i];
            count++;
        }
    }

    return count;
}

int main()
{
    struct Interval arr[100], result[100];
    int n, i, count;

    printf("Enter number of intervals: ");
    scanf("%d", &n);

    printf("Enter the intervals (start end):\n");

    for (i = 0; i < n; i++)
    {
        scanf("%d %d", &arr[i].start, &arr[i].end);
    }

    mergeSort(arr, 0, n - 1);

    count = mergeIntervals(arr, n, result);

    printf("Merged intervals are:\n");

    for (i = 0; i < count; i++)
    {
        printf("(%d, %d) ", result[i].start, result[i].end);
    }

    return 0;
}