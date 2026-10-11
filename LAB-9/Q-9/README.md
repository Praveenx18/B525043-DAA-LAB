# Hu–Tucker Greedy Simulation

## 1. Problem Title

**Hu–Tucker Greedy Simulation for an Optimal Alphabetic Binary Search Tree**

## 2. Problem Statement

Given an ordered sequence of positive weights:

`w1, w2, ..., wn`

construct an optimal alphabetic binary search tree, or equivalently its Hu–Tucker merge pattern, such that:

`Σ wi × depth(i)`

is minimum.

The important restriction is that the original leaves must remain in their given left-to-right order.

## 3. Objective

The objective of this program is to simulate the **combination/merge phase of the Hu–Tucker algorithm** using simple C arrays and structures.

The program:

- preserves the input order;
- identifies compatible blocks;
- finds a local minimum compatible pair;
- merges that pair;
- prints every merge;
- calculates the weighted path-length cost.

This is an educational direct implementation, not a production optimized implementation.

## 4. Hu–Tucker vs. Simple Huffman Greedy

A very important point is that Hu–Tucker is **not** simply:

> “Find the two smallest weights and merge them.”

That is the Huffman rule.

For alphabetic trees, the order of the original leaves must be preserved. Hu–Tucker therefore uses **compatibility**.

Two active blocks are compatible when every block between them is artificial (created by an earlier merge). The Hu–Tucker rule then chooses a **local minimum compatible pair**: each member is a minimum-weight block among the blocks compatible with the other.

This extra rule is what keeps the construction suitable for the alphabetic-tree problem.

## 5. Basic Idea

Initially every input weight is an original block.

At every step:

1. Determine which pairs are compatible.
2. For every block, find the smallest-weight block compatible with it.
3. Find pairs that are mutual minimum compatible partners.
4. Among these local minimum compatible pairs, choose the one with the smallest combined weight.
5. Merge the pair.
6. Mark the new block as artificial.
7. Add the merged weight to the total cost.
8. Continue until one block remains.

The merge cost works because merging two subtrees increases the depth of every leaf in those subtrees by one. Therefore the increase in weighted path length is exactly the sum of the two subtree weights.

Thus:

`Total cost = sum of all merge weights`

The combination phase produces the level/merge information used to obtain an optimal alphabetic tree.

## 6. Why This Approach Is Suitable

The Hu–Tucker algorithm is specifically designed for optimal alphabetic binary trees. The input order is important, so ordinary Huffman merging is not enough.

For a college lab, a direct array simulation is easier to understand than the full optimized O(n log n) implementation, while still demonstrating the important greedy rule.

## 7. Algorithm

1. Read `n`.
2. Read the weights in their required alphabetic order.
3. Store each weight as a block and mark it as original.
4. Repeat until only one block remains:
   - build a prefix array containing counts of original blocks;
   - use it to test compatibility in O(1);
   - for each block, find its minimum-weight compatible partner;
   - identify mutual minimum compatible pairs;
   - choose the pair with minimum combined weight;
   - add the combined weight to `totalCost`;
   - replace the pair by one artificial block.
5. Print the final cost.

## 8. Complete C Program

```c
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

```

## 9. Important Variables

### `struct Block`

Each active block stores:

- `weight` — total weight represented by the block.
- `original` — `1` if it is an original input block, `0` if it was created by a merge.

### `prefix[]`

`prefix[i]` stores the number of original blocks before position `i`.

It lets the program check whether an interval contains an original block without scanning the entire interval.

### `minCompatible[]`

For each active block, this stores the smallest weight of a compatible block.

It is used to identify mutual local-minimum compatible pairs.

### `totalCost`

Stores the sum of all merge weights. This equals the weighted path-length cost produced by the combination pattern.

## 10. Important Functions

### `printBlocks()`

Prints the current active blocks.

`O` means original and `A` means artificial.

### `buildPrefix()`

Builds the prefix count of original blocks.

This makes the compatibility test fast.

### `isCompatible()`

Checks whether two positions have only artificial blocks between them.

It returns `1` for compatible and `0` otherwise.

### `findMinimumCompatibleWeights()`

For every block, scans all other blocks and stores the smallest weight that is compatible with it.

### `findBestPair()`

Implements the main Hu–Tucker greedy selection.

A pair `(i,j)` is considered when:

- `i` and `j` are compatible;
- `j` is a minimum compatible partner of `i`;
- `i` is a minimum compatible partner of `j`.

Among such pairs, the smallest combined weight is selected.

### `mergeBlocks()`

Combines the selected pair and removes the second block by shifting later elements.

### `main()`

Reads input, initializes the blocks, repeatedly performs the Hu–Tucker merge, and prints the final cost.

## 11. Sample Input

```text
Enter number of weights: 5
Enter the weights in alphabetic order:
5 2 4 3 6
```

## 12. Sample Output

```text
Initial blocks: [ 5(O) 2(O) 4(O) 3(O) 6(O) ]

Merge: 2 + 4 = 6
Blocks after merge: [ 5(O) 6(A) 3(O) 6(O) ]

Merge: 5 + 3 = 8
Blocks after merge: [ 8(A) 6(A) 6(O) ]

Merge: 6 + 6 = 12
Blocks after merge: [ 8(A) 12(A) ]

Merge: 8 + 12 = 20
Blocks after merge: [ 20(A) ]

Minimum weighted path-length cost = 46
```

## 13. Complete Dry Run

Use:

`5, 2, 4, 3, 6`

### Step 1

All blocks are original:

`5(O) 2(O) 4(O) 3(O) 6(O)`

The compatible pairs are adjacent pairs.

The pair `2,4` is a mutual minimum compatible pair:

- for weight 2, the lowest compatible weight is 4;
- for weight 4, the lowest compatible weight is 2.

So merge:

`2 + 4 = 6`

Cost:

`6`

State:

`5(O) 6(A) 3(O) 6(O)`

### Step 2

Now the artificial block `6(A)` can be crossed when checking compatibility.

The pair `5,3` is compatible because the block between them is artificial.

It is a mutual minimum compatible pair:

- 5's smallest compatible block is 3;
- 3's smallest compatible block is 5.

Merge:

`5 + 3 = 8`

Total cost:

`6 + 8 = 14`

State:

`8(A) 6(A) 6(O)`

### Step 3

The two 6-weight blocks are adjacent and compatible.

Merge:

`6 + 6 = 12`

Total:

`14 + 12 = 26`

State:

`8(A) 12(A)`

### Step 4

Only two blocks remain.

Merge:

`8 + 12 = 20`

Final cost:

`26 + 20 = 46`

Therefore:

`Minimum weighted path-length cost = 46`

## 14. Dry-Run Table

| Step | Active blocks | Selected pair | Merge weight | Total cost |
|---|---|---|---:|---:|
| 0 | 5(O), 2(O), 4(O), 3(O), 6(O) | 2 + 4 | 6 | 6 |
| 1 | 5(O), 6(A), 3(O), 6(O) | 5 + 3 | 8 | 14 |
| 2 | 8(A), 6(A), 6(O) | 6 + 6 | 12 | 26 |
| 3 | 8(A), 12(A) | 8 + 12 | 20 | 46 |

## 15. Complexity Analysis

This program is deliberately a direct educational implementation.

There are `n - 1` merge steps.

For one merge step:

1. Building the prefix array takes `O(n)`.
2. Finding the minimum compatible partner for every block checks all pairs, so it takes `O(n²)`.
3. Finding the mutual candidate pair also takes `O(n²)`.

The compatibility check itself is `O(1)` because the prefix array tells us the number of original blocks between two positions immediately.

Therefore, one merge step costs:

`O(n²)`

There are `n - 1 = O(n)` merge steps.

So the total time is:

`O(n) × O(n²) = O(n³)`

### Best case

`O(n³)` for this implementation because the program still performs the repeated pair scans even when a suitable pair is found early.

### Average case

`O(n³)` as an asymptotic bound for the direct implementation.

### Worst case

`O(n³)`.

### Space complexity

The program uses arrays of size proportional to `n`:

- blocks
- prefix
- minimum-compatible weights

Therefore:

`O(n)`

extra space.

## 16. Theoretical Optimized Hu–Tucker Complexity

The original Hu–Tucker algorithm has implementations with better asymptotic complexity than this simple lab simulation. The literature reports an O(n log n) implementation using appropriate data structures. The original 1971 paper also presented an implementation requiring quadratic-order operations.

Therefore, do not confuse the **O(n³) complexity of this simple teaching implementation** with the best-known implementation complexity of the Hu–Tucker algorithm itself.

## 17. Complexity Graph

The attached graph:

`hu_tucker_complexity.png`

plots the theoretical O(n³) growth of this particular direct implementation.

The graph is not experimental timing data. It represents the mathematical growth of the operation bound discussed above.

## 18. Advantages

1. Preserves the required alphabetic order.
2. Demonstrates the actual Hu–Tucker compatibility idea.
3. Uses simple arrays and a structure.
4. Easy to trace during a lab demonstration.
5. Prints every merge.
6. Calculates weighted path length through merge weights.
7. Uses a prefix array to keep compatibility checking simple and efficient.

## 19. Disadvantages / Limitations

1. It is slower than optimized Hu–Tucker implementations.
2. It repeatedly scans pairs.
3. It focuses on the combination/merge phase rather than a full sophisticated tree-reconstruction implementation.
4. It is intended for educational use and small-to-medium inputs.
5. Equal-weight tie cases require consistent tie-breaking.

## 20. Hu–Tucker vs Huffman

| Feature | Huffman | Hu–Tucker |
|---|---|---|
| Main goal | Minimum weighted binary tree/code | Minimum weighted alphabetic tree |
| Preserves original order | No general guarantee | Yes |
| Pair selection | Two smallest weights | Mutual local-minimum compatible pair |
| Suitable for alphabetic keys | Not generally | Yes |
| Greedy | Yes | Yes |

## 21. Viva Questions

### Q1. What is the main idea of Hu–Tucker?

It constructs an optimal alphabetic binary tree by repeatedly merging suitable compatible blocks using a greedy rule.

### Q2. What does alphabetic mean?

The original leaves must appear in the same left-to-right order as the input.

### Q3. Why not use Huffman coding?

Huffman can combine nodes without respecting the original order, so it does not generally solve the alphabetic-tree problem.

### Q4. What is a compatible pair?

Two blocks are compatible if every block between them is artificial.

### Q5. What is a local minimum compatible pair?

For a pair `(x,y)`, each one must be a minimum-weight compatible partner of the other.

### Q6. Why do we add the merged weight to the cost?

Every leaf in the merged subtrees becomes one level deeper. The increase in weighted path length is the sum of their weights.

### Q7. How many merges are required for n leaves?

Exactly `n - 1`.

### Q8. What is the time complexity of this program?

`O(n³)` because there are `O(n)` merge rounds and each round performs `O(n²)` pair processing.

### Q9. What is the space complexity?

`O(n)`.

### Q10. Why is a prefix array used?

It lets us check whether there is an original block between two positions in constant time.

### Q11. What does `original = 1` mean?

The block is an original input weight.

### Q12. What does `original = 0` mean?

The block was created by an earlier merge.

### Q13. Why is the merged block marked artificial?

It is no longer an original leaf, so future compatible pairs may merge across it.

### Q14. Is this the fastest Hu–Tucker implementation?

No. This is a simplified teaching implementation. More advanced implementations can achieve O(n log n) time.

### Q15. What happens when only one block remains?

The algorithm stops because all weights have been combined into one root block.

## 22. C-Code-Specific Viva Questions

### Q16. Why is `INT_MAX` used?

It represents an initially very large value so that the first valid minimum can replace it.

### Q17. Why are two nested loops used in `findMinimumCompatibleWeights()`?

They compare each block with every other block to find its smallest compatible partner.

### Q18. Why is `prefix[right] - prefix[left + 1]` used?

It counts original blocks strictly between `left` and `right`. If the count is zero, the pair is compatible.

### Q19. Why does `mergeBlocks()` shift array elements?

The second block is removed, so later blocks must move left to keep the active sequence continuous.

### Q20. Why is `long long` used for `totalCost`?

The total can become larger than the range safely expected from a normal `int` when input weights grow.

## 23. Compile and Run

### GCC / Linux / macOS

```bash
gcc hu_tucker.c -o hu_tucker
./hu_tucker
```

### Windows with MinGW

```bash
gcc hu_tucker.c -o hu_tucker.exe
hu_tucker.exe
```

## 24. Conclusion

Hu–Tucker is a greedy algorithm designed for optimal alphabetic binary trees. Its major difference from Huffman coding is that it must preserve the original order of the leaves.

The program above demonstrates the important idea using a simple array representation, a compatibility test, mutual minimum compatible pairs, and repeated merging. It is intentionally written for a second-year DAA lab so that every major step can be explained clearly in a viva.
