#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define MAX_CODE 105

typedef struct Node {
    char symbol;
    int frequency;
    int minSymbol;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    Node *data[MAX * 2];
    int size;
} MinHeap;

typedef struct {
    char symbol;
    int frequency;
    int length;
    char code[MAX_CODE];
} CodeInfo;

/* Create a new tree node */
Node *createNode(char symbol, int frequency) {
    Node *node = (Node *)malloc(sizeof(Node));

    node->symbol = symbol;
    node->frequency = frequency;
    node->minSymbol = (unsigned char)symbol;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/* Return 1 if node a should come before node b */
int smaller(Node *a, Node *b) {
    if (a->frequency != b->frequency)
        return a->frequency < b->frequency;

    return a->minSymbol < b->minSymbol;
}

/* Insert a node into the min-heap */
void insertHeap(MinHeap *heap, Node *node) {
    int i = heap->size;

    heap->data[i] = node;
    heap->size++;

    while (i > 0) {
        int parent = (i - 1) / 2;

        if (smaller(heap->data[parent], heap->data[i]))
            break;

        Node *temp = heap->data[parent];
        heap->data[parent] = heap->data[i];
        heap->data[i] = temp;

        i = parent;
    }
}

/* Remove and return the smallest node */
Node *removeMin(MinHeap *heap) {
    Node *result = heap->data[0];

    heap->size--;
    heap->data[0] = heap->data[heap->size];

    int i = 0;

    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallestIndex = i;

        if (left < heap->size &&
            smaller(heap->data[left], heap->data[smallestIndex])) {
            smallestIndex = left;
        }

        if (right < heap->size &&
            smaller(heap->data[right], heap->data[smallestIndex])) {
            smallestIndex = right;
        }

        if (smallestIndex == i)
            break;

        Node *temp = heap->data[i];
        heap->data[i] = heap->data[smallestIndex];
        heap->data[smallestIndex] = temp;

        i = smallestIndex;
    }

    return result;
}

/* Build the Huffman tree */
Node *buildHuffmanTree(char symbols[], int freq[], int n) {
    MinHeap heap;
    heap.size = 0;

    int i;

    for (i = 0; i < n; i++) {
        insertHeap(&heap, createNode(symbols[i], freq[i]));
    }

    while (heap.size > 1) {
        Node *left = removeMin(&heap);
        Node *right = removeMin(&heap);

        Node *parent = createNode('\0',
                                  left->frequency + right->frequency);

        parent->minSymbol =
            left->minSymbol < right->minSymbol ?
            left->minSymbol : right->minSymbol;

        parent->left = left;
        parent->right = right;

        insertHeap(&heap, parent);
    }

    return removeMin(&heap);
}

/* Find code lengths by traversing the Huffman tree */
void findLengths(Node *root, CodeInfo info[], int depth) {
    if (root == NULL)
        return;

    /* If this is a leaf */
    if (root->left == NULL && root->right == NULL) {
        int i;

        for (i = 0; i < MAX; i++) {
            if (info[i].symbol == root->symbol) {
                info[i].length = depth;
                return;
            }
        }
    }

    findLengths(root->left, info, depth + 1);
    findLengths(root->right, info, depth + 1);
}

/* Sort by code length, then by symbol */
void sortCodeInfo(CodeInfo info[], int n) {
    int i, j;

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (info[j].length < info[i].length ||
                (info[j].length == info[i].length &&
                 info[j].symbol < info[i].symbol)) {

                CodeInfo temp = info[i];
                info[i] = info[j];
                info[j] = temp;
            }
        }
    }
}

/* Make a binary string containing zeroes */
void makeZeros(char code[], int length) {
    int i;

    for (i = 0; i < length; i++)
        code[i] = '0';

    code[length] = '\0';
}

/* Increase a binary string by 1 */
void incrementBinary(char code[]) {
    int i = (int)strlen(code) - 1;

    while (i >= 0 && code[i] == '1') {
        code[i] = '0';
        i--;
    }

    if (i >= 0)
        code[i] = '1';
}

/* Generate canonical Huffman codes */
void makeCanonicalCodes(CodeInfo info[], int n) {
    int i;

    makeZeros(info[0].code, info[0].length);

    for (i = 1; i < n; i++) {
        int oldLength = info[i - 1].length;
        int newLength = info[i].length;
        int j;

        incrementBinary(info[i - 1].code);

        strcpy(info[i].code, info[i - 1].code);

        for (j = oldLength; j < newLength; j++)
            info[i].code[j] = '0';

        info[i].code[newLength] = '\0';
    }
}

/* Free the Huffman tree */
void freeTree(Node *root) {
    if (root == NULL)
        return;

    freeTree(root->left);
    freeTree(root->right);

    free(root);
}

int main() {
    int n;
    char symbols[MAX];
    int freq[MAX];
    CodeInfo info[MAX];

    int i;
    int totalFrequency = 0;
    int weightedLength = 0;

    printf("Enter number of symbols: ");
    scanf("%d", &n);

    if (n < 2 || n > MAX) {
        printf("Number of symbols must be between 2 and %d.\n", MAX);
        return 0;
    }

    printf("Enter symbol and frequency:\n");

    for (i = 0; i < n; i++) {
        scanf(" %c %d", &symbols[i], &freq[i]);

        info[i].symbol = symbols[i];
        info[i].frequency = freq[i];
        info[i].length = 0;
        info[i].code[0] = '\0';

        totalFrequency += freq[i];
    }

    Node *root = buildHuffmanTree(symbols, freq, n);

    findLengths(root, info, 0);

    sortCodeInfo(info, n);

    makeCanonicalCodes(info, n);

    printf("\nCanonical Huffman Codebook:\n");
    printf("Symbol  Frequency  Length  Code\n");

    for (i = 0; i < n; i++) {
        printf("  %c       %d        %d     %s\n",
               info[i].symbol,
               info[i].frequency,
               info[i].length,
               info[i].code);

        weightedLength += info[i].frequency * info[i].length;
    }

    printf("\nTotal frequency = %d\n", totalFrequency);
    printf("Total weighted code length = %d\n", weightedLength);
    printf("Average code length = %.3f bits/symbol\n",
           (double)weightedLength / totalFrequency);

    freeTree(root);

    return 0;
}