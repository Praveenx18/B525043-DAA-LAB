#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(int arr[], int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i)
    {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n)
{
    int i;

    for (i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (i = n - 1; i > 0; i--)
    {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

int main()
{
    int n, i;
    int *arr;
    FILE *fp;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    if (n <= 0)
    {
        printf("Number of elements must be positive.\n");
        return 1;
    }

    arr = (int *)malloc(n * sizeof(int));

    if (arr == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    srand((unsigned int)time(NULL));

    fp = fopen("input.txt", "w");
    if (fp == NULL)
    {
        printf("Could not open input.txt\n");
        free(arr);
        return 1;
    }

    printf("\nRandomly generated elements:\n");

    for (i = 0; i < n; i++)
    {
        arr[i] = rand() % 1000 + 1;
        printf("%d ", arr[i]);
        fprintf(fp, "%d ", arr[i]);
    }

    fclose(fp);

    heapSort(arr, n);

    printf("\n\nSorted elements using Heap Sort:\n");
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);

    printf("\n\nThe generated elements are also stored in input.txt\n");

    free(arr);
    return 0;
}
