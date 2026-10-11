# Minimum Cost to Connect Sticks

## 1. Problem Title

**Minimum Cost to Connect Sticks**

## 2. Problem Statement

We are given `n` sticks with lengths `L1, L2, ..., Ln`.

To connect two sticks of lengths `x` and `y`:

- The cost of the operation is `x + y`.
- The two sticks become one stick of length `x + y`.

We must connect all sticks into one stick such that the **total cost is minimum**.

### Example

For sticks:

`4 3 2 6`

One optimal sequence is:

1. Connect `2 + 3 = 5`, cost = 5
2. Connect `4 + 5 = 9`, cost = 9
3. Connect `6 + 9 = 15`, cost = 15

Total cost:

`5 + 9 + 15 = 29`

## 3. Objective

To find the minimum possible total cost of connecting all given sticks into one stick.

The problem also demonstrates the use of the **Greedy Algorithm** and a **Min Heap (Priority Queue)**.

## 4. Possible Approaches

Two simple approaches are possible:

1. Repeatedly search the entire array for the two smallest sticks.
2. Store the sticks in a **min heap** and repeatedly remove the two smallest values.

The first approach is easier to imagine but can take `O(n²)` time.

The second approach is more efficient and gives `O(n log n)` time. Therefore, this program uses a **min heap**.

## 5. Basic Idea

The greedy rule is:

> Always connect the two shortest available sticks first.

Why?

When two sticks are connected, their combined length can participate in later connections. A large stick being created early may therefore be added to the cost several times.

By combining the two smallest sticks first, we keep these repeated contributions as small as possible.

This is the same greedy principle used in **Huffman Coding**.

## 6. Algorithm

1. Read the number of sticks `n`.
2. Read all stick lengths.
3. Insert all lengths into a min heap.
4. Set `totalCost = 0`.
5. While more than one stick remains:
   - Remove the smallest stick.
   - Remove the second-smallest stick.
   - Add their lengths to get the new stick.
   - Add this connection cost to `totalCost`.
   - Insert the new stick back into the min heap.
6. Print `totalCost`.

## 7. Why Min Heap?

A min heap allows us to efficiently obtain the smallest element.

For a heap containing `n` elements:

- Insertion: `O(log n)`
- Deletion of minimum: `O(log n)`
- Getting the minimum: `O(1)`

We need two minimum values for each connection, so the heap makes the complete algorithm efficient.

## 8. C Program

```c
#include <stdio.h>

#define MAX 100

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void insertHeap(int heap[], int *size, int value)
{
    int i;

    *size = *size + 1;
    i = *size - 1;
    heap[i] = value;

    while (i > 0 && heap[(i - 1) / 2] > heap[i])
    {
        swap(&heap[(i - 1) / 2], &heap[i]);
        i = (i - 1) / 2;
    }
}

int deleteMin(int heap[], int *size)
{
    int minValue;
    int i, child, temp;

    minValue = heap[0];

    heap[0] = heap[*size - 1];
    *size = *size - 1;

    i = 0;

    while (2 * i + 1 < *size)
    {
        child = 2 * i + 1;

        if (child + 1 < *size && heap[child + 1] < heap[child])
        {
            child = child + 1;
        }

        if (heap[i] <= heap[child])
        {
            break;
        }

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
    int n, size = 0;
    int i;
    int first, second, newStick;
    int totalCost = 0;

    printf("Enter number of sticks: ");
    scanf("%d", &n);

    printf("Enter lengths of sticks:\n");

    for (i = 0; i < n; i++)
    {
        int length;
        scanf("%d", &length);
        insertHeap(heap, &size, length);
    }

    while (size > 1)
    {
        first = deleteMin(heap, &size);
        second = deleteMin(heap, &size);

        newStick = first + second;
        totalCost = totalCost + newStick;

        insertHeap(heap, &size, newStick);
    }

    printf("Minimum total cost = %d\n", totalCost);

    return 0;
}
```

## 9. Important Functions

### `swap()`

```c
void swap(int *a, int *b)
```

Swaps two integer values.

It is used while maintaining the min-heap property.

### `insertHeap()`

```c
void insertHeap(int heap[], int *size, int value)
```

Adds a new stick length to the heap.

After inserting the value at the end, it moves the value upward until the min-heap property is restored.

### `deleteMin()`

```c
int deleteMin(int heap[], int *size)
```

Removes and returns the smallest stick length.

The smallest value is always at `heap[0]`.

After removing it, the last heap element is moved to the root and moved downward until the heap property is restored.

### `main()`

The `main()` function:

- Reads the input.
- Builds the min heap.
- Repeatedly removes two smallest sticks.
- Connects them.
- Adds the cost.
- Inserts the new stick.
- Prints the final minimum cost.

## 10. Important Variables

| Variable | Meaning |
|---|---|
| `heap[]` | Stores stick lengths in min-heap form |
| `n` | Number of original sticks |
| `size` | Current number of sticks in the heap |
| `first` | Smallest available stick |
| `second` | Second-smallest available stick |
| `newStick` | Length after connecting two sticks |
| `totalCost` | Total cost accumulated so far |
| `i` | Loop/index variable |

## 11. Sample Input

```text
4
4 3 2 6
```

## 12. Sample Output

```text
Minimum total cost = 29
```

## 13. Dry Run

Initial sticks:

`4, 3, 2, 6`

After inserting them into the min heap, the smallest values can be removed efficiently.

| Step | First | Second | New Stick | Cost Added | Total Cost |
|---|---:|---:|---:|---:|---:|
| Initial | - | - | - | - | 0 |
| 1 | 2 | 3 | 5 | 5 | 5 |
| 2 | 4 | 5 | 9 | 9 | 14 |
| 3 | 6 | 9 | 15 | 15 | 29 |

At the end only one stick of length `15` remains.

Therefore:

`Minimum Cost = 29`

## 14. Why Connecting the Two Smallest Works

Suppose we connect a very large stick too early.

The resulting large stick may be used again in another connection, causing that large value to be added repeatedly to the total cost.

Connecting smaller sticks first keeps the intermediate values small.

The greedy choice of the two smallest sticks therefore produces the minimum total cost.

## 15. Complexity Analysis

Let `n` be the number of sticks.

### Building the Heap

In this implementation, the sticks are inserted one at a time.

Each insertion takes at most `O(log n)`.

Therefore:

`O(n log n)`

### Main Loop

There are `n - 1` connections.

For every connection:

- Delete minimum: `O(log n)`
- Delete second minimum: `O(log n)`
- Insert new stick: `O(log n)`

So each iteration takes `O(log n)`.

There are `n - 1` iterations.

Therefore:

`O(n log n)`

### Total Time Complexity

`O(n log n) + O(n log n) = O(n log n)`

So:

**Best Case: `O(n log n)`**

**Average Case: `O(n log n)`**

**Worst Case: `O(n log n)`**

For this algorithm, the asymptotic complexity does not change based on the order of the input because every input is inserted into the heap and the same sequence of heap operations is required.

### Space Complexity

The heap can contain up to `n` sticks.

Therefore:

**Space Complexity = `O(n)`**

## 16. Complexity Graph

The generated graph compares the theoretical growth of:

- `O(n)`
- `O(n log n)`
- `O(n²)`

The `O(n log n)` curve is the important one for this implementation.

The curves represent mathematical growth rates, not measured execution times.

File:

`complexity_graph_connect_sticks.png`

## 17. Advantages

1. Gives the minimum possible connection cost.
2. Uses a clear greedy strategy.
3. Min heap makes finding the two smallest sticks efficient.
4. Time complexity is `O(n log n)`.
5. Suitable for large inputs compared with the repeated linear-search approach.
6. The logic is closely related to Huffman Coding.

## 18. Disadvantages / Limitations

1. A min heap implementation requires extra memory.
2. Heap operations are slightly more difficult to understand than simply sorting an array.
3. The program uses a fixed maximum size of 100 sticks because of the `MAX` array size.
4. The program uses `int`, so extremely large stick lengths could cause integer overflow.

## 19. Difference from Brute Force / Simple Approach

A simple approach could repeatedly scan the whole array to find the two smallest sticks.

For each of approximately `n` connections, scanning can take `O(n)` time.

Therefore, that approach can take:

`O(n²)`

The min-heap approach takes:

`O(n log n)`

So the heap approach is more efficient for larger inputs.

## 20. Viva Preparation

### Q1. What is the main idea of the algorithm?

Always connect the two shortest available sticks first using a min heap.

### Q2. Which algorithmic technique is used?

A **Greedy Algorithm** is used.

### Q3. Why is it called greedy?

At every step, we make the locally best choice: connect the two smallest sticks.

### Q4. Why do we use a min heap?

A min heap allows us to efficiently obtain the two smallest stick lengths.

### Q5. What is the time complexity?

The time complexity is `O(n log n)`.

### Q6. What is the space complexity?

The space complexity is `O(n)` because the heap stores the sticks.

### Q7. Why does the algorithm run `n - 1` times?

Every connection reduces the number of sticks by one. Starting with `n` sticks, we need `n - 1` connections to get one final stick.

### Q8. What is the cost of connecting two sticks?

If the lengths are `x` and `y`, the cost is `x + y`.

### Q9. Why do we insert the new stick back into the heap?

Because the newly created stick can participate in another future connection.

### Q10. Where is the minimum element in a min heap?

At index `0` in this array implementation.

### Q11. What is the difference between a min heap and a max heap?

A min heap keeps the smallest value at the root, while a max heap keeps the largest value at the root.

### Q12. Why not simply sort the sticks once?

After every connection, a new stick is created and must be considered again. A min heap efficiently supports repeated removal and insertion of minimum values.

### Q13. What happens when there is only one stick?

No connection is needed, so the cost is `0`.

### Q14. What does `size` represent?

It represents the current number of sticks stored in the heap.

### Q15. Why is `heap[0]` used in `deleteMin()`?

Because the root of a min heap contains the smallest element.

### C-Code-Specific Viva Questions

**Q16. Why is `*size` used instead of just `size` in `insertHeap()`?**

Because the function must change the actual heap size in `main()`. A pointer allows the function to modify the original variable.

**Q17. Why is `2 * i + 1` used?**

For an array-based binary heap, the left child of index `i` is at `2*i + 1`.

**Q18. Why is `(i - 1) / 2` used?**

It gives the parent index of a node in an array-based heap.

**Q19. Why do we use `while (size > 1)`?**

As long as at least two sticks exist, another connection can be performed.

**Q20. Why is `totalCost` initialized to zero?**

No connection has been performed at the beginning, so the accumulated cost is initially zero.

## 21. How to Compile and Run

Using GCC:

```bash
gcc connect_sticks.c -o connect_sticks
```

Run:

```bash
./connect_sticks
```

On Windows:

```bash
gcc connect_sticks.c -o connect_sticks.exe
connect_sticks.exe
```

## 22. Short Conclusion

The Minimum Cost to Connect Sticks problem can be solved efficiently using a greedy strategy with a min heap. At every step, the two smallest sticks are connected, and the resulting stick is inserted back into the heap. This gives a time complexity of `O(n log n)` and space complexity of `O(n)`. The approach is efficient, simple enough for a DAA lab, and demonstrates both greedy selection and priority-queue concepts.
