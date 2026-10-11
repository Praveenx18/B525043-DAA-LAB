#include <stdio.h>

struct Station {
    int distance;
    int fuel;
};

/* Insert a fuel amount into the max-heap */
void insertHeap(int heap[], int *size, int value) {
    int i;
    int parent;
    int temp;

    (*size)++;
    i = *size;

    heap[i] = value;

    while (i > 1) {
        parent = i / 2;

        if (heap[parent] >= heap[i]) {
            break;
        }

        temp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = temp;

        i = parent;
    }
}

/* Remove and return the largest fuel amount */
int removeMax(int heap[], int *size) {
    int maxValue;
    int i;
    int left;
    int right;
    int largest;
    int temp;

    maxValue = heap[1];

    heap[1] = heap[*size];
    (*size)--;

    i = 1;

    while (1) {
        left = 2 * i;
        right = 2 * i + 1;
        largest = i;

        if (left <= *size && heap[left] > heap[largest]) {
            largest = left;
        }

        if (right <= *size && heap[right] > heap[largest]) {
            largest = right;
        }

        if (largest == i) {
            break;
        }

        temp = heap[i];
        heap[i] = heap[largest];
        heap[largest] = temp;

        i = largest;
    }

    return maxValue;
}

/* Greedy algorithm */
int minimumStops(struct Station station[], int n, int D, int F) {
    int heap[100];
    int heapSize = 0;
    int reachable = F;
    int stops = 0;
    int i = 0;
    int fuel;

    while (reachable < D) {

        /* Add all stations that can currently be reached */
        while (i < n && station[i].distance <= reachable) {
            insertHeap(heap, &heapSize, station[i].fuel);
            i++;
        }

        /* No reachable station means target is impossible */
        if (heapSize == 0) {
            return -1;
        }

        /* Take the largest available fuel */
        fuel = removeMax(heap, &heapSize);

        reachable = reachable + fuel;
        stops++;
    }

    return stops;
}

int main() {
    int D, F, n;
    int i, answer;
    struct Station station[100];

    printf("Enter target distance D: ");
    scanf("%d", &D);

    printf("Enter initial fuel F: ");
    scanf("%d", &F);

    printf("Enter number of stations: ");
    scanf("%d", &n);

    printf("Enter distance and fuel for each station:\n");

    for (i = 0; i < n; i++) {
        scanf("%d %d", &station[i].distance, &station[i].fuel);
    }

    answer = minimumStops(station, n, D, F);

    if (answer == -1) {
        printf("Target cannot be reached.\n");
    } else {
        printf("Minimum number of refuelling stops = %d\n", answer);
    }

    return 0;
}