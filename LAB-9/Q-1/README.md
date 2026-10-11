# Fractional Knapsack with Deterioration Rate

## 1. Problem Title

**Fractional Knapsack with Deterioration Rate**

## 2. Problem Statement

We have `n` items. Each item has:

- Base value `v[i]`
- Weight `w[i]`
- Decay rate `lambda[i] > 0`

If item `i` is consumed at time `t`, its effective value density is:

```text
(v[i] / w[i]) - lambda[i] * t
```

The knapsack has capacity `W`.

The objective is to determine:

1. The best scheduling order of the items.
2. How much of each item should be taken.
3. The maximum total value.

### Important modelling assumption

The original statement does not completely specify how time is assigned. For this lab solution, we use the following clear discrete-time model:

- There are `n` time slots: `1, 2, ..., n`.
- A scheduling order assigns one item to each time slot.
- An item may be taken fully, partially, or not taken.
- If an item is assigned to time `t`, its density is calculated using that `t`.
- For a fixed schedule, fractional knapsack is used to choose the quantities.
- We try every possible schedule, so the best schedule is guaranteed to be found for this model.

This assumption is important because different definitions of time can lead to different algorithms.

---

## 3. Objective

The objective is to maximize:

```text
Sum of (weight taken from item i) *
    [(v[i] / w[i]) - lambda[i] * t[i]]
```

subject to:

```text
Sum of weight taken <= W
```

and

```text
0 <= weight taken from item i <= w[i]
```

---

## 4. Possible Approaches

### Approach 1: Simple Greedy Sorting

We could try sorting items by:

- base value density,
- decay rate,
- or `weight * decay rate`.

However, because the time assigned to an item changes its effective density, and the fractional choice also changes the result, one fixed sorting rule is not guaranteed to find the global optimum.

### Approach 2: Dynamic Programming

Normal 0/1 knapsack DP does not directly fit because the items are fractional and the schedule is also part of the decision.

### Approach 3: Exhaustive Scheduling + Fractional Knapsack

We try every possible scheduling order.

For each order:

1. Calculate every item's effective density at its scheduled time.
2. Apply fractional knapsack using these densities.
3. Calculate the total value.
4. Keep the best result.

This is the approach used in this program because it is simple to understand and guarantees the optimal answer for the stated discrete-time model.

---

## 5. Basic Idea

Suppose an order is:

```text
Item 2 -> Item 1 -> Item 3
```

Then:

- Item 2 is consumed at `t = 1`
- Item 1 is consumed at `t = 2`
- Item 3 is consumed at `t = 3`

Their effective densities become:

```text
Item 2: v2/w2 - lambda2 * 1
Item 1: v1/w1 - lambda1 * 2
Item 3: v3/w3 - lambda3 * 3
```

For this fixed order, the problem becomes an ordinary fractional knapsack problem.

We select the available positive-density items with the highest effective density first.

We repeat this for every possible order and keep the maximum value.

---

## 6. Step-by-Step Algorithm

### Step 1: Read input

Read:

- Number of items `n`
- Capacity `W`
- Value, weight, and decay rate of every item

### Step 2: Generate all schedules

Use recursion to generate every permutation of the `n` items.

There are:

```text
n!
```

possible orders.

### Step 3: Evaluate one order

For each item in the current order:

```text
density[i] = value[i] / weight[i] - decay[i] * time
```

where `time` is the item's position in the schedule.

### Step 4: Perform fractional selection

Repeatedly find the item with the largest positive effective density.

Take:

- the complete item if its weight fits, or
- only the remaining capacity if the complete item does not fit.

### Step 5: Compare results

If the total value of the current order is greater than the best value found so far, save:

- the order
- the fractions/weights taken
- the total value

### Step 6: Print the answer

Print the optimal order, fractional choices, and maximum total value.

---

## 7. C Program

```c
#include <stdio.h>
#include <stdlib.h>

#define MAX 10

int n;
double W;

double value[MAX], weight[MAX], decay[MAX];
int currentOrder[MAX], bestOrder[MAX];
int used[MAX];

double bestFraction[MAX];
double bestValue = -1.0;

/* Evaluate one complete scheduling order */
void evaluateOrder()
{
    double density[MAX];
    double fraction[MAX];
    double remaining = W;
    double totalValue = 0.0;
    int i, j, bestIndex;
    double bestDensity;

    /* Find the effective density of every item at its scheduled time */
    for (i = 0; i < n; i++)
    {
        int item = currentOrder[i];
        int time = i + 1;

        density[item] = value[item] / weight[item]
                        - decay[item] * time;
        fraction[item] = 0.0;
    }

    /*
       Fractional knapsack:
       repeatedly choose the remaining item with the largest
       positive effective density.
    */
    while (remaining > 1e-9)
    {
        bestIndex = -1;
        bestDensity = 0.0;

        for (i = 0; i < n; i++)
        {
            if (fraction[i] < weight[i] && density[i] > bestDensity)
            {
                bestDensity = density[i];
                bestIndex = i;
            }
        }

        if (bestIndex == -1)
            break;  /* No positive-value item is left */

        if (weight[bestIndex] <= remaining)
        {
            fraction[bestIndex] = weight[bestIndex];
            totalValue += weight[bestIndex] * density[bestIndex];
            remaining -= weight[bestIndex];
        }
        else
        {
            fraction[bestIndex] = remaining;
            totalValue += remaining * density[bestIndex];
            remaining = 0.0;
        }
    }

    /* Save the best solution found so far */
    if (totalValue > bestValue)
    {
        bestValue = totalValue;

        for (i = 0; i < n; i++)
            bestOrder[i] = currentOrder[i];

        for (i = 0; i < n; i++)
            bestFraction[i] = fraction[i];
    }
}

/* Generate all possible scheduling orders */
void generateOrders(int position)
{
    int i;

    if (position == n)
    {
        evaluateOrder();
        return;
    }

    for (i = 0; i < n; i++)
    {
        if (!used[i])
        {
            used[i] = 1;
            currentOrder[position] = i;

            generateOrders(position + 1);

            used[i] = 0;
        }
    }
}

int main()
{
    int i;

    printf("Enter number of items: ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX)
    {
        printf("Number of items must be between 1 and %d.\n", MAX);
        return 0;
    }

    printf("Enter knapsack capacity: ");
    scanf("%lf", &W);

    printf("Enter value, weight and decay rate for each item:\n");

    for (i = 0; i < n; i++)
    {
        printf("Item %d: ", i + 1);
        scanf("%lf %lf %lf", &value[i], &weight[i], &decay[i]);

        if (weight[i] <= 0 || decay[i] <= 0)
        {
            printf("Weight must be positive and decay rate must be positive.\n");
            return 0;
        }
    }

    for (i = 0; i < n; i++)
        used[i] = 0;

    generateOrders(0);

    printf("\nOptimal scheduling order:\n");

    for (i = 0; i < n; i++)
        printf("Time %d -> Item %d\n", i + 1, bestOrder[i] + 1);

    printf("\nOptimal fractional choices:\n");

    for (i = 0; i < n; i++)
    {
        if (bestFraction[i] > 1e-9)
        {
            double fractionTaken = bestFraction[i] / weight[i];
            printf("Item %d: weight taken = %.2f, fraction = %.2f\n",
                   i + 1, bestFraction[i], fractionTaken);
        }
    }

    printf("\nMaximum total value = %.2f\n", bestValue);

    return 0;
}

```

---

## 8. Important Functions

### `evaluateOrder()`

**Purpose:** Evaluates one complete scheduling order.

**Parameters:** None. It uses the global `currentOrder` array.

**What it does:**

1. Calculates effective density for every item.
2. Performs fractional knapsack selection.
3. Calculates the total value.
4. Saves the solution if it is better than the current best.

---

### `generateOrders(int position)`

**Purpose:** Generates every possible permutation of items.

**Parameter:**

- `position` = the position currently being filled in the schedule.

**How it works:**

If all positions are filled, it calls:

```c
evaluateOrder();
```

Otherwise, it tries every unused item at the current position and recursively fills the next position.

---

### `main()`

**Purpose:** Reads input, starts the recursive permutation generation, and prints the final answer.

---

## 9. Important Variables

| Variable | Meaning |
|---|---|
| `n` | Number of items |
| `W` | Knapsack capacity |
| `value[i]` | Base value of item `i` |
| `weight[i]` | Weight of item `i` |
| `decay[i]` | Decay rate of item `i` |
| `currentOrder[i]` | Current schedule being tested |
| `bestOrder[i]` | Best schedule found |
| `used[i]` | Tells whether an item is already used in the current permutation |
| `bestFraction[i]` | Weight of item `i` taken in the best solution |
| `bestValue` | Maximum value found so far |
| `density[i]` | Effective value density at the assigned time |
| `remaining` | Remaining knapsack capacity |

---

## 10. Sample Input

```text
Enter number of items: 3
Enter knapsack capacity: 5
Enter value, weight and decay rate for each item:
Item 1: 60 4 1
Item 2: 100 5 2
Item 3: 120 6 1
```

## 11. Sample Output

One optimal result for this input is:

```text
Optimal scheduling order:
Time 1 -> Item 3
Time 2 -> Item 1
Time 3 -> Item 2

Optimal fractional choices:
Item 3: weight taken = 5.00, fraction = 0.83

Maximum total value = 95.00
```

**Note:** The exact best result depends on the input. The program checks all schedules rather than assuming that this sample order is always optimal.

---

## 12. Dry Run

Consider:

```text
n = 3
W = 5

Item 1: v = 60,  w = 4, lambda = 1
Item 2: v = 100, w = 5, lambda = 2
Item 3: v = 120, w = 6, lambda = 1
```

Suppose one schedule is:

```text
Item 3 -> Item 1 -> Item 2
```

### Effective densities

| Time | Item | Base density `v/w` | Decay | Effective density |
|---:|---:|---:|---:|---:|
| 1 | 3 | 20 | 1 | 19 |
| 2 | 1 | 15 | 2 | 13 |
| 3 | 2 | 20 | 6 | 14 |

So the densities are:

```text
Item 3 = 19
Item 2 = 14
Item 1 = 13
```

### Fractional selection

Capacity = `5`.

Item 3 has weight `6`, so the complete item cannot fit.

We take:

```text
5 / 6 = 0.8333
```

of Item 3.

Value obtained:

```text
5 * 19 = 95
```

Capacity becomes zero.

Therefore:

```text
Total value = 95
```

The program performs this evaluation for every possible order.

For `n = 3`, there are:

```text
3! = 6
```

orders.

They are:

```text
1 2 3
1 3 2
2 1 3
2 3 1
3 1 2
3 2 1
```

The program evaluates each one and chooses the order producing the largest value.

---

## 13. Complexity Analysis

There are `n!` possible scheduling orders.

For every order:

1. Calculating effective densities takes `O(n)`.
2. Selecting fractional items is implemented by repeatedly scanning all `n` items.
3. In the worst case, this scan can be repeated `O(n)` times.

Therefore, evaluating one order takes:

```text
O(n²)
```

Since there are:

```text
n!
```

orders, the total time complexity is:

```text
O(n! * n²)
```

or:

```text
O(n² * n!)
```

### Best Case

The program still generates all permutations because it does not use pruning.

Therefore:

```text
Best-case = O(n² * n!)
```

### Average Case

Again, every permutation is evaluated:

```text
Average-case = O(n² * n!)
```

### Worst Case

The same complete enumeration is performed:

```text
Worst-case = O(n² * n!)
```

### Space Complexity

The recursion depth is `n`, and the arrays store `O(n)` elements.

Therefore:

```text
Space Complexity = O(n)
```

The program is intentionally limited to a small `MAX` value because factorial growth becomes very large quickly.

---

## 14. How the Complexity Was Derived

For each complete permutation:

```text
Generate one order        -> O(n)
Calculate densities       -> O(n)
Fractional selection      -> O(n²)
```

The dominant part is:

```text
O(n²)
```

There are:

```text
n!
```

orders.

So:

```text
O(n!) * O(n²)
= O(n² * n!)
```

---

## 15. Complexity Graph

The generated graph is:

**`fractional_knapsack_deterioration_complexity.png`**

It plots the theoretical growth:

```text
n² × n!
```

against input size `n`.

The y-axis is a **theoretical operation proxy**, not measured execution time. It represents the dominant asymptotic growth discussed above.

For example:

```text
n = 5  -> 5² × 5!  = 3,000
n = 7  -> 7² × 7!  = 246,960
n = 9  -> 9² × 9!  = 29,393,280 
```

The important point is that factorial growth becomes extremely large very quickly.

---

## 16. Advantages

1. It guarantees the optimal schedule for the stated discrete-time model.
2. It is easy to understand conceptually.
3. It uses a standard fractional-knapsack greedy step.
4. It demonstrates recursion and permutation generation.
5. It is suitable for validating small test cases.
6. The code avoids complicated data structures.

---

## 17. Disadvantages / Limitations

1. The algorithm is very slow for large `n`.
2. Factorial growth makes exhaustive scheduling impractical for large inputs.
3. The program uses a discrete-time model.
4. It assumes one scheduled item per time slot.
5. Different definitions of continuous consumption time may require a different mathematical formulation.
6. The program is intended mainly for small lab inputs and validation.

---

## 18. Why Not Use Only a Greedy Sorting Rule?

In ordinary fractional knapsack, we can sort by value density:

```text
value / weight
```

Here, however, density changes with time:

```text
value / weight - lambda * time
```

Therefore, moving an item to a later time changes its value.

An item with a high original density may become worse after significant deterioration.

Because both scheduling and fractional selection affect the answer, simply sorting once is not guaranteed to give the global optimum.

---

## 19. Viva Preparation

### Q1. What is the main idea of this algorithm?

We try every possible scheduling order. For each order, we calculate the time-dependent density and then solve the resulting fractional knapsack problem.

### Q2. Why did you choose exhaustive search?

Because the scheduling order affects the value, and a simple greedy ordering is not guaranteed to be optimal. Exhaustive search guarantees the optimal result for small inputs.

### Q3. How many schedules are possible for `n` items?

There are:

```text
n!
```

possible schedules.

### Q4. What is the time complexity?

The time complexity is:

```text
O(n² × n!)
```

because there are `n!` orders and each order takes `O(n²)` time to evaluate.

### Q5. What is the space complexity?

The extra space is:

```text
O(n)
```

because of the arrays and recursion depth.

### Q6. What is effective density?

It is:

```text
v[i] / w[i] - lambda[i] × t
```

It tells us the value obtained per unit weight at time `t`.

### Q7. Why is fractional knapsack used?

Because an item can be partially selected. If the remaining capacity is smaller than an item's weight, we can take only the required fraction.

### Q8. Why do we select the highest effective density first?

For a fixed schedule, this is the standard optimal greedy rule for fractional knapsack.

### Q9. What does `used[i]` mean?

It tells whether item `i` has already been placed in the current permutation.

### Q10. Why is recursion used?

Recursion makes it simple to generate all possible permutations by filling one schedule position at a time.

### Q11. What happens when `position == n`?

It means a complete scheduling order has been generated, so the program evaluates that order.

### Q12. Why can this algorithm not be used for very large `n`?

Because the number of permutations grows factorially:

```text
1!, 2!, 3!, ..., n!
```

### Q13. What is the difference between this and ordinary fractional knapsack?

Ordinary fractional knapsack has fixed value densities. Here, the density changes according to the item's scheduled time.

### Q14. What happens if the effective density becomes negative?

Taking that item would reduce the total value, so the program does not take it.

### Q15. Why do we use `double`?

Values, decay rates, densities, and fractional quantities can contain decimal values.

---

## 20. C-Code-Specific Viva Questions

### Q16. Why is `currentOrder` used?

It stores the scheduling permutation currently being tested.

### Q17. Why is `bestOrder` needed?

After checking many schedules, we need to remember which schedule produced the maximum value.

### Q18. Why do we reset `used[i] = 0` after recursion?

That allows the item to be used again in another branch of the permutation search.

### Q19. Why is `remaining` used?

It stores the unused knapsack capacity while performing fractional selection.

### Q20. Why do we check `density[i] > bestDensity`?

We want to choose the item with the largest positive effective density.

---

## 21. 1–2 Minute Teacher Explanation

> "My problem is fractional knapsack where every item has a decay rate, so its value density decreases with time. Because of this, the order in which items are scheduled affects their effective value.
>
> I used an exhaustive search approach because a simple greedy sorting rule is not guaranteed to give the optimal schedule. I generate all possible permutations of the items using recursion. For every permutation, the position of an item represents its consumption time.
>
> Then I calculate the effective density using `value/weight - decay*time`. For that fixed order, I solve the remaining problem as a fractional knapsack: I repeatedly choose the item having the highest positive effective density and take the required full or partial weight according to the remaining capacity.
>
> I calculate the total value for every possible order and store the order having the maximum value. Since there are `n!` permutations and evaluating one permutation takes `O(n²)`, the overall time complexity is `O(n² × n!)`. The extra space is `O(n)`.
>
> This method is good for validating small inputs because it guarantees the optimal answer, but it becomes slow for large `n` because of factorial growth."

---

## 22. How to Compile and Run

Save the program as:

```text
fractional_knapsack_deterioration.c
```

### GCC

```bash
gcc fractional_knapsack_deterioration.c -o knapsack
```

Run:

```bash
./knapsack
```

### Windows

```bash
gcc fractional_knapsack_deterioration.c -o knapsack.exe
knapsack.exe
```

---

## 23. Files in This Lab

1. `fractional_knapsack_deterioration.c` — C source code
2. `README.md` — complete explanation and lab documentation
3. `fractional_knapsack_deterioration_complexity.png` — theoretical complexity graph

---

## 24. Conclusion

The Fractional Knapsack with Deterioration Rate problem is more complicated than ordinary fractional knapsack because the value density changes with time.

The solution used here separates the problem into two parts:

1. Exhaustively generate possible schedules.
2. For every schedule, use the fractional-knapsack greedy method.

This provides an exact solution for small inputs under the stated discrete-time model. Its main limitation is the factorial number of schedules, giving an overall complexity of:

```text
O(n² × n!)
```

Therefore, it is mainly useful for DAA lab validation and small input sizes.
