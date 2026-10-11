#include <stdio.h>
#include <string.h>

#define MAX 1000
#define CHAR_SET 256

typedef struct {
    char ch;
    int freq;
} Node;

typedef struct {
    Node data[CHAR_SET];
    int size;
} MaxHeap;

typedef struct {
    Node data[CHAR_SET];
    int front;
    int rear;
    int size;
} Queue;


/* ---------- Max Heap Functions ---------- */

void swap(Node *a, Node *b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MaxHeap *heap, int index) {
    int parent;

    while (index > 0) {
        parent = (index - 1) / 2;

        if (heap->data[parent].freq >= heap->data[index].freq)
            break;

        swap(&heap->data[parent], &heap->data[index]);
        index = parent;
    }
}

void heapifyDown(MaxHeap *heap, int index) {
    int left, right, largest;

    while (1) {
        left = 2 * index + 1;
        right = 2 * index + 2;
        largest = index;

        if (left < heap->size &&
            heap->data[left].freq > heap->data[largest].freq)
            largest = left;

        if (right < heap->size &&
            heap->data[right].freq > heap->data[largest].freq)
            largest = right;

        if (largest == index)
            break;

        swap(&heap->data[index], &heap->data[largest]);
        index = largest;
    }
}

void insertHeap(MaxHeap *heap, Node item) {
    heap->data[heap->size] = item;
    heap->size++;

    heapifyUp(heap, heap->size - 1);
}

Node removeMax(MaxHeap *heap) {
    Node maxItem = heap->data[0];

    heap->size--;

    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];
        heapifyDown(heap, 0);
    }

    return maxItem;
}


/* ---------- Queue Functions ---------- */

void enqueue(Queue *queue, Node item) {
    queue->data[queue->rear] = item;
    queue->rear = (queue->rear + 1) % CHAR_SET;
    queue->size++;
}

Node dequeue(Queue *queue) {
    Node item = queue->data[queue->front];

    queue->front = (queue->front + 1) % CHAR_SET;
    queue->size--;

    return item;
}


/* ---------- Main Algorithm ---------- */

void reorganizeString(char str[], int k) {

    int frequency[CHAR_SET] = {0};

    MaxHeap heap;
    Queue waitQueue;

    char result[MAX];

    int n = strlen(str);
    int i;
    int resultIndex = 0;

    heap.size = 0;

    waitQueue.front = 0;
    waitQueue.rear = 0;
    waitQueue.size = 0;


    /* Count frequency of every character */

    for (i = 0; i < n; i++) {
        frequency[(unsigned char)str[i]]++;
    }


    /* Put characters into the max heap */

    for (i = 0; i < CHAR_SET; i++) {

        if (frequency[i] > 0) {

            Node item;

            item.ch = (char)i;
            item.freq = frequency[i];

            insertHeap(&heap, item);
        }
    }


    /*
       Waiting characters and their release positions.
    */

    Node waitingChar[MAX];
    int releaseAt[MAX];

    int waitFront = 0;
    int waitRear = 0;


    while (resultIndex < n) {


        /* Release characters whose waiting period is over */

        while (waitFront < waitRear &&
               releaseAt[waitFront] <= resultIndex) {

            insertHeap(&heap, waitingChar[waitFront]);

            waitFront++;
        }


        /* No character is available */

        if (heap.size == 0) {

            printf("Rearrangement is not possible.\n");

            return;
        }


        /* Choose the character with maximum frequency */

        Node current = removeMax(&heap);


        /* Put it into the result */

        result[resultIndex] = current.ch;

        resultIndex++;


        /* One occurrence has been used */

        current.freq--;


        /*
           If more occurrences remain,
           make the character wait for K positions.
        */

        if (current.freq > 0) {

            waitingChar[waitRear] = current;

            releaseAt[waitRear] = resultIndex + k - 1;

            waitRear++;
        }
    }


    /* End the C string */

    result[resultIndex] = '\0';


    printf("Rearranged string: %s\n", result);
}


int main() {

    char str[MAX];
    int k;


    printf("Enter the string: ");
    scanf("%s", str);


    printf("Enter K: ");
    scanf("%d", &k);


    if (k <= 1) {

        printf("Rearranged string: %s\n", str);

    } else {

        reorganizeString(str, k);
    }


    return 0;
}