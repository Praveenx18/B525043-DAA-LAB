# Kth Smallest Element Without Sorting

## Problem Statement
Find the kth smallest number out of a list of N numbers without sorting the list.

## Approach
This program uses the **Quickselect** algorithm.

Quickselect is based on the partitioning idea used in Quicksort, but it recursively processes only the side of the array that can contain the kth smallest element.

### Steps
1. Choose a pivot.
2. Partition the array so elements smaller than the pivot are on the left and larger elements are on the right.
3. Find the pivot's position.
4. If the pivot position is `k-1`, the pivot is the kth smallest element.
5. If `k-1` is smaller, continue in the left part.
6. Otherwise, continue in the right part.
7. Repeat until the kth smallest element is found.

## Why Quickselect?
A simple alternative is to sort the complete list and then take the kth element. However, sorting does more work than necessary. Quickselect avoids completely sorting the list and has an **average time complexity of O(N)**.

## Input
- First enter N, the number of elements.
- Enter N integers.
- Enter k, where 1 <= k <= N.

## Output
The program prints the kth smallest element.

## Example
Input:
```text
7
10 4 7 2 9 1 5
3
```

Output:
```text
3rd smallest element = 4
```

## Complexity
### Average Case
**O(N)** time.

Each partition scans the current portion of the array. On average, Quickselect eliminates a substantial part of the remaining elements after each partition.

### Best Case
**O(N)** time.

If the pivot divides the array so that the desired element is found immediately, the work is linear in the initial partition.

### Worst Case
**O(N^2)** time.

If a poor pivot is repeatedly chosen (for example, always the smallest or largest element), the problem size decreases by only one each time:
N + (N-1) + (N-2) + ... = O(N^2).

### Space
The implementation uses the array itself plus recursion stack space. The average recursion depth is O(log N), while the worst case can reach O(N).

## Graph
`kth_smallest_complexity_graph.png` is an **illustrative N log N reference curve**, included to help visualize how a comparison-based operation count grows. It is not a measurement of the exact Quickselect runtime. Quickselect's expected time complexity is O(N), while its worst case is O(N^2).

## Compilation
Using GCC:
```bash
gcc kth_smallest.c -o kth_smallest
./kth_smallest
```

## Viva Points
- Quickselect is based on partitioning.
- It does not fully sort the array.
- Average time complexity: O(N).
- Worst-case time complexity: O(N^2).
- It is useful for selection problems such as finding the kth smallest/largest element.
