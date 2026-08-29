# Heap Sort DAA Lab Assignment

## Problem Statement
Implement Heap Sort of `n` randomly generated elements and store the generated elements in a file. Perform complexity analysis and plot a graph showing running time against input size.

## Algorithm Used
Heap Sort using a Max Heap.

## Basic Idea
1. Generate `n` random integers.
2. Store the generated integers in `input.txt`.
3. Build a max heap from the array.
4. Swap the root (largest element) with the last unsorted element.
5. Reduce the heap size and heapify the root again.
6. Repeat until the array is sorted.

## Files
- `heap_sort.c` - C implementation.
- `input.txt` - generated input values when the program is executed.
- `complexity_data.csv` - sample timing data used for the graph.
- `heap_sort_complexity.png` - graph of input size vs execution time.

## Compile and Run
```bash
gcc heap_sort.c -o heap_sort
./heap_sort
```

On Windows:
```text
gcc heap_sort.c -o heap_sort.exe
heap_sort.exe
```

## Complexity Analysis
- Best case: O(n log n)
- Average case: O(n log n)
- Worst case: O(n log n)
- Auxiliary space: O(log n) for recursive `heapify` calls in the worst case.
- The array itself uses O(n) memory.

Building the heap takes O(n). The sorting phase performs n-1 extractions, and each extraction calls heapify in O(log n), giving O(n log n). Therefore total time is O(n log n) in all cases.

## Graph
The graph is generated from measured execution times for increasing input sizes. Because very small input sizes can have noisy timings due to OS scheduling and timer resolution, the graph should be interpreted as an empirical demonstration of the expected O(n log n) growth, not as a proof of the theoretical complexity.

## Viva Points
- Heap Sort is a comparison-based sorting algorithm.
- It uses a binary heap.
- A max heap is used to produce ascending order.
- Heap Sort has O(n log n) worst-case time complexity.
- Heap Sort is in-place apart from recursion-stack space.
