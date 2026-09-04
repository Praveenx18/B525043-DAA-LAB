# Sorting via Reversal Procedure

## DAA Lab Assignment

### Problem Statement

Given a permutation `p` of the integers `1` to `n`, sort it into increasing order `[1, 2, ..., n]`.

The only allowed operation is:

`reverse(p, i, j)`

which reverses the elements from position `i` through `j`.

The assignment has two requirements:

1. Show mathematically that any permutation can be sorted using `O(n)` reversals.
2. When the cost of `reverse(p, i, j)` is `|j - i| + 1`, design an algorithm that sorts the permutation with total cost `O(n log^2 n)`.
3. Analyze the running time and cost, prove correctness, and validate the algorithm using a C program.

---

## 1. O(n) Reversal Proof

A simple way to sort any permutation is to place the values one by one.

For `i = 1` to `n - 1`:

1. Find the current position of value `i`.
2. Reverse the section from position `i` to the position of `i`.

After each reversal, value `i` is placed permanently in its correct position.

At most one reversal is required for each of the first `n - 1` values.

Therefore:

`Number of reversals <= n - 1 = O(n)`

So every permutation can be sorted using `O(n)` reversals.

### Why this is not enough

Although the number of reversals is `O(n)`, a reversal may contain `O(n)` elements.

For example, a reverse-sorted permutation can require reversals of lengths close to:

`n, n-1, n-2, ...`

Thus the total cost can become:

`n + (n-1) + ... + 1 = O(n^2)`

Therefore, we need a different algorithm to obtain the required `O(n log^2 n)` total cost.

---

## 2. Proposed Algorithm

The required algorithm uses **Divide and Conquer**.

The values are repeatedly divided into two groups.

For a range of values `[low, high]`, calculate:

`mid = (low + high) / 2`

Then divide the values into:

- Lower half: `low ... mid`
- Upper half: `mid + 1 ... high`

Using a recursive stable partition, all lower-half values are placed before all upper-half values.

After partitioning, recursively sort both halves.

---

## 3. Stable Partition Using Reversals

Suppose the two halves have already been partitioned:

`[Lower Upper] [Lower Upper]`

The required arrangement is:

`[Lower Lower] [Upper Upper]`

The two middle blocks are exchanged using three reversals.

If the blocks are `A` and `B`:

1. Reverse `A`
2. Reverse `B`
3. Reverse `A + B`

This changes:

`A B`

into:

`B A`

For example:

`[5 6] [2 3]`

Reverse first block:

`[6 5] [2 3]`

Reverse second block:

`[6 5] [3 2]`

Reverse both:

`[2 3 5 6]`

Thus two adjacent blocks can be exchanged using three reversals.

---

## 4. Algorithm Steps

### `reverse(a, l, r)`

1. Set two pointers `l` and `r`.
2. Swap the elements at `l` and `r`.
3. Increment `l`.
4. Decrement `r`.
5. Continue until the pointers meet.

### `stablePartition(a, l, r, value)`

1. If there is only one element, check whether it belongs to the lower group.
2. Divide the current section into two halves.
3. Recursively partition the left half.
4. Recursively partition the right half.
5. Exchange the middle upper and lower blocks using reversals.
6. Return the number of lower-group elements.

### `sortRange(a, l, r, low, high)`

1. If `low >= high`, stop because there is only one value.
2. Calculate `mid`.
3. Stable-partition the current range around `mid`.
4. Calculate the number of values in the lower half.
5. Recursively sort the lower half.
6. Recursively sort the upper half.

---

## 5. C Program

```c
#include <stdio.h>

/* Reverse elements from index l to r */
void reverse(int a[], int l, int r)
{
    int temp;

    while (l < r)
    {
        temp = a[l];
        a[l] = a[r];
        a[r] = temp;

        l++;
        r--;
    }
}

/*
    Stable partition the array from l to r.

    Elements <= value are placed first.
    Elements > value are placed second.

    Returns the number of elements <= value.
*/
int stablePartition(int a[], int l, int r, int value)
{
    int mid;
    int leftCount, rightCount;
    int leftLength;
    int falseLeft;
    int trueRight;

    if (l == r)
    {
        if (a[l] <= value)
            return 1;
        else
            return 0;
    }

    mid = (l + r) / 2;

    /* Partition left half */
    leftCount = stablePartition(a, l, mid, value);

    /* Partition right half */
    rightCount = stablePartition(a, mid + 1, r, value);

    leftLength = mid - l + 1;

    falseLeft = leftLength - leftCount;
    trueRight = rightCount;

    /* Reverse elements > value in left half */
    if (falseLeft > 0)
    {
        reverse(a, l + leftCount, mid);
    }

    /* Reverse elements <= value in right half */
    if (trueRight > 0)
    {
        reverse(a, mid + 1, mid + trueRight);
    }

    /* Exchange the two blocks */
    if (falseLeft > 0 && trueRight > 0)
    {
        reverse(a, l + leftCount, mid + trueRight);
    }

    return leftCount + rightCount;
}

/* Sort the values from low to high */
void sortRange(int a[], int l, int r, int low, int high)
{
    int mid;
    int lowerCount;

    if (low >= high)
        return;

    mid = (low + high) / 2;

    /* Put lower values before upper values */
    stablePartition(a, l, r, mid);

    /* Number of values in lower half */
    lowerCount = mid - low + 1;

    /* Sort lower half */
    sortRange(a, l, l + lowerCount - 1, low, mid);

    /* Sort upper half */
    sortRange(a, l + lowerCount, r, mid + 1, high);
}

int main()
{
    int n;
    int a[100];

    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter the permutation: ");

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &a[i]);
    }

    sortRange(a, 0, n - 1, 1, n);

    printf("Sorted permutation: ");

    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }

    printf("\n");

    return 0;
}
```

---

## 6. Sample Input

```text
Enter number of elements: 4
Enter the permutation: 4 1 3 2
```

## Sample Output

```text
Sorted permutation: 1 2 3 4
```

---

## 7. Example Dry Run

Input:

`[4, 1, 3, 2]`

Initially:

`[4, 1, 3, 2]`

For values `1...4`:

`mid = 2`

Partition into:

- Lower values: `{1, 2}`
- Upper values: `{3, 4}`

After stable partition:

`[1, 2, 4, 3]`

Now recursively sort:

`[1, 2]`

and:

`[4, 3]`

The first part is already sorted.

For `[4, 3]`, reverse the two elements:

`[3, 4]`

Final result:

`[1, 2, 3, 4]`

---

## 8. Correctness Proof

### Base Case

If `low >= high`, there is only one value.

A single value is already sorted.

Therefore, the algorithm is correct for the base case.

### Inductive Step

For a range `[low, high]`, the stable partition places every value from `low` through `mid` before every value from `mid + 1` through `high`.

The algorithm then recursively sorts both groups.

Assuming the smaller recursive problems are solved correctly, both groups become sorted.

Therefore the complete range becomes:

`[low, low+1, ..., high]`

Hence the algorithm correctly sorts the permutation.

---

## 9. Complexity Analysis

### Cost of `reverse`

If `k` elements are reversed, the cost is:

`O(k)`

because every element involved in the reversal is processed.

### Stable Partition

The recurrence is:

`P(n) = 2P(n/2) + O(n)`

Therefore:

`P(n) = O(n log n)`

### Overall Cost

The main divide-and-conquer algorithm has `O(log n)` levels.

Each level performs partitioning with a total cost bounded by approximately `O(n log n)`.

Therefore:

`Total Cost = O(n log n) × O(log n)`

So:

`O(n log^2 n)`

### Running Time

Since the C program performs each reversal element by element, its running time follows the total reversal cost:

`O(n log^2 n)`

### Space Complexity

The algorithm works in-place.

The extra space is mainly the recursion stack:

`O(log n)`

---

## 10. Complexity Summary

| Property | Complexity |
|---|---|
| Number of reversals in simple method | O(n) |
| Worst-case cost of simple method | O(n²) |
| Stable partition cost | O(n log n) |
| Total reversal cost | O(n log² n) |
| Best-case running time | O(n log² n) |
| Average-case running time | O(n log² n) |
| Worst-case running time | O(n log² n) |
| Auxiliary space | O(log n) |

---

## 11. Advantages

- Uses only the allowed reversal operation.
- Uses divide and conquer.
- Achieves the required `O(n log² n)` cost.
- Works for any permutation of `1...n`.
- Works in-place.
- Uses basic C concepts suitable for a DAA lab.

## 12. Disadvantages

- More complicated than the simple O(n)-reversal method.
- Uses recursion.
- Requires more reversals than the simple method.
- The implementation is harder to understand than ordinary sorting algorithms.

---

## 13. Viva Questions

### 1. What is the main idea?

Divide the values into two halves, partition them using reversals, and recursively sort both halves.

### 2. Why not use the simple O(n)-reversal method?

Because O(n) reversals do not guarantee low total cost. The total cost can become O(n²).

### 3. What is the cost of a reversal?

The number of elements reversed:

`|j - i| + 1`

### 4. Why are three reversals used?

Three reversals can exchange two adjacent blocks.

### 5. What is stable partition?

Partitioning elements into two groups while preserving the relative order within each group.

### 6. What is the stable partition recurrence?

`P(n) = 2P(n/2) + O(n)`

which gives `O(n log n)`.

### 7. What is the overall cost?

`O(n log² n)`.

### 8. What is the space complexity?

`O(log n)` due to recursion.

### 9. What is the base case?

When `low >= high`, meaning there is only one value.

### 10. What does `leftCount` store?

It stores the number of elements in the left half that belong to the lower group.

### 11. Why is `temp` used?

It temporarily stores an element while two elements are swapped during reversal.

### 12. Why does the reverse loop use `l < r`?

Because after the two pointers meet, no more swaps are necessary.

### 13. Why does `stablePartition()` return a count?

The count tells `sortRange()` where the lower group ends and the upper group begins.

### 14. What happens when `n` increases?

The running time grows as `O(n log² n)`, which is slower than linear but better asymptotically than quadratic growth.

### 15. What is the difference between reversal count and reversal cost?

Reversal count tells how many operations are performed. Reversal cost also considers the number of elements affected by each operation.

---

## 14. Teacher Explanation

A short explanation for viva:

> "My algorithm uses divide and conquer. First, I divide the values into lower and upper halves. I use a stable partition to place all lower values before the upper values. When two blocks are in the wrong order, I exchange them using three reversals. Then I recursively sort the two halves.
>
> A simple method can sort using O(n) reversals, but the reversals can be very long, giving O(n²) total cost. In my algorithm, stable partition costs O(n log n), and there are O(log n) levels. Therefore, the total reversal cost and running time are O(n log² n). The algorithm works in-place and uses O(log n) recursion space."

---

## 15. Graph

The complexity graph should show:

- X-axis: Input size `n`
- Y-axis: Total reversal cost
- Growth: `O(n log² n)`

The graph is illustrative and is intended to demonstrate the theoretical growth of the algorithm.

---

## Conclusion

The permutation can always be sorted using `O(n)` reversals. However, because reversal cost depends on the length of each reversal, the simple method can have `O(n²)` total cost.

The divide-and-conquer stable-partition approach reduces the total reversal cost to:

`O(n log² n)`

and the corresponding C implementation follows the same analysis.
