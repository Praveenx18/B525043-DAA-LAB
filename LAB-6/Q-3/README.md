# Convolution of Two Vectors Using FFT

## Problem Statement

Given two vectors `A` and `B` of lengths `m` and `n`, where `n >= m`,
find their convolution:

C\[k\] = sum A\[j\] \* B\[k-j\]

The goal is to design and implement an **O(n log n) divide-and-conquer
algorithm** for convolution.

------------------------------------------------------------------------

## Objective

To calculate the convolution of two vectors efficiently using the **Fast
Fourier Transform (FFT)** and verify the result using a C program.

------------------------------------------------------------------------

## Input

-   Size of vector `A`: `m`
-   Size of vector `B`: `n`
-   Elements of vector `A`
-   Elements of vector `B`

The problem assumes:

``` text
n >= m
```

------------------------------------------------------------------------

## Output

The convolution vector `C` containing:

``` text
m + n - 1
```

elements.

------------------------------------------------------------------------

## Example

### Input

``` text
Enter size of vector A: 3
Enter size of vector B: 4
Enter elements of vector A:
1 2 3
Enter elements of vector B:
4 5 6 7
```

### Output

``` text
Convolution of A and B:
4.00 13.00 28.00 34.00 32.00 21.00
```

------------------------------------------------------------------------

## Algorithm Used

The algorithm uses **Fast Fourier Transform (FFT)**, which is a
divide-and-conquer technique.

### Main Steps

1.  Read vectors `A` and `B`.

2.  Calculate the required convolution size:

    ``` text
    m + n - 1
    ```

3.  Find the smallest power of 2 greater than or equal to this size.

4.  Add zeroes to both vectors until they have this size.

5.  Apply FFT to vector `A`.

6.  Apply FFT to vector `B`.

7.  Multiply the corresponding FFT values point-by-point.

8.  Apply inverse FFT to the multiplied result.

9.  Divide the resulting values by the FFT size.

10. Print the first `m + n - 1` values as the convolution.

------------------------------------------------------------------------

## Why FFT?

The direct convolution method uses nested loops and requires:

``` text
O(mn)
```

operations.

Since `n >= m`, this can become approximately:

``` text
O(n²)
```

FFT reduces the complexity to:

``` text
O(n log n)
```

which is much more efficient for large vectors.

------------------------------------------------------------------------

## Divide-and-Conquer Approach

FFT divides the input into:

-   Even-indexed elements
-   Odd-indexed elements

For example:

``` text
A = [a0, a1, a2, a3, a4, a5, a6, a7]

Even = [a0, a2, a4, a6]
Odd  = [a1, a3, a5, a7]
```

FFT is recursively applied to both parts and then the results are
combined using complex roots of unity.

The recurrence is:

``` text
T(n) = 2T(n/2) + O(n)
```

Therefore:

``` text
T(n) = O(n log n)
```

------------------------------------------------------------------------

## Input Representation

FFT works conveniently when the input size is a power of 2.

The required result contains:

``` text
m + n - 1
```

elements.

Therefore, we choose:

``` text
N = smallest power of 2 >= m + n - 1
```

The vectors are zero-padded to size `N`.

For example:

``` text
A = [1, 2, 3]
B = [4, 5, 6, 7]

m + n - 1 = 6

N = 8
```

After padding:

``` text
A = [1, 2, 3, 0, 0, 0, 0, 0]
B = [4, 5, 6, 7, 0, 0, 0, 0]
```

------------------------------------------------------------------------

## Important Functions

### `fft()`

``` c
void fft(complex double a[], int n, int inverse)
```

Performs the recursive FFT or inverse FFT.

Parameters:

-   `a[]` - input array
-   `n` - current array size
-   `inverse` - identifies normal FFT or inverse FFT

### `nextPowerOfTwo()`

``` c
int nextPowerOfTwo(int size)
```

Finds the smallest power of 2 that is greater than or equal to the
required size.

------------------------------------------------------------------------

## Complexity Analysis

Let `N` be the padded size.

### FFT

``` text
O(N log N)
```

There are `log N` recursive levels and each level performs `O(N)`
combine work.

### Point-wise multiplication

``` text
O(N)
```

Each of the `N` elements is multiplied once.

### Inverse FFT

``` text
O(N log N)
```

Therefore, the overall complexity is:

``` text
O(N log N)
```

Since `n >= m`, we have `N = O(n)`, so:

``` text
Overall Time Complexity = O(n log n)
```

### Space Complexity

The recursive FFT uses temporary arrays, giving:

``` text
O(N) = O(n)
```

auxiliary space.

------------------------------------------------------------------------

## Complexity Comparison

  Method               Time Complexity
  -------------------- -----------------
  Direct Convolution   O(n²)
  FFT Convolution      O(n log n)

FFT is therefore more suitable when the input size becomes large.

------------------------------------------------------------------------

## C Program

The program uses:

``` c
#include <stdio.h>
#include <math.h>
#include <complex.h>
```

Compile using:

``` bash
gcc program.c -o program -lm
```

Run using:

``` bash
./program
```

On Windows with MinGW:

``` bash
gcc program.c -o program.exe -lm
program.exe
```

------------------------------------------------------------------------

## Notes

-   The implementation assumes `n >= m`.
-   The FFT size is chosen as a power of 2.
-   Zero-padding is used so that the recursive FFT can divide the array
    evenly.
-   The final answer contains exactly `m+n-1` values.
-   Small floating-point errors may occur because FFT uses
    floating-point complex arithmetic.

------------------------------------------------------------------------

## Conclusion

The convolution of two vectors can be calculated efficiently by treating
the vectors as polynomial coefficients and using FFT.

The algorithm performs:

``` text
FFT(A)
   +
FFT(B)
   ↓
Point-wise multiplication
   ↓
Inverse FFT
   ↓
Convolution
```

The divide-and-conquer FFT provides the required:

``` text
O(n log n)
```

time complexity, making it significantly faster than the direct `O(n²)`
method for large inputs.
