# Minimise Deviation in Array — Two-Way Greedy with Max-Heap

## 1. Problem Title

**Minimise Deviation in Array (Two-Way Greedy with Max-Heap)**

## 2. Problem Statement

Given an array of positive integers, two operations are allowed:

1. If an element is odd, multiply it by 2.
2. If an element is even, divide it by 2.

The operations can be performed any number of times, subject to these rules.

The **deviation** of the array is:

```text
maximum element - minimum element
```

The objective is to perform the operations in a suitable order so that the final deviation is as small as possible.

---

## 3. Objective

Find the minimum possible deviation of the array after applying the allowed operations.

The implementation uses a **greedy approach with a max-heap**.

---

## 4. Basic Idea

For every number, its largest useful value is obtained by making an odd number even:

```text
odd x -> 2*x
```

After this step, every number is even.

Now consider the current maximum element.

- If it is even, divide it by 2.
- This decreases the maximum and can therefore reduce the deviation.
- We continue while the current maximum is even.
- If the maximum becomes odd, it cannot be divided anymore, so we stop.

A **max-heap** is used so that we can quickly find the current maximum element.

We also maintain the current minimum value separately.

---

## 5. Why This Greedy Approach Works

Suppose all odd numbers have first been doubled.

Now the largest element is the only element that can be reduced in a way that may decrease the current range.

If the current maximum is even, replacing it by half can only move that maximum downward. We choose the largest element because reducing a smaller element does not reduce the maximum and can make the range no better.

The process stops when the maximum is odd because an odd maximum cannot be divided by 2 under the allowed operation.

We keep the best deviation seen during the process.

---

## 6. Step-by-Step Algorithm

1. Read `n` and the array.
2. Set `minimum` to a very large value.
3. For every element:
   - If it is odd, multiply it by 2.
   - Update `minimum`.
   - Insert the resulting value into a max-heap.
4. Set `answer = maximum - minimum`.
5. Repeat:
   - Remove the current maximum from the max-heap.
   - If the maximum is odd, stop.
   - Divide the maximum by 2.
   - Update `minimum` if necessary.
   - Insert the new value back into the max-heap.
   - Calculate the new deviation.
   - Update `answer` if the new deviation is smaller.
6. Print `answer`.

---

## 7. C Program

```c
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

/* Find minimum value in the array */
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

        /* Odd numbers are first doubled */
        if (arr[i] % 2 != 0)
            arr[i] = arr[i] * 2;

        insertHeap(heap, &heapSize, arr[i]);
    }

    minimum = findMin(heap, heapSize);

    maximum = heap[0];
    answer = maximum - minimum;

    /*
       Repeatedly reduce the current maximum
       as long as it is even.
    */
    while (1)
    {
        maximum = removeMax(heap, &heapSize);

        /* An odd maximum cannot be reduced */
        if (maximum % 2 != 0)
            break;

        maximum = maximum / 2;

        if (maximum < minimum)
            minimum = maximum;

        insertHeap(heap, &heapSize, maximum);

        deviation = heap[0] - minimum;

        if (deviation < answer)
            answer = deviation;
    }

    printf("Minimum possible deviation = %d\n", answer);

    return 0;
}
```

---

## 8. Important Code Explanation

### Header file

```c
#include <stdio.h>
```

This provides `printf()` and `scanf()` for input and output.

### Maximum array size

```c
#define MAX 100
```

The program allows up to 100 elements in this simple lab implementation.

### `swap()` function

```c
void swap(int *a, int *b)
```

It exchanges two integer values.

It is used while maintaining the max-heap.

Parameters:

- `a` — address of first integer.
- `b` — address of second integer.

It does not return a value.

---

### `insertHeap()`

```c
void insertHeap(int heap[], int *size, int value)
```

This inserts a new value into the max-heap.

Important variables:

- `heap[]` — stores heap elements.
- `size` — current number of heap elements.
- `value` — new element to insert.
- `i` — current position of the new element.
- `parent` — parent position.

After inserting the value at the end, the function compares it with its parent and moves it upward until the max-heap property is restored.

For a max-heap:

```text
parent >= child
```

---

### `removeMax()`

```c
int removeMax(int heap[], int *size)
```

This removes the largest element.

In a max-heap, the largest element is always at:

```c
heap[0]
```

The last element is moved to the root, and then it is moved downward until the heap property is restored.

The function returns the removed maximum value.

---

### `findMin()`

```c
int findMin(int heap[], int size)
```

The minimum value is not directly available at the root of a max-heap.

Therefore, this function scans the heap and finds the smallest value.

It is called after the initial heap is built.

---

### Main function

The array is read first.

For every element:

```c
if (arr[i] % 2 != 0)
    arr[i] = arr[i] * 2;
```

If the number is odd, it is doubled.

For example:

```text
3 -> 6
5 -> 10
7 -> 14
```

This creates the largest useful starting representation.

Then every value is inserted into the max-heap.

---

### Initial minimum

```c
minimum = findMin(heap, heapSize);
```

We store the current smallest element because deviation depends on both maximum and minimum.

---

### Initial answer

```c
maximum = heap[0];
answer = maximum - minimum;
```

Since `heap[0]` is the largest element:

```text
deviation = maximum - minimum
```

The initial deviation is stored in `answer`.

---

### Main greedy loop

```c
maximum = removeMax(heap, &heapSize);
```

Remove the current largest element.

Then:

```c
if (maximum % 2 != 0)
    break;
```

If it is odd, it cannot be divided by 2, so no further useful reduction of the maximum is possible.

Otherwise:

```c
maximum = maximum / 2;
```

The maximum is reduced.

If the new value becomes smaller than the current minimum:

```c
if (maximum < minimum)
    minimum = maximum;
```

The minimum is updated.

Then the new value is inserted back into the heap.

Finally:

```c
deviation = heap[0] - minimum;
```

The new deviation is calculated and compared with the best answer.

---

# 9. Sample Input

```text
Enter number of elements: 4
Enter the elements:
1 2 3 4
```

# 10. Sample Output

```text
Minimum possible deviation = 1
```

---

# 11. Complete Dry Run

Consider:

```text
A = [1, 2, 3, 4]
```

### Step 1: Make odd numbers even

```text
1 -> 2
2 -> 2
3 -> 6
4 -> 4
```

So the initial values are:

```text
[2, 2, 6, 4]
```

Current minimum:

```text
minimum = 2
```

Current maximum:

```text
maximum = 6
```

Initial deviation:

```text
6 - 2 = 4
```

So:

```text
answer = 4
```

### Step 2: Reduce maximum

Current maximum = `6`.

Since `6` is even:

```text
6 / 2 = 3
```

Array becomes conceptually:

```text
[2, 2, 3, 4]
```

Minimum remains:

```text
2
```

New maximum:

```text
4
```

New deviation:

```text
4 - 2 = 2
```

Therefore:

```text
answer = 2
```

### Step 3: Reduce maximum again

Current maximum = `4`.

It is even:

```text
4 / 2 = 2
```

Array becomes:

```text
[2, 2, 2, 3]
```

Minimum:

```text
2
```

Maximum:

```text
3
```

Deviation:

```text
3 - 2 = 1
```

Therefore:

```text
answer = 1
```

### Step 4: Check maximum

Current maximum = `3`.

`3` is odd, so it cannot be divided.

The algorithm stops.

Final answer:

```text
1
```

### Dry Run Table

| Step | Current Maximum | Operation | New Maximum | Minimum | Deviation | Best Answer |
|---|---:|---|---:|---:|---:|---:|
| Initial | 6 | None | 6 | 2 | 4 | 4 |
| 1 | 6 | 6 / 2 | 4 | 2 | 2 | 2 |
| 2 | 4 | 4 / 2 | 3 | 2 | 1 | 1 |
| 3 | 3 | Stop, odd | 3 | 2 | 1 | 1 |

---

# 12. Complexity Analysis

Let:

- `n` = number of elements.
- `M` = maximum value after the initial doubling of odd numbers.

### Building the heap

Each element is inserted into the heap.

One insertion takes:

```text
O(log n)
```

Using repeated insertion gives:

```text
O(n log n)
```

For this simple implementation.

### Number of reductions

An even number can be divided by 2 only a limited number of times.

For a value at most `M`, this is approximately:

```text
O(log M)
```

times.

Across `n` elements, there can be at most approximately:

```text
O(n log M)
```

heap updates.

### Cost of each heap operation

Each remove/insert operation costs:

```text
O(log n)
```

Therefore, the dominant worst-case complexity is:

```text
O(n log M log n)
```

### Best Case

If the largest element is already odd after normalization, the reduction loop stops immediately.

The simple implementation still builds the heap, so the practical best-case bound is:

```text
O(n log n)
```

### Average Case

The exact average depends on the distribution of the input values.

A reasonable bound for the heap-based process is:

```text
O(n log M log n)
```

in the general case.

### Space Complexity

The heap stores `n` elements:

```text
O(n)
```

The input array also stores `n` elements, so total auxiliary/storage space is:

```text
O(n)
```

---

# 13. Complexity Graph

The generated graph is based on theoretical complexity curves, not experimental measurements.

It compares:

- `O(n)`
- `O(n log n)`
- `O(n log M log n)` for an illustrative fixed scale of `M = 2^10`

The graph uses:

- X-axis: **Input Size (n)**
- Y-axis: **Number of Operations / Time**
- White background
- Clearly labelled curves

File:

`complexity_graph_minimise_deviation.png`

---

# 14. Advantages

1. The greedy idea is simple to understand.
2. A max-heap gives fast access to the current maximum.
3. It avoids trying every possible sequence of operations.
4. It works well for large arrays compared with brute-force enumeration.
5. The approach is suitable for explaining greedy algorithms in a DAA viva.

---

# 15. Disadvantages / Limitations

1. A heap makes the implementation more complicated than a simple array-only approach.
2. The simple C program uses a fixed maximum array size of 100.
3. Very large values may cause integer overflow if normal `int` is used.
4. Repeated heap operations add a `log n` factor.
5. The average running time depends on the values and their divisibility by 2.

---

# 16. Comparison with Brute Force

A brute-force method could try many possible combinations of doubling and halving operations.

The number of possible operation sequences can become very large.

The greedy method avoids exploring all these possibilities.

Instead, it:

1. Normalizes odd values.
2. Always focuses on the current maximum.
3. Reduces it when possible.
4. Keeps the best deviation found.

This makes the approach much more efficient and practical.

---

# 17. Viva Preparation

### Q1. What is the main idea of the algorithm?

**Answer:** First, every odd number is doubled so that all numbers are in their largest useful form. Then we repeatedly divide the current maximum by 2 while it is even.

### Q2. Why do we use a max-heap?

**Answer:** A max-heap lets us quickly get the largest element, which is the element we want to reduce.

### Q3. Why are odd numbers doubled initially?

**Answer:** An odd number cannot be divided by 2, but it can be doubled. Doubling it gives the largest useful representation from which future halving operations can be performed.

### Q4. When does the algorithm stop?

**Answer:** It stops when the current maximum becomes odd because an odd number cannot be divided by 2.

### Q5. What is deviation?

**Answer:** Deviation is the difference between the maximum and minimum array values.

```text
Deviation = Maximum - Minimum
```

### Q6. What is the time complexity?

**Answer:** For the simple heap implementation, the general worst-case bound is approximately `O(n log M log n)`, where `M` is the maximum normalized value.

### Q7. Why is there a `log n` factor?

**Answer:** Each insertion and deletion from the max-heap takes `O(log n)` time.

### Q8. Why is there a `log M` factor?

**Answer:** A value can be divided by 2 only about `log2(M)` times before it becomes small or odd.

### Q9. What is the space complexity?

**Answer:** `O(n)` because the heap and array store a number of elements proportional to `n`.

### Q10. Why do we keep the minimum separately?

**Answer:** The heap efficiently gives the maximum, but a max-heap does not directly give the minimum. Therefore, we maintain the minimum separately.

### Q11. Why don't we reduce the minimum?

**Answer:** Reducing the minimum would generally increase the deviation. The useful greedy operation is to reduce the current maximum.

### Q12. What happens if the maximum is odd?

**Answer:** It cannot be divided by 2, so the reduction process stops.

### Q13. What is the purpose of `removeMax()`?

**Answer:** It removes the largest element from the max-heap and restores the heap property.

### Q14. Why is `heap[0]` the maximum?

**Answer:** In a max-heap, every parent is greater than or equal to its children, so the largest element is stored at the root, index 0.

### Q15. What does `arr[i] % 2 != 0` check?

**Answer:** It checks whether `arr[i]` is odd.

---

## C-Code-Specific Viva Questions

### Q16. Why do we use pointers in `swap()`?

**Answer:** We need to change the original variables, not just their copies. Pointers allow the function to modify the original values.

### Q17. Why is `size` passed as a pointer to `insertHeap()`?

**Answer:** The function needs to increase the actual heap size, so it receives the address of `size`.

### Q18. Why is `while (1)` used?

**Answer:** It creates a loop that continues until a condition explicitly calls `break`. Here, we stop when the maximum becomes odd.

### Q19. Why do we calculate `parent = (i - 1) / 2`?

**Answer:** This is the index formula for finding the parent of a node in an array-based binary heap.

### Q20. Why are `left = 2*i+1` and `right = 2*i+2` used?

**Answer:** These are the standard array index formulas for the left and right children of a binary heap node.

---

# 18. Teacher-Explanation Version

If the teacher asks, **"Explain your algorithm and code"**, you can say:

> "My problem is to minimize the difference between the maximum and minimum elements of an array. We can multiply an odd element by 2 and divide an even element by 2.
>
> First, I convert every odd element into an even element by multiplying it by 2. This gives every element its largest useful value. Then I use a max-heap to always get the current maximum element.
>
> If the maximum is even, I divide it by 2 because reducing the maximum can reduce the deviation. After every division, I update the minimum if required and calculate the new deviation. I keep the smallest deviation found so far.
>
> When the maximum becomes odd, I stop because it cannot be divided anymore.
>
> In the code, `insertHeap()` maintains the max-heap after insertion, and `removeMax()` removes the largest element and restores the heap property. The minimum is maintained separately because a max-heap gives us the maximum efficiently but not the minimum.
>
> The heap operations take `O(log n)` time, and an element can be divided by 2 about `O(log M)` times. Therefore, the general worst-case complexity is approximately `O(n log M log n)`, and the space complexity is `O(n)`."

---

# 19. Compilation and Execution

Save the program as:

```text
minimise_deviation.c
```

Compile using GCC:

```bash
gcc minimise_deviation.c -o minimise_deviation
```

Run:

### Windows

```bash
minimise_deviation.exe
```

### Linux / macOS

```bash
./minimise_deviation
```

---

# 20. Conclusion

The problem can be solved efficiently using a greedy strategy and a max-heap. First, odd elements are doubled to put them into their largest useful form. Then the current maximum is repeatedly halved whenever possible. The minimum value is maintained separately and the smallest deviation encountered is stored as the answer.

The max-heap makes finding and removing the current maximum efficient, making this approach suitable for a DAA lab implementation and viva explanation.
