# Median Without Sorting — Quickselect

## Problem Statement

Find the median of a list of N numbers without sorting the list.

## Approach

This program uses the **Quickselect algorithm**.

Quickselect is based on the partitioning technique used in Quick Sort. Instead of sorting the complete array, it partitions the array and continues searching only in the part that contains the required median position.

### For odd N

The median is the element at index:

`N / 2`

using 0-based indexing.

### For even N

The two middle elements are at:

`N / 2 - 1`

and

`N / 2`

The median is their average.

## Algorithm

1. Read N and the array elements.
2. Determine the required median position.
3. Choose the last element as the pivot.
4. Partition the array around the pivot.
5. Check the pivot's position.
6. If it is the required position, return the element.
7. If the required position is smaller, search the left part.
8. Otherwise, search the right part.
9. For even N, repeat Quickselect for both middle positions and calculate their average.

## Complexity Analysis

### Best Case

**O(N)**

If the pivot divides the remaining elements reasonably well, the amount of work is approximately:

`N + N/2 + N/4 + N/8 + ...`

This geometric series is O(N).

### Average Case

**O(N)**

On average, Quickselect eliminates a substantial part of the array after each partition, so the expected total work is linear.

### Worst Case

**O(N²)**

If the pivot is repeatedly the smallest or largest element, only one element is eliminated in each partition:

`N + (N-1) + (N-2) + ... + 1`

This results in O(N²).

### Space Complexity

The array requires **O(N)** storage.

The recursive call stack is:

- Average case: **O(log N)**
- Worst case: **O(N)**

## Why Quickselect?

A normal sorting approach would first sort the complete array, usually requiring O(N log N) time.

The problem specifically says not to sort the list. Quickselect avoids completely sorting the array and finds only the required median position.

## Compilation

Using GCC:

```bash
gcc median.c -o median
```

Run:

```bash
./median
```

On Windows:

```bash
median.exe
```

## Sample Input

```text
Enter number of elements: 5
Enter elements:
10 4 7 2 9
```

## Sample Output

```text
Median = 7.00
```

## Example

Input:

`10 4 7 2 9`

The sorted order would be:

`2 4 7 9 10`

But the program does **not** sort the array. Quickselect finds the element having the middle rank, which is `7`.

## Complexity Graph

The accompanying graph compares the growth of the best/average O(N) case with the worst O(N²) case.

The graph represents theoretical operation growth, not measured execution time.

## Files

- `median.c` — C implementation of Quickselect.
- `quickselect_complexity_graph.png` — complexity graph.
- `README.md` — project explanation and complexity analysis.
