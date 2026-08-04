# DAA Lab 01 - Question 6

## Objective

Check whether a randomly generated array contains duplicate elements.

## Approach

- Generate `n` random integers.
- Sort the array using `qsort()`.
- Compare each element with its next element.
- If two adjacent elements are equal, duplicates exist.

## Algorithm

1. Read the value of `n`.
2. Generate `n` random numbers.
3. Sort the array.
4. Traverse the sorted array.
5. If any two adjacent elements are equal, report duplicates.

## Time Complexity

- Sorting: **O(n log n)**
- Checking: **O(n)**

Overall:

**O(n log n)**

## Space Complexity

**O(1)** (excluding the internal recursion of `qsort()`)

## Conclusion

We can see that for sufficiently large values of `n`, the `rand()` always ends up giving up a duplicate element and also since we have used qsort() This method become more and more efficient with larger value of `n`.