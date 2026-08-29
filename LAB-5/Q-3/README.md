# Quick Sort of N Random Elements Stored in a File

## Aim
Implement Quick Sort for `N` random elements stored in a file, analyze its complexity, and plot a graph showing how execution time changes with input size.

## Algorithm
1. Read the numbers from the input file.
2. Choose the last element as the pivot.
3. Partition the array so elements smaller than or equal to the pivot are placed before it and larger elements after it.
4. Recursively apply Quick Sort to the left and right parts.
5. Write/display the sorted result.

## Complexity
- Best case: **O(N log N)** — the pivot divides the array into two nearly equal parts at each level.
- Average case: **O(N log N)** — random/unbalanced partitions are balanced on average.
- Worst case: **O(N²)** — the pivot repeatedly becomes the smallest or largest element.
- Auxiliary space: **O(log N)** on average for recursion; **O(N)** in the worst case due to recursion depth.

## File Handling
The lab program should read the numbers from a text file. For large input sizes, it is better to store the count followed by the elements, or read until end-of-file.

## Graph
`quick_sort_complexity_graph.png` shows measured average execution time for:
100, 500, 1000, 2000, 5000, 10000 elements.

## Experimental Notes
The graph is an empirical performance graph, so exact timings depend on the computer, compiler, operating system, and background processes. It should be used to observe the trend rather than to prove the theoretical complexity.

## Viva Points
- Quick Sort follows the Divide and Conquer technique.
- Partitioning places the pivot in its final position.
- The pivot choice strongly affects performance.
- Average-case complexity is O(N log N), while the worst case is O(N²).
