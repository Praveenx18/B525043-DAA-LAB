#include <stdio.h>
#include <limits.h>

#define MAX 100

struct Block {
    int weight;
    int original;
};

void printBlocks(struct Block blocks[], int n) {
    int i;

    printf("[ ");
    for (i = 0; i < n; i++) {
        printf("%d", blocks[i].weight);

        if (blocks[i].original)
            printf("(O)");
        else
            printf("(A)");

        if (i != n - 1)
            printf(" ");
    }
    printf(" ]\n");
}

/*
   prefix[i] = number of original blocks among
               blocks[0] ... blocks[i-1]

   For i < j, the number of original blocks strictly
   between i and j is prefix[j] - prefix[i + 1].
*/
void buildPrefix(struct Block blocks[], int n, int prefix[]) {
    int i;

    prefix[0] = 0;

    for (i = 0; i < n; i++) {
        prefix[i + 1] = prefix[i] + blocks[i].original;
    }
}

int isCompatible(int left, int right, int prefix[]) {
    if (left > right) {
        int temp = left;
        left = right;
        right = temp;
    }

    if (prefix[right] - prefix[left + 1] == 0)
        return 1;

    return 0;
}

/*
   For every block, find the smallest-weight block
   compatible with it.
*/
void findMinimumCompatibleWeights(struct Block blocks[],
                                  int n,
                                  int prefix[],
                                  int minCompatible[]) {
    int i, j;

    for (i = 0; i < n; i++) {
        minCompatible[i] = INT_MAX;

        for (j = 0; j < n; j++) {
            if (i == j)
                continue;

            if (isCompatible(i, j, prefix)) {
                if (blocks[j].weight < minCompatible[i])
                    minCompatible[i] = blocks[j].weight;
            }
        }
    }
}

/*
   Hu-Tucker chooses a local minimum compatible pair:
   each block must be the minimum-weight compatible
   block for the other block.

   Among several such pairs, choose the one with the
   smallest combined weight.
*/
void findBestPair(struct Block blocks[],
                  int n,
                  int prefix[],
                  int *bestLeft,
                  int *bestRight) {
    int minCompatible[MAX];
    int i, j;
    int bestSum = INT_MAX;

    *bestLeft = -1;
    *bestRight = -1;

    findMinimumCompatibleWeights(blocks, n, prefix,
                                 minCompatible);

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {

            if (isCompatible(i, j, prefix)) {

                if (blocks[i].weight == minCompatible[j] &&
                    blocks[j].weight == minCompatible[i]) {

                    int sum = blocks[i].weight +
                              blocks[j].weight;

                    if (sum < bestSum) {
                        bestSum = sum;
                        *bestLeft = i;
                        *bestRight = j;
                    }
                }
            }
        }
    }
}

void mergeBlocks(struct Block blocks[], int *n,
                 int left, int right) {
    int i;

    blocks[left].weight =
        blocks[left].weight + blocks[right].weight;

    blocks[left].original = 0;

    for (i = right; i < *n - 1; i++) {
        blocks[i] = blocks[i + 1];
    }

    (*n)--;
}

int main() {
    struct Block blocks[MAX];
    int prefix[MAX + 1];
    int n, i;
    int left, right;
    long long totalCost = 0;

    printf("Enter number of weights: ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX) {
        printf("Invalid number of weights.\n");
        return 0;
    }

    printf("Enter the weights in alphabetic order:\n");

    for (i = 0; i < n; i++) {
        scanf("%d", &blocks[i].weight);
        blocks[i].original = 1;
    }

    printf("\nInitial blocks: ");
    printBlocks(blocks, n);

    while (n > 1) {
        buildPrefix(blocks, n, prefix);

        findBestPair(blocks, n, prefix, &left, &right);

        if (left == -1) {
            printf("No valid compatible pair found.\n");
            return 0;
        }

        printf("\nMerge: %d + %d = %d\n",
               blocks[left].weight,
               blocks[right].weight,
               blocks[left].weight +
               blocks[right].weight);

        totalCost += blocks[left].weight +
                     blocks[right].weight;

        mergeBlocks(blocks, &n, left, right);

        printf("Blocks after merge: ");
        printBlocks(blocks, n);
    }

    printf("\nMinimum weighted path-length cost = %lld\n",
           totalCost);

    return 0;
}
