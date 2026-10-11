#include <stdio.h>

#define MAX 100

/* Swap two integers */
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* Insert an element into max-heap */
void insertHeap(int heap[], int *size, int value)
{
    int i;

    (*size)++;
    i = *size - 1;

    heap[i] = value;

    /* Move the new element upward */
    while (i > 0)
    {
        int parent = (i - 1) / 2;

        if (heap[parent] >= heap[i])
            break;

        swap(&heap[parent], &heap[i]);
        i = parent;
    }
}

/* Remove and return maximum element */
int removeMax(int heap[], int *size)
{
    int maximum;
    int i = 0;

    maximum = heap[0];

    heap[0] = heap[*size - 1];
    (*size)--;

    /* Move the root downward */
    while (1)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if (left < *size && heap[left] > heap[largest])
            largest = left;

        if (right < *size && heap[right] > heap[largest])
            largest = right;

        if (largest == i)
            break;

        swap(&heap[i], &heap[largest]);
        i = largest;
    }

    return maximum;
}

/* Find minimum value in the heap */
int findMin(int heap[], int size)
{
    int i;
    int minimum = heap[0];

    for (i = 1; i < size; i++)
    {
        if (heap[i] < minimum)
            minimum = heap[i];
    }

    return minimum;
}

int main()
{
    int n;
    int arr[MAX];
    int heap[MAX];
    int heapSize = 0;
    int minimum;
    int maximum;
    int deviation;
    int answer;
    int i;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter the elements:\n");

    for (i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);

        /* Make every odd number even */
        if (arr[i] % 2 != 0)
            arr[i] = arr[i] * 2;

        insertHeap(heap, &heapSize, arr[i]);
    }

    /* Find the initial minimum */
    minimum = findMin(heap, heapSize);

    /* Root of max-heap is maximum */
    maximum = heap[0];

    /* Initial deviation */
    answer = maximum - minimum;

    /*
       Repeatedly reduce the current maximum
       as long as it is even.
    */
    while (1)
    {
        maximum = removeMax(heap, &heapSize);

        /* Cannot divide an odd number */
        if (maximum % 2 != 0)
            break;

        maximum = maximum / 2;

        /* Update minimum */
        if (maximum < minimum)
            minimum = maximum;

        /* Put reduced value back into heap */
        insertHeap(heap, &heapSize, maximum);

        /* Calculate current deviation */
        deviation = heap[0] - minimum;

        /* Store the best answer */
        if (deviation < answer)
            answer = deviation;
    }

    printf("Minimum possible deviation = %d\n", answer);

    return 0;
}