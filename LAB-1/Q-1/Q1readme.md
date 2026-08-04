# DAA Lab 01 - Question 1

## Objective
Arrange the given functions in increasing order of asymptotic growth using a C program.

## Approach
- Takes a sufficiently large value of `n` as input.
- Took logarithmic value of each function to avoid overflow.
- Store the function names and values in a structure.
- Sort the functions using Bubble Sort.
- Display the sorted order.

## Time Complexity
- Bubble Sort: **O(n²)**

## Space Complexity
- **O(n)**

## Sample Output

```
1. 1/n
2. log2(n)
3. 12*sqrt(n)
4. 50*sqrt(n)
5. n^0.51
6. 2^32*n
7. n*log2(n)
8. n^2-324
9. 100n^2+6n
10. 2n^3
11. n^(log2 n)
12. 3^n
```