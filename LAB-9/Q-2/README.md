# Huffman Coding in C

## 1. Problem Title

**Huffman Coding — Minimum Expected Length and Canonical Huffman Codebook**

## 2. Problem Statement

Given a set of symbols and their frequencies, construct a prefix-free binary code with minimum expected code length using Huffman coding.

After constructing the Huffman tree, output the **canonical Huffman codebook**. In the canonical codebook, codes are arranged by increasing code length; symbols having the same code length are ordered by symbol.

The program also validates the generated code lengths by constructing the Huffman tree and displays the resulting canonical codes.

---

## 3. Objective

The objectives are:

1. Store symbols and their frequencies.
2. Construct a Huffman tree using a min-heap.
3. Find the code length of every symbol.
4. Convert those lengths into canonical Huffman codes.
5. Calculate the weighted/expected code length.
6. Analyze the time and space complexity.

---

## 4. Basic Idea

Huffman coding is a **greedy algorithm**.

The two symbols/subtrees with the smallest frequencies are repeatedly combined into a new subtree.

For example, if the two smallest frequencies are 5 and 9:

```text
5 + 9 = 14
```

The new subtree has frequency 14 and is inserted back into the min-heap.

This continues until only one tree remains.

The Huffman tree gives the code length of every symbol. We then generate the **canonical** version of those codes from the code lengths.

### Why Huffman coding is suitable

Huffman coding is suitable because the greedy choice of repeatedly combining the two least frequent nodes produces an optimal prefix-free code for the given frequencies.

---

## 5. Input Representation

Each input symbol is represented by:

- `symbol` — a character such as A, B, C
- `frequency` — its frequency

Example:

```text
6
A 5
B 9
C 12
D 13
E 16
F 45
```

Here there are 6 symbols.

---

## 6. Step-by-Step Algorithm

### Step 1: Create leaf nodes

Create one node for every symbol. Store its frequency.

### Step 2: Insert all nodes into a min-heap

The node with the smallest frequency should always be available at the root of the heap.

### Step 3: Combine the two smallest nodes

Remove the two nodes with the smallest frequencies.

Create a new internal node whose frequency is:

```text
frequency = first frequency + second frequency
```

Make the two removed nodes its left and right children.

### Step 4: Insert the new node

Insert the newly created internal node back into the min-heap.

### Step 5: Repeat

Continue until the heap contains only one node.

That node is the root of the Huffman tree.

### Step 6: Find code lengths

Traverse the Huffman tree.

- Going left increases the code length by 1.
- Going right also increases the code length by 1.
- When a leaf is reached, its current depth is its Huffman code length.

### Step 7: Generate canonical codes

Sort the symbols by:

1. increasing code length
2. symbol order when lengths are equal

The first canonical code is all zeros of its length.

For every next symbol:

```text
new code = (previous code + 1)
```

and then extend it with zeros if the new code is longer.

### Step 8: Calculate expected/weighted length

The total weighted length is:

```text
sum(frequency[i] * code_length[i])
```

The average/expected length is:

```text
weighted length / total frequency
```

---

## 7. Complete C Program

```c
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
```

---

## 8. Important Code Explanation

### `Node` structure

```c
typedef struct Node {
    char symbol;
    int frequency;
    int minSymbol;
    struct Node *left;
    struct Node *right;
} Node;
```

This represents one node of the Huffman tree.

- `symbol` stores the character.
- `frequency` stores its frequency.
- `minSymbol` helps make equal-frequency heap ordering deterministic.
- `left` points to the left child.
- `right` points to the right child.

Internal nodes have `symbol = '\0'`.

---

### `MinHeap` structure

```c
typedef struct {
    Node *data[MAX * 2];
    int size;
} MinHeap;
```

The min-heap keeps the node with the smallest frequency at the top.

`size` tells us how many nodes are currently inside the heap.

---

### `createNode()`

```c
Node *createNode(char symbol, int frequency)
```

This function creates a new Huffman tree node.

It receives:

- a symbol
- its frequency

It returns a pointer to the newly created node.

---

### `insertHeap()`

This function adds a node to the min-heap.

After inserting the node at the end, it repeatedly compares the node with its parent and moves it upward if necessary.

This is called **heapify-up**.

---

### `removeMin()`

This removes the smallest-frequency node.

The root is saved, the last node is moved to the root, and then the new root is moved downward until the heap property is restored.

This is called **heapify-down**.

---

### `buildHuffmanTree()`

This is the main greedy part.

```text
while more than one node exists:
    remove smallest node
    remove second smallest node
    combine them
    insert combined node
```

Because a min-heap is used, finding the two smallest nodes is efficient.

---

### `findLengths()`

This recursively traverses the Huffman tree.

Each edge adds one to the depth.

Therefore:

```text
root -> leaf at depth 3
```

means the corresponding symbol has a Huffman code length of 3.

The actual left/right bits are not needed because the problem asks for a **canonical** codebook.

---

### `sortCodeInfo()`

Canonical Huffman codes require symbols to be ordered by:

1. code length
2. symbol

The function uses a simple sorting method because it is easy to understand for a DAA lab.

---

### `makeCanonicalCodes()`

This converts the code lengths into canonical codes.

Suppose the sorted lengths are:

```text
2, 3, 3, 3
```

The codes become:

```text
00
010
011
100
```

Notice that codes of the same length increase lexicographically.

---

### `incrementBinary()`

For example:

```text
001
```

becomes:

```text
010
```

And:

```text
011
```

becomes:

```text
100
```

It works exactly like binary addition by 1.

---

### `freeTree()`

The Huffman tree was dynamically allocated using `malloc`.

`freeTree()` releases all nodes after the result has been printed.

This prevents memory leaks.

---

## 9. Sample Input

```text
6
A 5
B 9
C 12
D 13
E 16
F 45
```

## 10. Sample Output

One valid output is:

```text
Canonical Huffman Codebook:
Symbol  Frequency  Length  Code
  F       45        1     0
  C       12        3     100
  D       13        3     101
  E       16        3     110
  A       5         4     1110
  B       9         4     1111

Total frequency = 100
Total weighted code length = 224
Average code length = 2.240 bits/symbol
```

**Note:** With equal-frequency ties, more than one optimal Huffman tree can exist. Therefore, a different but still optimal set of code lengths can be possible for some inputs. The program uses a deterministic tie-breaking rule based on the smallest symbol in a subtree.

---

## 11. Complete Dry Run

Use:

```text
A = 5
B = 9
C = 12
D = 13
E = 16
F = 45
```

### Initial min-heap

```text
5, 9, 12, 13, 16, 45
```

### Iteration 1

Remove:

```text
5 and 9
```

Combine:

```text
5 + 9 = 14
```

Heap becomes:

```text
12, 13, 14, 16, 45
```

### Iteration 2

Remove:

```text
12 and 13
```

Combine:

```text
12 + 13 = 25
```

Heap becomes:

```text
14, 16, 25, 45
```

### Iteration 3

Remove:

```text
14 and 16
```

Combine:

```text
14 + 16 = 30
```

Heap becomes:

```text
25, 30, 45
```

### Iteration 4

Remove:

```text
25 and 30
```

Combine:

```text
25 + 30 = 55
```

Heap becomes:

```text
45, 55
```

### Iteration 5

Remove:

```text
45 and 55
```

Combine:

```text
45 + 55 = 100
```

Now the heap has one node.

That node is the root.

### Tree-derived code lengths

The depths are:

| Symbol | Frequency | Code length |
|---|---:|---:|
| A | 5 | 4 |
| B | 9 | 4 |
| C | 12 | 3 |
| D | 13 | 3 |
| E | 16 | 3 |
| F | 45 | 1 |

### Canonical ordering

Sort by length and then symbol:

| Symbol | Length |
|---|---:|
| F | 1 |
| C | 3 |
| D | 3 |
| E | 3 |
| A | 4 |
| B | 4 |

Canonical codes:

| Symbol | Length | Canonical code |
|---|---:|---|
| F | 1 | 0 |
| C | 3 | 100 |
| D | 3 | 101 |
| E | 3 | 110 |
| A | 4 | 1110 |
| B | 4 | 1111 |

The exact canonical codes depend on the sorted code lengths produced by the chosen deterministic tie rule.

---

## 12. Expected Length Calculation

For the above code lengths:

```text
A: 5  × 4 = 20
B: 9  × 4 = 36
C: 12 × 3 = 36
D: 13 × 3 = 39
E: 16 × 3 = 48
F: 45 × 1 = 45
```

Total:

```text
20 + 36 + 36 + 39 + 48 + 45 = 224
```

Total frequency:

```text
5 + 9 + 12 + 13 + 16 + 45 = 100
```

Average code length:

```text
224 / 100 = 2.24 bits/symbol
```

---

## 13. Time Complexity

Let `n` be the number of symbols.

### Building the Huffman tree

There are `n - 1` combinations.

Each combination performs:

- two `removeMin()` operations
- one `insertHeap()` operation

Each heap operation takes:

```text
O(log n)
```

Therefore:

```text
O(n log n)
```

for building the tree.

### Finding code lengths

Every tree node is visited once:

```text
O(n)
```

### Sorting code lengths

The program deliberately uses simple nested loops for sorting:

```text
O(n²)
```

### Canonical code generation

The program manipulates binary strings. In the usual analysis for Huffman coding, canonicalization is linear in the number of symbols plus the total code representation. For this lab implementation, the simple string operations can add dependence on code length.

### Overall complexity of this exact implementation

Because the code uses a simple `O(n²)` sort after the `O(n log n)` Huffman construction, the overall worst-case running time of the **given C program** is:

```text
O(n²)
```

For the Huffman-tree construction itself, the standard complexity is:

```text
O(n log n)
```

If the canonicalization order were produced using an efficient `O(n log n)` sort, the overall algorithm would remain:

```text
O(n log n)
```

---

## 14. Best, Average and Worst Case

For the main Huffman construction, the number of heap operations is determined mainly by the number of symbols rather than by whether the input is already sorted.

Therefore:

- Best case: `O(n log n)` for Huffman tree construction
- Average case: `O(n log n)`
- Worst case: `O(n log n)`

For the **complete program**, the simple sorting step makes the worst-case bound `O(n²)`.

The program therefore demonstrates an important DAA point: the choice of supporting data structure and sorting method affects the final complexity.

---

## 15. Space Complexity

The Huffman tree contains approximately:

```text
2n - 1
```

nodes.

The heap also stores nodes during construction.

The code information array stores information for all `n` symbols.

Therefore the main auxiliary space is:

```text
O(n)
```

The recursion used for tree traversal also uses stack space proportional to the tree height, which is at most `O(n)`.

So the overall space complexity is:

```text
O(n)
```

---

## 16. Complexity Graph

The generated graph compares representative growth rates:

- `O(n)`
- `O(n log n)`
- `O(n²)`

The curves are **theoretical growth functions**, not measured execution times. They are included to visualize why an `O(n²)` sorting step grows faster than the standard `O(n log n)` Huffman construction.

File:

**`huffman_complexity_graph.png`**

---

## 17. Advantages

1. Huffman coding produces an optimal prefix-free code for the given frequencies.
2. It is a greedy algorithm and is relatively easy to understand.
3. Frequent symbols receive shorter codes.
4. The generated code is prefix-free.
5. Canonical Huffman codes provide a compact and deterministic way to represent the codebook once code lengths are known.
6. A min-heap makes the repeated selection of the two smallest frequencies efficient.

---

## 18. Disadvantages / Limitations

1. The Huffman tree depends on the input frequencies.
2. Equal frequencies can lead to multiple valid optimal Huffman trees.
3. The given lab implementation uses simple `O(n²)` sorting for canonical ordering.
4. Huffman coding does not necessarily compress data well when symbol frequencies are nearly uniform.
5. The implementation assumes symbols are single characters.
6. The code uses fixed-size arrays, so the maximum number of symbols is limited by `MAX`.

---

## 19. Viva Preparation

### Q1. What is Huffman coding?

Huffman coding is a greedy algorithm that creates an optimal prefix-free variable-length code based on symbol frequencies.

### Q2. Why are frequent symbols given shorter codes?

Because they occur more often, so assigning shorter codes to them reduces the total weighted code length.

### Q3. Why do we use a min-heap?

We repeatedly need the two smallest-frequency nodes. A min-heap allows us to obtain them efficiently.

### Q4. What is the greedy choice?

The greedy choice is to combine the two nodes having the smallest frequencies.

### Q5. What is a prefix-free code?

A prefix-free code is a code in which no codeword is the prefix of another codeword.

### Q6. What is the standard complexity of Huffman tree construction?

With a binary min-heap, it is `O(n log n)`.

### Q7. Why does the given complete program have an `O(n²)` bound?

Because the program uses simple nested-loop sorting to order the code lengths before canonical code generation.

### Q8. What is a canonical Huffman code?

It is a Huffman code generated from the code lengths using a standard ordering rule, rather than storing the exact tree structure.

### Q9. Why do we sort by code length first?

Canonical Huffman codes assign consecutive binary values to codes of the same length, so the lengths must be processed in increasing order.

### Q10. Why is symbol order used when code lengths are equal?

It gives a deterministic order for symbols having the same code length.

### Q11. What does `frequency * code length` represent?

It represents the total number of coded bits contributed by that symbol.

### Q12. What does the average code length mean?

It is the weighted average number of bits needed per symbol.

### Q13. Why is `freeTree()` needed?

The nodes are created using `malloc`, so they should be released using `free` after use.

### Q14. What does `findLengths()` do?

It traverses the Huffman tree and records the depth of every leaf as that symbol's code length.

### Q15. What happens if two symbols have equal frequencies?

A tie-breaking rule is used to make the result deterministic. Other tie choices can still produce another optimal Huffman tree.

---

## 20. C-Code-Specific Viva Questions

### Q1. Why is `Node *left` used?

It stores the address of the left child of a Huffman tree node.

### Q2. Why do we use `malloc()`?

The number of tree nodes is created dynamically, so `malloc()` allocates memory while the program is running.

### Q3. Why is the condition `heap.size > 1` used?

As long as more than one node exists, two nodes can be combined. When one node remains, the Huffman tree is complete.

### Q4. Why does `removeMin()` perform heapify-down?

After removing the root, the last heap element is moved to the root. Heapify-down restores the min-heap property.

### Q5. Why does `findLengths()` call itself recursively?

A Huffman tree is a binary tree. Recursion makes it simple to visit both the left and right subtrees.

---

## 21. How to Compile and Run

### GCC

Save the program as:

```text
huffman.c
```

Compile:

```bash
gcc huffman.c -o huffman
```

Run:

```bash
./huffman
```

On Windows with MinGW:

```bash
gcc huffman.c -o huffman.exe
huffman.exe
```

---

## 22. Teacher-Explanation Version

If the teacher asks, **"Explain your algorithm and code,"** you can say:

> "My program implements Huffman coding using a greedy approach. First, I take all the symbols with their frequencies and insert them into a min-heap. Then I repeatedly remove the two nodes with the smallest frequencies, combine them into a new node, and insert the new node back into the heap. When only one node remains, it becomes the root of the Huffman tree.
>
> Next, I traverse the tree to find the code length of every symbol. Instead of directly using the tree codes, I sort the symbols by code length and symbol and generate canonical Huffman codes. This makes the codebook ordered and deterministic.
>
> I also calculate the weighted code length by multiplying every symbol's frequency by its code length. The standard Huffman tree construction takes O(n log n) time because every heap operation takes O(log n). In my complete program I used simple nested-loop sorting for the canonical ordering, so the overall implementation has an O(n²) worst-case bound. The space complexity is O(n) because the tree and supporting arrays contain O(n) elements."

---

## 23. Conclusion

This program demonstrates how the greedy Huffman algorithm constructs an optimal prefix-free code from symbol frequencies.

The min-heap efficiently selects the two least frequent nodes. The resulting tree provides the optimal code lengths, and canonicalization converts those lengths into an ordered canonical codebook.

For a standard efficient Huffman implementation, the main construction takes `O(n log n)` time and `O(n)` space. The present beginner-friendly lab implementation intentionally uses simple sorting, giving the complete program an `O(n²)` worst-case bound.

This makes the program suitable for a DAA lab because it demonstrates the greedy strategy, min-heap data structure, tree traversal, canonical coding, and complexity analysis in one implementation.
