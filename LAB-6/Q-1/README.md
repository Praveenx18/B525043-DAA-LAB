# DAA Lab -- 1D Array Operations and Complexity Analysis

## Problem Statement

Given an array containing `n` unsorted integer elements, determine the
worst-case time complexity of the following operations:

1.  Finding the maximum element
2.  Finding the first and second largest elements
3.  Finding the mean
4.  Finding the median
5.  Finding the standard deviation
6.  Finding the mode
7.  Removing all duplicates
8.  Reversing the elements of the array
9.  Partitioning the array with respect to a given pivot so that all
    elements less than the pivot appear after all elements greater than
    or equal to the pivot

A C program is written to validate these operations and study their
corresponding complexity.

------------------------------------------------------------------------

## Approach Used

The program uses simple, beginner-friendly approaches suitable for a DAA
laboratory:

  -----------------------------------------------------------------------
  No.               Operation         Approach          Worst-Case Time
  ----------------- ----------------- ----------------- -----------------
  1                 Maximum           Single traversal  `O(n)`

  2                 First & Second    Single traversal  `O(n)`
                    Largest           using two         
                                      variables         

  3                 Mean              Sum all elements  `O(n)`
                                      and divide by `n` 

  4                 Median            Copy array +      `O(n log n)`
                                      Merge Sort        

  5                 Standard          Calculate mean    `O(n)`
                    Deviation         and squared       
                                      differences       

  6                 Mode              Nested loops to   `O(n²)`
                                      count frequencies 

  7                 Remove Duplicates Compare each      `O(n²)`
                                      element with      
                                      unique elements   

  8                 Reverse           Two-pointer       `O(n)`
                                      swapping          

  9                 Partition         Two-pointer       `O(n)`
                                      partition around  
                                      pivot             
  -----------------------------------------------------------------------

------------------------------------------------------------------------

## Algorithm Details

### 1. Find Maximum

-   Assume the first element is the maximum.
-   Compare every remaining element with the current maximum.
-   Update the maximum whenever a larger element is found.

**Complexity:** `O(n)`

### 2. Find First and Second Largest

-   Maintain `largest` and `secondLargest`.
-   Traverse the array once.
-   Update the two values whenever a larger element is found.
-   The program considers the second largest as the second **distinct**
    largest value.

**Complexity:** `O(n)`

### 3. Find Mean

Use:

`Mean = Sum of all elements / n`

Traverse the array once to calculate the sum.

**Complexity:** `O(n)`

### 4. Find Median

-   Copy the original array so that the original data is not changed.
-   Sort the copy using Merge Sort.
-   If `n` is odd, select the middle element.
-   If `n` is even, calculate the average of the two middle elements.

**Complexity:** `O(n log n)`

Merge Sort is used because it follows the Divide and Conquer technique
and has a guaranteed `O(n log n)` worst-case time complexity.

### 5. Find Standard Deviation

First calculate the mean.

Then use:

`Standard Deviation = sqrt(sum((x - mean)²) / n)`

Two linear traversals are still `O(n)`.

**Complexity:** `O(n)`

### 6. Find Mode

-   For every element, scan the complete array.
-   Count how many times that element occurs.
-   Store the element having the highest frequency.

**Complexity:** `O(n²)`

### 7. Remove Duplicates

-   Maintain a separate array containing unique elements.
-   For every input element, check whether it is already present in the
    unique array.
-   Add it only if it has not appeared before.

**Complexity:** `O(n²)` in the worst case.

### 8. Reverse Array

Use two pointers:

-   `left = 0`
-   `right = n - 1`

Swap the elements and move the pointers toward the center.

**Complexity:** `O(n)`

### 9. Partition Around Pivot

Given a pivot:

-   Elements greater than or equal to the pivot should be before
    elements less than the pivot.
-   Use two pointers from the left and right.
-   Find elements on the wrong sides and swap them.

The two groups do not need to be sorted.

**Complexity:** `O(n)`

------------------------------------------------------------------------

## Complexity Summary

  ------------------------------------------------------------------------
  Operation         Best Case   Average Case     Worst Case    Extra Space
  ------------ -------------- -------------- -------------- --------------
  Find Maximum         `O(n)`         `O(n)`         `O(n)`         `O(1)`

  First &              `O(n)`         `O(n)`         `O(n)`         `O(1)`
  Second                                                    
  Largest                                                   

  Mean                 `O(n)`         `O(n)`         `O(n)`         `O(1)`

  Median using   `O(n log n)`   `O(n log n)`   `O(n log n)`         `O(n)`
  Merge Sort                                                

  Standard             `O(n)`         `O(n)`         `O(n)`         `O(1)`
  Deviation                                                 

  Mode                `O(n²)`        `O(n²)`        `O(n²)`         `O(1)`

  Remove              `O(n²)`        `O(n²)`        `O(n²)`         `O(n)`
  Duplicates                                                

  Reverse              `O(n)`         `O(n)`         `O(n)`         `O(1)`

  Partition            `O(n)`         `O(n)`         `O(n)`         `O(1)`
  ------------------------------------------------------------------------

> Note: The space shown is the extra space required by the individual
> operation. The complete C program uses fixed-size arrays for copies
> and temporary storage.

------------------------------------------------------------------------

## Sample Input

``` text
Enter number of elements: 8
Enter 8 elements:
7 2 9 2 5 9 1 4
```

## Sample Output

``` text
Original Array: 7 2 9 2 5 9 1 4

1. Maximum element = 9
2. First largest = 9
   Second largest = 7

3. Mean = 4.88
4. Median = 4.50
5. Standard deviation = 2.80
6. Mode = 2

7. Array after removing duplicates: 7 2 9 5 1 4
8. Reversed array: 4 1 9 5 2 9 2 7

Enter pivot element for partition: 5
9. Array after partitioning: 7 9 9 5 2 2 1 4
```

The exact order of elements after partition may vary because
partitioning only separates elements into two groups; it does not sort
them.

------------------------------------------------------------------------

## Compilation and Execution

### GCC

Compile using:

``` bash
gcc program.c -o program -lm
```

Run using:

``` bash
./program
```

On Windows:

``` bash
program.exe
```

The `-lm` option links the math library required for the `sqrt()`
function.

------------------------------------------------------------------------

## Graph

The accompanying complexity graph plots:

-   **X-axis:** `n` -- number of array elements
-   **Y-axis:** approximate number of operations
-   Linear operations such as maximum, mean, standard deviation,
    reverse, and partition show `O(n)` growth.
-   Median shows `O(n log n)` growth.
-   Mode and duplicate removal show quadratic `O(n²)` growth.

The graph demonstrates visually that quadratic algorithms grow much
faster as the input size increases.

------------------------------------------------------------------------

## Important Variables

  Variable          Purpose
  ----------------- ---------------------------------------------------
  `arr[]`           Stores the original input array
  `n`               Number of elements
  `max`             Stores the maximum element
  `largest`         Stores the largest element
  `secondLargest`   Stores the second distinct largest element
  `sum`             Stores the sum or accumulated squared differences
  `mean`            Stores the average of the elements
  `temp[]`          Copy of the array used for median calculation
  `frequency`       Counts occurrences of an element
  `mode`            Stores the element with maximum frequency
  `unique[]`        Stores elements after duplicate removal
  `left`, `right`   Two pointers used for reverse and partition
  `pivot`           Element used as the partition reference

------------------------------------------------------------------------

## Important Functions

  -----------------------------------------------------------------------
  Function                            Purpose
  ----------------------------------- -----------------------------------
  `findMaximum()`                     Finds maximum element

  `findLargestTwo()`                  Finds largest and second largest
                                      distinct elements

  `findMean()`                        Calculates mean

  `merge()`                           Merges two sorted portions

  `mergeSort()`                       Sorts an array using Merge Sort

  `findMedian()`                      Calculates median

  `findStandardDeviation()`           Calculates standard deviation

  `findMode()`                        Finds the most frequent element

  `removeDuplicates()`                Removes duplicate elements

  `reverseArray()`                    Reverses the array

  `partitionArray()`                  Partitions the array around a pivot

  `printArray()`                      Displays array elements
  -----------------------------------------------------------------------

------------------------------------------------------------------------

## Key DAA Concepts Demonstrated

-   Array traversal
-   Linear search
-   Nested loops
-   Divide and Conquer
-   Merge Sort
-   Two-pointer technique
-   Frequency counting
-   Duplicate removal
-   Partitioning
-   Time complexity analysis
-   Space complexity analysis

------------------------------------------------------------------------

## Conclusion

The program demonstrates that different operations on the same unsorted
array can have very different computational costs.

Most operations can be performed in `O(n)` time using a single
traversal. Median requires `O(n log n)` in this implementation because
the array is sorted using Merge Sort. Mode and duplicate removal use
nested comparisons, resulting in `O(n²)` worst-case complexity.

This experiment shows why choosing an appropriate algorithm is
important, especially when the input size `n` becomes large.
