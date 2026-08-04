# Dictionary Operations – Time Complexity Validation

This project implements and analyzes the performance of Dictionary Abstract Data Type (ADT) operations using different data structures. The objective is to validate the theoretical asymptotic time complexities by experimentally measuring execution time for increasing input sizes.

## Problem Statement

A Dictionary ADT supports the following operations:

- Search(D, k)
- Insert(D, x)
- Delete(D, x)
- Maximum(D)
- Minimum(D)
- Predecessor(D, x)
- Successor(D, x)

The aim is to study the worst-case running time of these operations and compare practical execution times with theoretical complexities.

## Implemented Data Structures

This project currently implements and benchmarks:

- Unsorted Array
- Sorted Singly Linked List

## Operations Tested

The following dictionary operations are measured:

- Search
- Insert
- Delete
- Maximum
- Minimum
- Successor
- Predecessor

Each operation is executed repeatedly for different input sizes to obtain average execution time.

## Input Sizes

The program performs experiments for the following values of **n**:

```
100
1000
2000
5000
8000
10000
15000
20000
```

## Methodology

For each input size:

1. Build a fresh data structure.
2. Execute one dictionary operation multiple times.
3. Compute the average execution time per operation.
4. Store the timing results in CSV format.
5. Plot the order of growth using the generated data.

This approach minimizes timer noise and produces more reliable measurements.

## Time Complexity Summary

| Operation | Unsorted Array | Sorted Singly Linked List |
|-----------|---------------|----------------------------|
| Search | O(n) | O(n) |
| Insert | O(1) | O(n) |
| Delete | O(n) | O(n) |
| Maximum | O(n) | O(n) |
| Minimum | O(n) | O(1) |
| Successor | O(n) | O(1) (given node) |
| Predecessor | O(n) | O(n) |

## Compilation

```bash
gcc -O2 -o dict_simple_timing dict_simple_timing.c
```

## Execution

```bash
./dict_simple_timing > timing_simple.csv
```

The program generates a CSV file containing:

- Data Structure
- Operation
- Input Size (n)
- Average Execution Time

This CSV can be imported into Excel, Google Sheets, Python (Matplotlib), or any plotting tool to visualize the order of growth.

## Learning Outcomes

- Understand Dictionary ADT operations.
- Compare practical performance with theoretical time complexities.
- Observe how execution time grows with increasing input size.
- Analyze the impact of different data structures on dictionary operations.

## Author

Developed as part of the **Design and Analysis of Algorithms (DAA) Laboratory** to experimentally validate asymptotic time complexity through performance analysis.