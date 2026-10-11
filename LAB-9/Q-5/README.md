# Candy Distribution Problem - Bi-directional Slope Greedy

## 1. Problem Title

**Candy Distribution Problem (Bi-directional Slope Greedy)**

## 2. Problem Statement

There are `n` children standing in a line. Each child has a rating.

The rules are:

1. Every child must receive at least one candy.
2. If a child has a higher rating than an immediate neighbour, that child must receive more candies than that neighbour.
3. Find the minimum total number of candies required.

The important point is that a child may have a higher-rated neighbour on either its left side, right side, or both sides.

---

## 3. Objective

The objective is to find the minimum number of candies satisfying all the rating constraints.

The program should solve the problem efficiently and demonstrate the **greedy approach using two directional passes**.

---

## 4. Approaches

A few possible approaches are:

- Brute force: repeatedly adjust candy values until all conditions are satisfied.
- Dynamic programming: store requirements for different positions.
- Two-pass greedy approach: calculate the requirement from the left and from the right, then combine them.

For a DAA lab, the **two-pass greedy approach** is simple, efficient, and directly demonstrates the bi-directional slope idea.

---

## 5. Basic Idea

We use two arrays:

- `left[i]`: minimum candies required by child `i` considering only the left neighbour.
- `right[i]`: minimum candies required by child `i` considering only the right neighbour.

### Left-to-right pass

If:

```text
rating[i] > rating[i-1]
```

then child `i` needs one more candy than child `i-1`.

### Right-to-left pass

If:

```text
rating[i] > rating[i+1]
```

then child `i` needs one more candy than child `i+1`.

### Combining the results

A child can have requirements from both sides. Therefore:

```text
candies[i] = max(left[i], right[i])
```

Finally, add all `candies[i]`.

---

## 6. Why This Approach Is Suitable

The two-pass greedy approach is suitable because:

- Every child starts with one candy.
- Each pass handles one direction independently.
- We never need to repeatedly modify already processed positions.
- Two linear scans are enough.
- The overall time complexity is `O(n)`.
- The logic is easy to explain in a DAA lab and viva.

---

## 7. Step-by-Step Algorithm

1. Read the number of children `n`.
2. Read the ratings.
3. Create arrays `left` and `right`.
4. Initialize every element of both arrays to `1`.
5. Traverse from left to right:
   - If the current rating is greater than the previous rating, set:
     `left[i] = left[i-1] + 1`.
6. Traverse from right to left:
   - If the current rating is greater than the next rating, set:
     `right[i] = right[i+1] + 1`.
7. For every child, take:
   `max(left[i], right[i])`.
8. Add these values to obtain the minimum total.
9. Print the result.

---

## 8. C Program

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, i;
    int *rating;
    int *left;
    int *right;
    int total = 0;

    printf("Enter number of children: ");
    scanf("%d", &n);

    rating = (int *)malloc(n * sizeof(int));
    left = (int *)malloc(n * sizeof(int));
    right = (int *)malloc(n * sizeof(int));

    if (rating == NULL || left == NULL || right == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter the ratings of the children:\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &rating[i]);
    }

    for (i = 0; i < n; i++) {
        left[i] = 1;
        right[i] = 1;
    }

    for (i = 1; i < n; i++) {
        if (rating[i] > rating[i - 1]) {
            left[i] = left[i - 1] + 1;
        }
    }

    for (i = n - 2; i >= 0; i--) {
        if (rating[i] > rating[i + 1]) {
            right[i] = right[i + 1] + 1;
        }
    }

    for (i = 0; i < n; i++) {
        if (left[i] > right[i]) {
            total += left[i];
        } else {
            total += right[i];
        }
    }

    printf("Minimum number of candies = %d\n", total);

    free(rating);
    free(left);
    free(right);

    return 0;
}
```

---

## 9. Program Explanation

### Header files

```c
#include <stdio.h>
#include <stdlib.h>
```

- `stdio.h` provides `printf()` and `scanf()`.
- `stdlib.h` provides `malloc()` and `free()`.

### Variables

```c
int n, i;
```

- `n` stores the number of children.
- `i` is the loop variable.

```c
int *rating;
int *left;
int *right;
```

- `rating` stores the input ratings.
- `left` stores the candy requirement considering the left neighbour.
- `right` stores the candy requirement considering the right neighbour.

```c
int total = 0;
```

Stores the final minimum number of candies.

### Dynamic memory

```c
rating = (int *)malloc(n * sizeof(int));
```

This creates an array capable of storing `n` ratings.

The same is done for `left` and `right`.

Dynamic arrays are used because the input size is provided at runtime.

### Reading ratings

```c
for (i = 0; i < n; i++) {
    scanf("%d", &rating[i]);
}
```

This reads the rating of every child.

### Initialization

```c
for (i = 0; i < n; i++) {
    left[i] = 1;
    right[i] = 1;
}
```

Every child must receive at least one candy, so both arrays initially contain `1`.

### First pass

```c
for (i = 1; i < n; i++) {
    if (rating[i] > rating[i - 1]) {
        left[i] = left[i - 1] + 1;
    }
}
```

This checks the left neighbour.

For example, if ratings are:

```text
1 2 3
```

the left requirements become:

```text
1 2 3
```

### Second pass

```c
for (i = n - 2; i >= 0; i--) {
    if (rating[i] > rating[i + 1]) {
        right[i] = right[i + 1] + 1;
    }
}
```

This performs the same idea from the right side.

It is important to traverse backwards because `right[i+1]` must already be known.

### Combining both directions

```c
if (left[i] > right[i])
    total += left[i];
else
    total += right[i];
```

This is equivalent to:

```text
total += max(left[i], right[i])
```

We choose the larger requirement because the child must satisfy both directional constraints.

### Freeing memory

```c
free(rating);
free(left);
free(right);
```

This releases the dynamically allocated memory.

---

## 10. Sample Input

```text
Enter number of children: 5
Enter the ratings of the children:
1 2 3 2 1
```

## 11. Sample Output

```text
Minimum number of candies = 9
```

---

## 12. Complete Dry Run

Consider:

```text
Ratings = [1, 2, 3, 2, 1]
```

Initially:

```text
left  = [1, 1, 1, 1, 1]
right = [1, 1, 1, 1, 1]
```

### Left-to-right pass

| i | Rating comparison | left[i] |
|---|---|---:|
| 1 | 2 > 1 | 2 |
| 2 | 3 > 2 | 3 |
| 3 | 2 > 3 is false | 1 |
| 4 | 1 > 2 is false | 1 |

So:

```text
left = [1, 2, 3, 1, 1]
```

### Right-to-left pass

Start from index `3`.

| i | Rating comparison | right[i] |
|---|---|---:|
| 3 | 2 > 1 | 2 |
| 2 | 3 > 2 | 3 |
| 1 | 2 > 3 is false | 1 |
| 0 | 1 > 2 is false | 1 |

So:

```text
right = [1, 1, 3, 2, 1]
```

### Combine

| Child | left[i] | right[i] | max | Candies |
|---:|---:|---:|---:|---:|
| 0 | 1 | 1 | 1 | 1 |
| 1 | 2 | 1 | 2 | 2 |
| 2 | 3 | 3 | 3 | 3 |
| 3 | 1 | 2 | 2 | 2 |
| 4 | 1 | 1 | 1 | 1 |

Total:

```text
1 + 2 + 3 + 2 + 1 = 9
```

Therefore:

```text
Minimum candies = 9
```

---

## 13. Why Taking Maximum Works

Suppose a child has:

```text
left[i] = 3
right[i] = 2
```

The child needs at least 3 candies to satisfy the left-side condition and at least 2 to satisfy the right-side condition.

Giving 3 satisfies both.

Giving 2 would violate the left-side condition.

Therefore:

```text
candies[i] = max(left[i], right[i])
```

---

## 14. Complexity Analysis

### Initialization

We visit all `n` children once:

```text
O(n)
```

### Left-to-right pass

We perform one pass through the array:

```text
O(n)
```

### Right-to-left pass

We perform another pass:

```text
O(n)
```

### Final summation

We visit every child once again:

```text
O(n)
```

Therefore:

```text
T(n) = O(n) + O(n) + O(n) + O(n)
     = O(4n)
     = O(n)
```

Constant factors are ignored in asymptotic analysis.

### Best case

`O(n)`

Even if ratings are all equal, the program still performs all passes.

### Average case

`O(n)`

The number of passes does not depend on the arrangement of ratings.

### Worst case

`O(n)`

Increasing, decreasing, or random ratings all require the same linear scans.

### Space complexity

Three arrays of size `n` are used:

```text
rating -> O(n)
left   -> O(n)
right  -> O(n)
```

Therefore:

```text
O(n) + O(n) + O(n) = O(n)
```

So the auxiliary/input storage used by this implementation is **O(n)**.

---

## 15. Complexity Graph

The accompanying PNG graph models the algorithm's operation growth using the linear passes:

- Forward pass: `n`
- Backward pass: `n`
- Combined two-pass growth: `2n`

These are operation-growth models, not measured execution times.

The graph is:

**`candy_distribution_complexity.png`**

It shows that the number of operations increases linearly as the number of children increases.

---

## 16. Advantages

1. Simple to understand.
2. Uses only two main directional passes.
3. Handles increasing and decreasing rating sequences.
4. Handles a child that must satisfy conditions from both neighbours.
5. Runs in `O(n)` time.
6. Easy to implement in C.
7. Easy to explain during viva.

---

## 17. Disadvantages / Limitations

1. This implementation uses `O(n)` additional memory.
2. Three arrays are maintained, which uses more memory than some optimized one-array implementations.
3. The algorithm assumes the children are arranged in a single line.
4. The solution is designed specifically for the stated neighbour-based candy constraints.

---

## 18. Viva Preparation

### Q1. What is the main idea of the algorithm?

We make two passes. The first pass handles the left neighbour and the second pass handles the right neighbour. Finally, we take the maximum requirement for every child.

### Q2. Why do we need two passes?

Because a child can have a higher rating than its left neighbour and also a higher rating than its right neighbour. One directional scan cannot handle both cases correctly.

### Q3. Why do we initialize every child with one candy?

Because the problem says every child must receive at least one candy.

### Q4. Why do we use `max(left[i], right[i])`?

The child has to satisfy both directional requirements. The larger value is enough to satisfy both.

### Q5. What is the time complexity?

`O(n)` because the program performs a constant number of linear scans over the `n` children.

### Q6. What is the space complexity?

`O(n)` because the program stores the ratings and two additional arrays of size `n`.

### Q7. What happens when two adjacent children have equal ratings?

There is no requirement for one to receive more candies, so the default requirement of one candy remains sufficient.

### Q8. What happens for increasing ratings such as `1 2 3 4`?

The left pass produces:

```text
1 2 3 4
```

so the minimum candies are `1 + 2 + 3 + 4 = 10`.

### Q9. What happens for decreasing ratings such as `4 3 2 1`?

The right pass handles the sequence and produces:

```text
4 3 2 1
```

so the total is also `10`.

### Q10. Is this brute force?

No. It does not repeatedly try different candy assignments. It determines the minimum directional requirements using two greedy passes.

### Q11. Why is the right-to-left loop started at `n-2`?

The last child has no child to its right, so the comparison starts from the second-last child.

### Q12. Why is `left[i] = left[i-1] + 1` used?

If the current rating is greater than the previous rating, the current child must have more candies than the previous child. Therefore, we add one.

### Q13. Why is `right[i] = right[i+1] + 1` used?

During the reverse pass, if the current rating is greater than the child on its right, the current child needs one more candy than that right neighbour.

### Q14. Why is `malloc()` used?

The value of `n` is entered at runtime, so dynamic memory allocation allows arrays to be created according to the input size.

### Q15. Why do we call `free()`?

Memory allocated using `malloc()` should be released using `free()` after it is no longer needed.

---

## 19. C-Code-Specific Viva Questions

### Q1. Why does the first loop start from `i = 1`?

Because `left[i]` compares the current child with the previous child. Index `0` has no previous child.

### Q2. Why does the second loop start from `i = n - 2`?

Because it compares `rating[i]` with `rating[i+1]`. The last index has no element on its right.

### Q3. Why are `left` and `right` initialized to `1`?

Every child must get at least one candy, so `1` is the minimum starting value.

### Q4. Why is `total` initialized to zero?

It stores the sum of the final candy requirements, so we need to start the sum from zero.

### Q5. Why do we check whether `malloc()` returned `NULL`?

`NULL` means memory allocation failed. Checking it prevents the program from using invalid memory.

---

## 20. How to Compile and Run

### Using GCC

Save the program as:

```text
candy_distribution.c
```

Compile:

```bash
gcc candy_distribution.c -o candy_distribution
```

Run on Linux/macOS:

```bash
./candy_distribution
```

On Windows:

```bash
candy_distribution.exe
```

---

## 21. Teacher-Explanation Version

If the teacher asks, **"Explain your algorithm and code"**, you can say:

> "Sir/Ma'am, this is the Candy Distribution problem. Every child has to get at least one candy, and if a child has a higher rating than an immediate neighbour, the child must get more candies than that neighbour.
>
> I use a two-pass greedy approach. First, I move from left to right and store the minimum candies required when considering only the left neighbour. If the current rating is greater than the previous rating, I give one more candy than the previous child.
>
> Then I move from right to left and do the same thing considering the right neighbour. Finally, for every child I take the maximum of the left and right requirements because both conditions have to be satisfied.
>
> I add these final values to get the minimum total number of candies. Since I make only a constant number of passes through the array, the time complexity is O(n). The arrays used for ratings, left requirements and right requirements require O(n) space."

---

## 22. Conclusion

The bi-directional slope greedy algorithm solves the Candy Distribution problem efficiently.

By processing the children once from left to right and once from right to left, we calculate the requirements imposed by both neighbours. Taking the maximum of these requirements gives a valid minimum candy assignment.

The algorithm runs in:

```text
Time:  O(n)
Space: O(n)
```

This makes it suitable for a DAA lab implementation and viva explanation.
