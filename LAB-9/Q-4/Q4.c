#include <stdio.h>

#define MAX 100

// Function to swap two numbers
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Insert a value into the min heap
void insertHeap(int heap[], int *size, int value)
{
    int i;

    // Increase heap size
    *size = *size + 1;

    // Insert the value at the end
    i = *size - 1;
    heap[i] = value;

    // Move the value upward if it is smaller than its parent
    while (i > 0 && heap[(i - 1) / 2] > heap[i])
    {
        swap(&heap[(i - 1) / 2], &heap[i]);

        i = (i - 1) / 2;
    }
}

// Remove and return the minimum element
int deleteMin(int heap[], int *size)
{
    int minValue;
    int i, child;
    int temp;

    // Minimum element is at the root
    minValue = heap[0];

    // Move the last element to the root
    heap[0] = heap[*size - 1];

    // Decrease heap size
    *size = *size - 1;

    i = 0;

    // Move the root downward to restore min heap property
    while (2 * i + 1 < *size)
    {
        child = 2 * i + 1;

        // Find the smaller child
        if (child + 1 < *size &&
            heap[child + 1] < heap[child])
        {
            child = child + 1;
        }

        // If parent is already smaller, heap property is satisfied
        if (heap[i] <= heap[child])
        {
            break;
        }

        // Swap parent and smaller child
        temp = heap[i];
        heap[i] = heap[child];
        heap[child] = temp;

        i = child;
    }

    return minValue;
}

int main()
{
    int heap[MAX];
    int n;
    int size = 0;
    int i;

    int first;
    int second;
    int newStick;

    int totalCost = 0;

    printf("Enter number of sticks: ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX)
    {
        printf("Invalid number of sticks.\n");
        return 0;
    }

    printf("Enter lengths of sticks:\n");

    // Read sticks and insert them into the min heap
    for (i = 0; i < n; i++)
    {
        int length;

        scanf("%d", &length);

        insertHeap(heap, &size, length);
    }

    // Continue until only one stick remains
    while (size > 1)
    {
        // Get two smallest sticks
        first = deleteMin(heap, &size);
        second = deleteMin(heap, &size);

        // Connect them
        newStick = first + second;

        // Add connection cost
        totalCost = totalCost + newStick;

        // Insert the new stick back into heap
        insertHeap(heap, &size, newStick);
    }

    printf("Minimum total cost = %d\n", totalCost);

    return 0;
}