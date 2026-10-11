# Greedy Superstring — DAA Lab

## 1. Problem Title

**Greedy Superstring / Shortest Common Superstring using Greedy Approach**

---

## 2. Problem Statement

Given a set of strings:

`S = {s1, s2, ..., sn}`

find a single string that contains every input string as a substring.

The optimization version asks for the **shortest possible common superstring**.

For this lab, we implement the classic greedy heuristic:

1. Find the pair of strings having the maximum overlap.
2. Merge the pair using that overlap.
3. Replace the two strings by the merged string.
4. Repeat until only one string remains.

### Important theoretical note

The Shortest Common Superstring problem is NP-hard, so finding the exact optimum is difficult for general inputs.

The classic greedy method was conjectured to have approximation ratio 2. Recent 2026 arXiv work by Hiroki Shibata gives a lower bound of at least `9/4` for the greedy algorithm, disproving that conjecture if the result is accepted as stated. The result is currently a preprint, so this README does not treat it as a peer-reviewed final theorem.

A separate August 2026 arXiv paper also showed that strings of length 6 can already achieve ratio 2 for the greedy algorithm.

---

## 3. Objective

The objectives of this program are:

- Understand the Shortest Common Superstring problem.
- Understand the idea of string overlap.
- Implement a greedy approximation/heuristic.
- Practice greedy algorithm design.
- Analyze the time and space complexity.
- Understand why the greedy result is not necessarily optimal.

---

## 4. Basic Idea

Suppose we have:

```text
ABCDEF
DEFG
```

The suffix `DEF` of the first string matches the prefix `DEF` of the second string.

Therefore, the overlap is 3 and we can merge them as:

```text
ABCDEFG
```

Instead of writing:

```text
ABCDEFDEFG
```

the greedy algorithm tries to obtain the largest possible overlap at every step.

### Why greedy?

The largest overlap gives the largest immediate saving in the final string length.

This makes the algorithm simple and usually produces a reasonably short superstring, although it does not always produce the globally shortest one.

---

## 5. Handling Contained Strings

Before applying the greedy merging process, the program removes a string if it is already completely contained inside another string.

For example:

```text
ABCD
BC
```

`BC` is already present inside `ABCD`, so it does not need to be merged separately.

This preprocessing is important because otherwise the greedy algorithm could unnecessarily increase the answer.

---

## 6. Step-by-Step Algorithm

### Step 1: Read the input strings

Store all input strings in an array.

### Step 2: Remove contained strings

For every pair of strings, check whether one is a substring of another.

If a string is completely contained in another string, remove it.

### Step 3: Find maximum overlap

For every pair `(i, j)`, calculate:

- suffix of string `i`
- prefix of string `j`

Find the largest matching length.

### Step 4: Merge the best pair

If the maximum overlap is `k`, merge:

```text
string_i + non-overlapping part of string_j
```

For example:

```text
ABCDEF
DEFG
```

with overlap 3 becomes:

```text
ABCDEFG
```

### Step 5: Replace the pair

Remove the two old strings and insert the merged string.

### Step 6: Repeat

Continue until only one string remains.

### Step 7: Print the result

The remaining string is the greedy superstring.

---

## 7. C Program

```c
#include <stdio.h>
#include <string.h>

#define MAX_STRINGS 100
#define MAX_LEN 10000

/* Remove newline added by fgets() */
void removeNewline(char str[]) {
    int len = strlen(str);

    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

/* Return 1 if small string is present inside big string */
int isSubstring(char big[], char small[]) {
    return strstr(big, small) != NULL;
}

/* Find maximum suffix-prefix overlap */
int findOverlap(char a[], char b[]) {
    int lenA = strlen(a);
    int lenB = strlen(b);
    int maxOverlap;

    if (lenA < lenB)
        maxOverlap = lenA;
    else
        maxOverlap = lenB;

    for (int len = maxOverlap; len > 0; len--) {
        int match = 1;

        for (int i = 0; i < len; i++) {
            if (a[lenA - len + i] != b[i]) {
                match = 0;
                break;
            }
        }

        if (match)
            return len;
    }

    return 0;
}

/* Merge b after a using the given overlap */
void mergeStrings(char result[], char a[], char b[], int overlap) {
    int lenA = strlen(a);

    strcpy(result, a);
    strcat(result, b + overlap);

    result[lenA + strlen(b) - overlap] = '\0';
}

/* Remove a string from the array */
void removeString(char strings[][MAX_LEN], int *n, int index) {
    for (int i = index; i < *n - 1; i++) {
        strcpy(strings[i], strings[i + 1]);
    }

    (*n)--;
}

/* Remove strings which are already contained in another string */
void removeContainedStrings(char strings[][MAX_LEN], int *n) {
    int removed;

    do {
        removed = 0;

        for (int i = 0; i < *n; i++) {
            for (int j = 0; j < *n; j++) {
                if (i != j && isSubstring(strings[j], strings[i])) {
                    removeString(strings, n, i);
                    removed = 1;
                    break;
                }
            }

            if (removed)
                break;
        }

    } while (removed);
}

int main() {
    int n;

    printf("Enter number of strings: ");
    scanf("%d", &n);
    getchar();

    char strings[MAX_STRINGS][MAX_LEN];

    printf("Enter the strings:\n");

    for (int i = 0; i < n; i++) {
        fgets(strings[i], MAX_LEN, stdin);
        removeNewline(strings[i]);
    }

    /* Remove duplicate/contained strings */
    removeContainedStrings(strings, &n);

    while (n > 1) {
        int bestI = 0;
        int bestJ = 1;
        int bestOverlap = -1;

        /* Find pair with maximum overlap */
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j)
                    continue;

                int currentOverlap = findOverlap(strings[i], strings[j]);

                if (currentOverlap > bestOverlap) {
                    bestOverlap = currentOverlap;
                    bestI = i;
                    bestJ = j;
                }
            }
        }

        char merged[MAX_LEN];

        mergeStrings(
            merged,
            strings[bestI],
            strings[bestJ],
            bestOverlap
        );

        /* Replace first string by merged string */
        strcpy(strings[bestI], merged);

        /* Remove second string */
        removeString(strings, &n, bestJ);
    }

    printf("\nGreedy Superstring: %s\n", strings[0]);

    return 0;
}
```

---

## 8. Important Functions

### `removeNewline()`

```c
void removeNewline(char str[])
```

Removes the newline character added by `fgets()`.

For example:

```text
"ABC\n"
```

becomes:

```text
"ABC"
```

---

### `isSubstring()`

```c
int isSubstring(char big[], char small[])
```

Checks whether `small` occurs inside `big`.

It uses the standard C function:

```c
strstr()
```

It returns a non-zero value when the substring exists.

---

### `findOverlap()`

```c
int findOverlap(char a[], char b[])
```

This is one of the most important functions.

It checks how many characters at the end of `a` match the beginning of `b`.

Example:

```text
a = ABCDEF
b = DEFG
```

The matching part is:

```text
DEF
```

So the function returns:

```text
3
```

It starts from the largest possible overlap and moves downward. Therefore, the first successful match is the maximum overlap.

---

### `mergeStrings()`

```c
void mergeStrings(char result[], char a[], char b[], int overlap)
```

Combines two strings according to the overlap.

Example:

```text
a = ABCDEF
b = DEFG
overlap = 3
```

Result:

```text
ABCDEFG
```

The first string is copied completely and only the non-overlapping portion of the second string is appended.

---

### `removeString()`

```c
void removeString(char strings[][MAX_LEN], int *n, int index)
```

Removes one string from the array.

Since C arrays do not automatically shrink, the function shifts all strings after the removed position one place to the left.

---

### `removeContainedStrings()`

```c
void removeContainedStrings(char strings[][MAX_LEN], int *n)
```

Removes strings that are already present inside another string.

Example:

```text
ABCDEF
BCD
```

`BCD` can be removed because it is already contained in `ABCDEF`.

---

## 9. Main Program Explanation

### Reading input

```c
scanf("%d", &n);
getchar();
```

`scanf()` reads the number of strings.

`getchar()` consumes the newline left in the input buffer.

Then:

```c
fgets(strings[i], MAX_LEN, stdin);
```

reads each complete string.

---

### Preprocessing

```c
removeContainedStrings(strings, &n);
```

removes strings that are already contained in another input string.

---

### Greedy loop

```c
while (n > 1)
```

The algorithm continues until only one string remains.

Inside the loop:

```c
bestOverlap = -1;
```

keeps track of the best overlap found so far.

The nested loops examine every ordered pair:

```c
for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
```

For every pair, `findOverlap()` is called.

If the current overlap is larger:

```c
if (currentOverlap > bestOverlap)
```

the best pair is updated.

---

### Merging

After finding the best pair:

```c
mergeStrings(merged, strings[bestI],
             strings[bestJ], bestOverlap);
```

creates the combined string.

Then:

```c
strcpy(strings[bestI], merged);
```

stores the merged string.

Finally:

```c
removeString(strings, &n, bestJ);
```

removes the second string.

Therefore, the number of strings decreases by one in every greedy iteration.

---

## 10. Important Variables

| Variable | Meaning |
|---|---|
| `n` | Current number of strings |
| `strings` | Array containing the current strings |
| `i` | First string index |
| `j` | Second string index |
| `bestI` | Index of first string in best pair |
| `bestJ` | Index of second string in best pair |
| `bestOverlap` | Largest overlap found so far |
| `currentOverlap` | Overlap of current pair |
| `merged` | Temporary string containing the merged result |
| `overlap` | Number of characters shared by two strings |

---

# 11. Sample Input

```text
Enter number of strings: 4
Enter the strings:
ABCDEF
DEFG
EFGHI
HIJK
```

# 12. Sample Output

```text
Greedy Superstring: ABCDEFGHIJK
```

The exact result can depend on tie-breaking when multiple pairs have the same maximum overlap.

---

# 13. Complete Dry Run

Consider:

```text
S = {ABCDEF, DEFG, EFGHI, HIJK}
```

### Initial set

| String | Value |
|---|---|
| S1 | ABCDEF |
| S2 | DEFG |
| S3 | EFGHI |
| S4 | HIJK |

---

### Iteration 1

Important overlaps:

| Pair | Overlap |
|---|---:|
| ABCDEF → DEFG | 3 (`DEF`) |
| DEFG → EFGHI | 2 (`EF`) |
| EFGHI → HIJK | 2 (`HI`) |

The maximum overlap is:

```text
3
```

Merge:

```text
ABCDEF + DEFG
```

using `DEF`.

Result:

```text
ABCDEFG
```

New set:

```text
{ABCDEFG, EFGHI, HIJK}
```

---

### Iteration 2

Important overlaps:

| Pair | Overlap |
|---|---:|
| ABCDEFG → EFGHI | 3 (`EFG`) |
| EFGHI → HIJK | 2 (`HI`) |

Maximum overlap = `3`.

Merge:

```text
ABCDEFG
EFGHI
```

Result:

```text
ABCDEFGHI
```

New set:

```text
{ABCDEFGHI, HIJK}
```

---

### Iteration 3

Overlap:

```text
ABCDEFGHI → HIJK
```

The common part is:

```text
HI
```

So overlap = `2`.

Merge:

```text
ABCDEFGHI + HIJK
```

Result:

```text
ABCDEFGHIJK
```

Only one string remains.

### Final Answer

```text
ABCDEFGHIJK
```

Every original string is contained in it:

```text
ABCDEF
DEFG
EFGHI
HIJK
```

---

# 14. Complexity Analysis

Let:

- `n` = number of input strings
- `L` = maximum length of a string before merging

The simple implementation repeatedly compares pairs and computes their overlap.

## Finding pairs

If there are `n` strings, there are approximately:

```text
n × n
```

ordered pairs.

Therefore, one greedy iteration examines:

```text
O(n²)
```

pairs.

There are approximately:

```text
n - 1
```

merging iterations.

So pair selection alone contributes:

```text
O(n³)
```

pair checks.

## Cost of calculating an overlap

The program compares characters while checking possible overlap lengths.

Also, merged strings can become longer as the algorithm proceeds. In the worst case, a current string can have length proportional to:

```text
O(nL)
```

Therefore, for this simple direct string implementation, a conservative worst-case bound is:

```text
O(n⁴L)
```

where `L` is the maximum original input-string length.

For a lab-level discussion where string length is treated as bounded or constant, this is often simplified to:

```text
O(n⁴)
```

The important point is that the algorithm performs a cubic number of pair-selection checks, with additional cost for character comparisons and growing merged strings.

## Best Case

The exact best-case complexity depends on the input and the amount of overlap.

Even when overlaps are easy to find, the program still scans pairs in each iteration.

Therefore, a practical upper-level analysis is still polynomial and dominated by the repeated pair and overlap checks.

## Average Case

For general strings, the average running time depends on:

- number of strings,
- string lengths,
- amount of overlap,
- number of successful overlap comparisons.

A safe asymptotic bound for this implementation is:

```text
O(n⁴L)
```

## Space Complexity

The program stores the current strings and temporary merged strings.

The total size can grow to approximately `O(nL)` characters.

Therefore:

```text
Space = O(nL)
```

apart from small auxiliary variables.

---

# 15. Complexity Graph

The generated graph shows the theoretical worst-case growth with input size `n`.

The graph uses:

```text
O(n⁴)
```

as the displayed growth curve, assuming the maximum original string length `L` is fixed.

This is a **theoretical complexity curve**, not measured execution-time data.

Generated file:

`greedy_superstring_complexity.png`

---

# 16. Advantages

1. Very simple greedy idea.
2. Easy to implement in C.
3. Easy to understand for a DAA lab.
4. Usually produces a reasonably compact superstring.
5. Does not require exponential dynamic programming.
6. Demonstrates an important greedy strategy: maximize immediate benefit.
7. Useful for understanding approximation algorithms.

---

# 17. Disadvantages / Limitations

1. It does not always produce the shortest possible superstring.
2. A locally maximum overlap may lead to a poor later choice.
3. The exact Shortest Common Superstring problem is computationally difficult.
4. The simple implementation can become expensive for many long strings.
5. Tie-breaking can affect the result.
6. The classic claim that greedy is always a 2-approximation is not supported by the September 2026 arXiv counterexample as stated in that preprint.

---

# 18. Greedy vs Brute Force

### Greedy

Chooses the pair with the maximum overlap at every step.

Advantages:

- Simple
- Fast compared with exhaustive search
- Easy to implement

Disadvantage:

- May miss the global optimum.

### Brute Force

Tries many possible arrangements/orders.

Advantages:

- Can find the exact optimum for small instances.

Disadvantages:

- Becomes extremely expensive as `n` increases.

Therefore, greedy is much more practical for a DAA lab demonstration.

---

# 19. Viva Preparation

### Q1. What is the Shortest Common Superstring problem?

**Answer:** We need to find the shortest string that contains every given string as a substring.

### Q2. What is the main idea of the greedy algorithm?

**Answer:** Repeatedly select the two strings having the maximum overlap and merge them.

### Q3. What is overlap?

**Answer:** Overlap is the number of characters at the end of one string that are equal to the beginning of another string.

### Q4. Give an example of overlap.

**Answer:**

```text
ABCDEF
DEFG
```

The overlap is `DEF`, so the overlap length is 3.

### Q5. Why do we choose maximum overlap?

**Answer:** A larger overlap means fewer new characters have to be added during merging.

### Q6. Why do we remove contained strings?

**Answer:** If one string is already inside another, it is already covered and does not need a separate merge.

### Q7. Does greedy always produce the optimal shortest superstring?

**Answer:** No. It makes locally best choices, which do not necessarily give the globally best solution.

### Q8. What is the main limitation of the greedy approach?

**Answer:** A maximum overlap chosen now may prevent better overlaps in later steps.

### Q9. What is the time complexity of this implementation?

**Answer:** A conservative worst-case bound is `O(n⁴L)`, where `n` is the number of strings and `L` is the maximum original string length.

### Q10. Why is there an `n²` factor?

**Answer:** In each iteration, the program checks every pair of strings.

### Q11. Why is there another factor of `n`?

**Answer:** There are approximately `n-1` merging iterations.

### Q12. What is the space complexity?

**Answer:** Approximately `O(nL)` for storing the strings and merged strings.

### Q13. What does `strstr()` do?

**Answer:** `strstr()` checks whether one string occurs inside another string.

### Q14. Why is `getchar()` used after `scanf()`?

**Answer:** `scanf()` leaves the newline character in the input buffer. `getchar()` consumes it before `fgets()` is used.

### Q15. What happens when two pairs have the same maximum overlap?

**Answer:** The program keeps the first maximum pair it encounters. A different tie-breaking rule can sometimes produce a different greedy superstring.

---

# 20. C-Code-Specific Viva Questions

### Q1. Why do we use `strcpy()`?

**Answer:** It copies one string into another character array.

### Q2. Why do we use `strcat()`?

**Answer:** It appends one string to the end of another string.

### Q3. Why is `bestOverlap` initialized to `-1`?

**Answer:** So that even an overlap of zero is greater than the initial value and can become the best pair.

### Q4. Why do we shift strings in `removeString()`?

**Answer:** C arrays have fixed positions, so after removing an element, the remaining strings are shifted left to fill the empty position.

### Q5. Why are functions used instead of writing everything inside `main()`?

**Answer:** Functions divide the program into smaller logical parts, making the code easier to understand, test, and explain in a viva.

---

# 21. How to Compile and Run

Using GCC:

```bash
gcc greedy_superstring.c -o greedy_superstring
```

Run:

```bash
./greedy_superstring
```

On Windows using MinGW:

```bash
gcc greedy_superstring.c -o greedy_superstring.exe
greedy_superstring.exe
```

---

# 22. Teacher-Explanation Version

If the teacher asks:

> "Explain your algorithm and code."

You can say:

> "This problem asks us to construct one string that contains all the given strings. Finding the exact shortest common superstring is difficult, so here we use the greedy approach. First, I remove strings that are already contained inside another string. Then, in every iteration, I check all pairs of strings and calculate their suffix-prefix overlap. I select the pair with the maximum overlap because that saves the maximum number of characters immediately. Then I merge those two strings and replace them with the merged string. I repeat this until only one string is left. The `findOverlap()` function calculates the maximum overlap, `mergeStrings()` combines the strings, and `removeString()` removes the old string from the array. This is a greedy heuristic, so the result is not guaranteed to be the exact shortest superstring. The simple implementation has a conservative worst-case time complexity of `O(n⁴L)` and uses approximately `O(nL)` space."

---

# 23. Conclusion

The Greedy Superstring algorithm is a useful example of a greedy strategy.

At every step it chooses the pair with the maximum overlap, because that gives the largest immediate reduction in the resulting string length.

Although this approach is simple and practical, it does not guarantee the exact shortest common superstring. It is therefore a good example for understanding both the usefulness and limitations of greedy algorithms.

---

## References / Further Reading

- Hiroki Shibata, **"Disproving the Greedy Superstring Conjecture"**, arXiv, September 2026.
- Nikolai Chukhin, Alexander S. Kulikov, Ivan Mihajlin, Alexander Smal, **"The Greedy Superstring Algorithm Achieves Ratio 2 for Strings of Length 6 Already"**, arXiv, August 2026.
- Maik Weinard and Georg Schnitger, **"On the Greedy Superstring Conjecture"**, SIAM Journal on Discrete Mathematics.
