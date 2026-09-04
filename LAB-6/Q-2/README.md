# 2D Square Matrix Operations and Their Complexities

## DAA Lab Assignment

### Problem Statement

Given square matrices with `n` rows and `n` columns, determine the
worst-case computational complexity of:

1.  Matrix Addition
2.  Matrix Multiplication
3.  Checking whether a matrix is a Zero Matrix
4.  Checking whether a matrix is a Symmetric Matrix
5.  Computing the Determinant
6.  Transposing the matrix in situ (in-place)
7.  Finding the Eigenvalue and Eigenvector

A C program is used to implement these operations and validate the
complexity analysis.

## Complexity Summary

  ----------------------------------------------------------------------------
  Operation             Best Case   Average Case     Worst Case    Extra Space
  ---------------- -------------- -------------- -------------- --------------
  Matrix Addition           O(n²)          O(n²)      **O(n²)**          O(n²)

  Matrix                    O(n³)          O(n³)      **O(n³)**          O(n²)
  Multiplication                                                

  Zero Matrix                O(1)          O(n²)      **O(n²)**           O(1)
  Check                                                         

  Symmetric Matrix           O(1)          O(n²)      **O(n²)**           O(1)
  Check                                                         

  Determinant -             O(n!)          O(n!)      **O(n!)**        O(n²) +
  Recursive                                                          recursion
  Cofactor                                                      

  In-place                  O(n²)          O(n²)      **O(n²)**       **O(1)**
  Transpose                                                     

  Power Method             O(kn²)         O(kn²)         O(kn²)           O(n)
  ----------------------------------------------------------------------------

## Algorithms

### 1. Matrix Addition

For every element:

`C[i][j] = A[i][j] + B[i][j]`

There are `n²` elements, so the complexity is **O(n²)**.

### 2. Matrix Multiplication

For every result element, a row of A is multiplied with a column of B.
The standard implementation uses three nested loops.

`C[i][j] = C[i][j] + A[i][k] * B[k][j]`

Therefore the complexity is **O(n³)**.

### 3. Zero Matrix

The program checks every element. It can stop immediately after finding
a non-zero element.

-   Best case: **O(1)**
-   Worst case: **O(n²)**

### 4. Symmetric Matrix

A matrix is symmetric when:

`A[i][j] = A[j][i]`

Only one half of the matrix is checked. The number of comparisons is
`n(n-1)/2`, giving **O(n²)**.

### 5. Determinant

The program uses recursive cofactor expansion. Each recursive call
creates determinants of smaller matrices.

Approximate recurrence:

`T(n) = nT(n-1) + O(n²)`

Therefore the complexity is **O(n!)**.

**Note:** Determinant can be computed in approximately O(n³) using
Gaussian elimination or LU decomposition. The O(n!) complexity applies
to the recursive cofactor method used here.

### 6. In-place Transpose

Elements above the main diagonal are swapped with their corresponding
elements below it:

`A[i][j] <-> A[j][i]`

There are `n(n-1)/2` swaps, so time complexity is **O(n²)** and extra
space is **O(1)**.

### 7. Eigenvalue and Eigenvector

The program uses the **Power Method** to find the dominant eigenvalue
and its corresponding eigenvector.

Each matrix-vector multiplication requires O(n²) time. For `k`
iterations:

**O(kn²)**

For general dense-matrix eigenvalue computation using standard numerical
methods, the typical complexity is around O(n³).

## Operation Counts for the Graph

  ----------------------------------------------------------------------------
            n    Addition   Multiplication Zero Matrix   Symmetric    In-place
                                                                     Transpose
  ----------- ----------- ---------------- ----------- ----------- -----------
            1           1                1           1           0           0

            2           4                8           4           1           1

            3           9               27           9           3           3

            4          16               64          16           6           6

            5          25              125          25          10          10

            6          36              216          36          15          15

            7          49              343          49          21          21

            8          64              512          64          28          28

            9          81              729          81          36          36

           10         100             1000         100          45          45
  ----------------------------------------------------------------------------

### Formulas

-   Addition = `n²`
-   Multiplication = `n³`
-   Zero Matrix, worst case = `n²`
-   Symmetric Matrix = `n(n-1)/2`
-   In-place Transpose = `n(n-1)/2`

The graph demonstrates quadratic growth for addition, zero checking,
symmetry checking, and transpose, while multiplication grows cubically.

## Main C Functions

  -----------------------------------------------------------------------
  Function                            Purpose
  ----------------------------------- -----------------------------------
  `addMatrices()`                     Adds two matrices

  `multiplyMatrices()`                Multiplies two matrices

  `isZeroMatrix()`                    Checks whether all elements are
                                      zero

  `isSymmetric()`                     Checks whether the matrix is
                                      symmetric

  `getCofactor()`                     Creates a smaller matrix for
                                      determinant calculation

  `determinant()`                     Recursively calculates determinant

  `transpose()`                       Transposes the matrix in-place

  `powerMethod()`                     Finds the dominant eigenvalue and
                                      eigenvector

  `displayMatrix()`                   Displays a matrix
  -----------------------------------------------------------------------

## Sample Input

``` text
Enter order of square matrix: 2

Enter elements of Matrix A:
1 2
3 4

Enter elements of Matrix B:
5 6
7 8
```

## Sample Output

``` text
Matrix Addition:
6 8
10 12

Matrix Multiplication:
19 22
43 50

Matrix A is NOT a Zero Matrix.
Matrix A is NOT Symmetric.
Determinant of Matrix A = -2

Transpose of Matrix A:
1 3
2 4
```

The eigenvalue/eigenvector output is a numerical approximation produced
by the Power Method.

## Compilation

Using GCC:

``` bash
gcc matrix_operations.c -o matrix_operations -lm
```

The `-lm` option links the math library required by `fabs()`.

## Execution

### Windows

``` bash
matrix_operations.exe
```

### Linux / macOS

``` bash
./matrix_operations
```

## Experimental Procedure

1.  Run the algorithms for different values of `n`.
2.  Count the important basic operations.
3.  Record the operation count for each value of `n`.
4.  Plot `n` on the X-axis.
5.  Plot the number of operations on the Y-axis.
6.  Compare the graph with the theoretical complexity.

## Conclusion

The experiment demonstrates that:

-   Matrix Addition has **O(n²)** worst-case time.
-   Matrix Multiplication has **O(n³)** worst-case time.
-   Zero Matrix checking has **O(n²)** worst-case time.
-   Symmetric Matrix checking has **O(n²)** worst-case time.
-   Recursive cofactor determinant calculation has **O(n!)** time.
-   In-place Transpose has **O(n²)** time and **O(1)** extra space.
-   The Power Method takes **O(kn²)** for `k` iterations.

This experiment shows how the choice of algorithm affects execution time
as the matrix size increases.
