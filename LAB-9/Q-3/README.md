# Minimum Refuelling Stops — Greedy Approach

## 1. Problem Statement

A vehicle must travel from distance `0` to a target distance `D`.

- The vehicle starts with `F` units of fuel.
- There are `n` fuel stations.
- Each station is represented by `(d, f)`:
  - `d` = distance of the station from the origin
  - `f` = fuel available at that station
- The vehicle uses 1 unit of fuel per unit distance.
- At a station, the vehicle may take all available fuel.
- Find the minimum number of refuelling stops needed to reach `D`.

### Important modelling assumption

The stations are given in increasing order of distance. A station can be used only after the vehicle can reach its distance.

---

## 2. Objective

To solve the minimum-refuelling-stops problem using a greedy strategy and analyze its time and space complexity.

---

## 3. About "Reverse Greedy"

The problem statement calls this **Minimum Initial Fuel (Reverse Greedy)**. A literal rule such as:

> "Start at `D` and repeatedly subtract the largest fuel amount."

is **not correct for arbitrary station positions**, because it can select a station that the vehicle could not actually reach.

For example, a station far away cannot help the vehicle reach that station in the first place.

Therefore, the lab implementation below uses the standard correct greedy invariant:

> Among all stations that are currently reachable, postpone the actual choice until necessary, and take the station with the maximum fuel.

This is the classic greedy solution to the minimum-refuelling-stops problem. It is simple, correct, and directly models the idea of choosing the most useful fuel source.

---

## 4. Basic Idea

Suppose the vehicle can currently reach distance `R`.

We look at all stations with:

```text
station.distance <= R
```

These stations are reachable.

If we still cannot reach the target, we must choose some station from the reachable stations.

The greedy choice is:

```text
Choose the reachable station having maximum fuel.
```

Why?

Because if we are going to make one more stop, taking the largest available amount of fuel increases our reachable distance as much as possible.

To efficiently find the largest fuel amount, we use a **max-heap**.

---

## 5. Why a Max-Heap?

A max-heap always keeps the largest value at the top.

For this problem:

```text
heap = fuel amounts of all currently reachable stations
```

The top of the heap is therefore:

```text
largest available fuel
```

Whenever we need another refuelling stop:

```text
fuel = maximum fuel from heap
```

and:

```text
reachable = reachable + fuel
```

---

## 6. Step-by-Step Algorithm

1. Read `D`, `F`, and the stations.
2. Set `reachable = F`.
3. Set `i = 0` and `stops = 0`.
4. Insert into the max-heap every station whose distance is at most `reachable`.
5. If `reachable >= D`, stop; the target is already reachable.
6. If the heap is empty, the target cannot be reached.
7. Remove the largest fuel amount from the heap.
8. Add that fuel to `reachable`.
9. Increase `stops`.
10. Repeat until the target is reachable or no useful station remains.
11. Print the minimum number of stops.

---

## 7. C Program

```c
#include <stdio.h>

struct Station {
    int distance;
    int fuel;
};

/* Insert a fuel amount into the max-heap */
void insertHeap(int heap[], int *size, int value) {
    int i;
    int parent;
    int temp;

    (*size)++;
    i = *size;

    heap[i] = value;

    while (i > 1) {
        parent = i / 2;

        if (heap[parent] >= heap[i]) {
            break;
        }

        temp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = temp;

        i = parent;
    }
}

/* Remove and return the largest fuel amount */
int removeMax(int heap[], int *size) {
    int maxValue;
    int i;
    int left;
    int right;
    int largest;
    int temp;

    maxValue = heap[1];

    heap[1] = heap[*size];
    (*size)--;

    i = 1;

    while (1) {
        left = 2 * i;
        right = 2 * i + 1;
        largest = i;

        if (left <= *size && heap[left] > heap[largest]) {
            largest = left;
        }

        if (right <= *size && heap[right] > heap[largest]) {
            largest = right;
        }

        if (largest == i) {
            break;
        }

        temp = heap[i];
        heap[i] = heap[largest];
        heap[largest] = temp;

        i = largest;
    }

    return maxValue;
}

/* Greedy algorithm */
int minimumStops(struct Station station[], int n, int D, int F) {
    int heap[100];
    int heapSize = 0;
    int reachable = F;
    int stops = 0;
    int i = 0;
    int fuel;

    while (reachable < D) {

        /* Add all stations that can currently be reached */
        while (i < n && station[i].distance <= reachable) {
            insertHeap(heap, &heapSize, station[i].fuel);
            i++;
        }

        /* No reachable station means the target is impossible */
        if (heapSize == 0) {
            return -1;
        }

        /* Take the largest available fuel */
        fuel = removeMax(heap, &heapSize);

        reachable = reachable + fuel;
        stops++;
    }

    return stops;
}

int main() {
    int D, F, n;
    int i, answer;
    struct Station station[100];

    printf("Enter target distance D: ");
    scanf("%d", &D);

    printf("Enter initial fuel F: ");
    scanf("%d", &F);

    printf("Enter number of stations: ");
    scanf("%d", &n);

    printf("Enter distance and fuel for each station:\n");

    for (i = 0; i < n; i++) {
        scanf("%d %d", &station[i].distance, &station[i].fuel);
    }

    answer = minimumStops(station, n, D, F);

    if (answer == -1) {
        printf("Target cannot be reached.\n");
    } else {
        printf("Minimum number of refuelling stops = %d\n", answer);
    }

    return 0;
}
```

---

## 8. Code Explanation

### `struct Station`

```c
struct Station {
    int distance;
    int fuel;
};
```

This stores information about one station.

- `distance` = location of the station.
- `fuel` = fuel available there.

---

### `insertHeap()`

```c
void insertHeap(int heap[], int *size, int value)
```

This inserts a fuel amount into the max-heap.

Parameters:

- `heap[]` = heap array.
- `size` = current number of heap elements.
- `value` = new fuel amount.

The `size` pointer is used because the function must change the actual heap size.

After insertion, the new value is moved upward until the max-heap property is restored.

---

### `removeMax()`

```c
int removeMax(int heap[], int *size)
```

This removes and returns the largest fuel amount.

In a max-heap, the largest value is always at:

```text
heap[1]
```

After removing it, the last element is moved to the root and moved downward until the heap property is restored.

---

### `minimumStops()`

This function contains the main greedy algorithm.

Important variables:

#### `reachable`

```c
int reachable = F;
```

It tells us the farthest distance the vehicle can currently reach.

Initially, the vehicle has only its starting fuel:

```text
reachable = F
```

When we take fuel:

```c
reachable = reachable + fuel;
```

---

#### `stops`

```c
int stops = 0;
```

Counts how many stations have actually been used.

---

#### `i`

```c
int i = 0;
```

Points to the next station that has not yet been inserted into the heap.

Because stations are sorted by distance, each station is inserted only once.

---

#### Adding reachable stations

```c
while (i < n && station[i].distance <= reachable)
```

If the station is within the current reachable distance, we can reach it.

Its fuel is inserted into the max-heap.

---

#### Checking impossibility

```c
if (heapSize == 0) {
    return -1;
}
```

If we cannot reach the target and there is no reachable station left to refuel from, the journey is impossible.

---

#### Greedy choice

```c
fuel = removeMax(heap, &heapSize);
```

The largest available fuel is selected.

Then:

```c
reachable = reachable + fuel;
stops++;
```

So one refuelling stop is used and the reachable distance increases as much as possible.

---

## 9. Sample Input

```text
Enter target distance D: 100
Enter initial fuel F: 10
Enter number of stations: 4
Enter distance and fuel for each station:
10 60
20 20
50 30
70 40
```

## 10. Sample Output

```text
Minimum number of refuelling stops = 2
```

---

## 11. Complete Dry Run

Input:

```text
D = 100
F = 10

Stations:

Distance    Fuel
10          60
20          20
50          30
70          40
```

### Initial state

```text
reachable = 10
stops = 0
heap = empty
```

### Iteration 1

Station `(10,60)` is reachable because:

```text
10 <= 10
```

Insert `60`.

Heap:

```text
[60]
```

The target is not reachable:

```text
10 < 100
```

Remove maximum:

```text
fuel = 60
```

Update:

```text
reachable = 10 + 60 = 70
stops = 1
```

### Iteration 2

Now:

```text
reachable = 70
```

Stations `(20,20)`, `(50,30)`, and `(70,40)` are reachable.

Heap contains:

```text
60 was already used
20
30
40
```

So the heap is effectively:

```text
[40, 20, 30]
```

Remove maximum:

```text
fuel = 40
```

Update:

```text
reachable = 70 + 40 = 110
stops = 2
```

Now:

```text
110 >= 100
```

Target reached.

### Dry-run table

| Step | Reachable before | Newly available stations | Largest fuel selected | Reachable after | Stops |
|---|---:|---|---:|---:|---:|
| Start | 10 | (10,60) | 60 | 70 | 1 |
| 2 | 70 | (20,20), (50,30), (70,40) | 40 | 110 | 2 |
| End | 110 | — | — | 110 | 2 |

Answer:

```text
Minimum stops = 2
```

---

## 12. Why Is the Greedy Choice Correct?

Suppose several stations are currently reachable.

If we need one more stop, choosing a station with less fuel cannot increase our reachable distance as much as choosing the station with more fuel.

Therefore, for the same number of stops, taking the largest available fuel gives the maximum possible reach.

This is why the max-heap greedy method works.

---

## 13. Complexity Analysis

Let `n` be the number of stations.

### Each station is inserted once

A station enters the heap only once.

Heap insertion takes:

```text
O(log n)
```

Therefore, all insertions take:

```text
O(n log n)
```

### Each selected station is removed once

There can be at most `n` refuelling stops.

Each `removeMax()` operation takes:

```text
O(log n)
```

Therefore:

```text
O(n log n)
```

for all removals.

### Total time

Combining the two:

```text
O(n log n) + O(n log n)
= O(n log n)
```

### Best case

If:

```text
F >= D
```

the target is already reachable, so no heap operation is needed.

The algorithm performs essentially constant work:

```text
Best case = O(1)
```

under the stated sorted-input representation.

### Average case

The heap operations are logarithmic and each station is processed once:

```text
Average case = O(n log n)
```

### Worst case

All stations may need to be inserted and many may be removed:

```text
Worst case = O(n log n)
```

### Space complexity

The heap can contain up to `n` stations' fuel values:

```text
Space = O(n)
```

The station array itself also takes `O(n)` space.

Overall:

```text
Space complexity = O(n)
```

---

## 14. Complexity Graph

The generated PNG graph shows theoretical growth curves for:

- `O(n log n)` — the complexity of the heap-based greedy algorithm.
- `O(n²)` — a reference curve showing how much faster quadratic growth becomes for larger `n`.

The graph is based on mathematical growth functions, **not experimental execution times**.

File:

```text
minimum_initial_fuel_complexity.png
```

---

## 15. Advantages

1. Gives the minimum number of refuelling stops.
2. Greedy idea is easy to explain.
3. Max-heap efficiently finds the largest available fuel.
4. Each station is processed only once.
5. `O(n log n)` is efficient for large inputs.
6. The implementation can be written using basic C arrays without external libraries.

---

## 16. Disadvantages / Limitations

1. A max-heap must be implemented manually in C.
2. The input stations must be in increasing distance order for this simple implementation.
3. The sample program uses a fixed array size of 100.
4. The heap logic is slightly more difficult than a simple array-based greedy approach.
5. If the input is not sorted, an additional sorting step is required.

---

## 17. Viva Preparation

### Q1. What is the main idea?

We keep track of all stations that are currently reachable and choose the one having maximum fuel whenever another stop is required.

### Q2. Why is this greedy?

At every step, we choose the currently best option: the reachable station with the maximum fuel.

### Q3. Why do we use a max-heap?

A max-heap gives the largest available fuel in `O(1)` access to the root and removes it in `O(log n)`.

### Q4. What does `reachable` mean?

It is the farthest distance the vehicle can currently travel.

### Q5. When is a station inserted into the heap?

When:

```text
station.distance <= reachable
```

### Q6. When do we take fuel from the heap?

When the target is still unreachable and we need to increase the reachable distance.

### Q7. What happens if the heap is empty?

There is no reachable station from which we can obtain more fuel, so the target cannot be reached.

### Q8. What is the worst-case time complexity?

`O(n log n)` because each station is inserted at most once and removed at most once, with each heap operation taking `O(log n)`.

### Q9. What is the space complexity?

`O(n)` for the station array and heap.

### Q10. Why not simply choose the next station?

Because the nearest station may have very little fuel. A farther but already reachable station may provide much more fuel and reduce the total number of stops.

### Q11. What is the brute-force alternative?

We could try different combinations of stations, but the number of combinations can grow exponentially.

### Q12. What is the main advantage of greedy?

It avoids checking every possible combination and makes a locally optimal choice that leads to a globally minimum number of stops.

### Q13. What is a max-heap?

A binary heap in which every parent is greater than or equal to its children. Therefore, the largest element is at the root.

### Q14. Why is `heap[1]` the maximum?

This implementation uses a 1-based heap. In a max-heap, the root at index 1 contains the maximum value.

### Q15. What does `*size` mean in the heap functions?

It is a pointer to the actual heap size so that the function can change the size stored in `minimumStops()`.

### C-code questions

### Q16. Why is `struct Station` used?

It keeps distance and fuel together for each station.

### Q17. Why is `i` increased only when a station is inserted?

Because each station is processed once. Once it is put into the heap, there is no need to insert it again.

### Q18. Why do we use `while` instead of `if` to add stations?

Several stations may become reachable after one refuelling operation, so all newly reachable stations must be inserted.

### Q19. Why does `removeMax()` move the last heap element to the root?

After deleting the root, moving the last element to the root keeps the heap as a complete binary tree. We then restore the heap property by moving that element downward.

### Q20. Why does `insertHeap()` move an element upward?

The new element is placed at the end. If it is greater than its parent, it must move upward to maintain the max-heap property.

---

## 18. How to Compile and Run

Save the program as:

```text
minimum_initial_fuel.c
```

Compile with GCC:

```bash
gcc minimum_initial_fuel.c -o minimum_initial_fuel
```

Run:

```bash
./minimum_initial_fuel
```

On Windows:

```bash
gcc minimum_initial_fuel.c -o minimum_initial_fuel.exe
minimum_initial_fuel.exe
```

---

## 19. Teacher-Explanation Version

If the teacher asks:

> "Explain your algorithm and code."

You can say:

> "Sir/Ma'am, the problem is to reach a target distance using the minimum number of refuelling stops. I use a greedy approach with a max-heap. I maintain a variable called `reachable`, which tells me the farthest distance I can currently reach. I insert into the max-heap all stations whose distance is within this reachable range. If the target is still not reachable, I take the largest fuel value from the heap. This is the greedy choice because among all stations I can already reach, taking the maximum fuel increases my reachable distance as much as possible with one stop.
>
> I repeat this until I reach the target. If the heap becomes empty before reaching the target, then the journey is impossible. Each station is inserted at most once and removed at most once, and each heap operation takes `O(log n)`, so the overall time complexity is `O(n log n)` and the space complexity is `O(n)`."

---

## 20. Conclusion

The minimum-refuelling-stops problem can be solved efficiently using a greedy strategy with a max-heap.

The key idea is simple:

> **Among all currently reachable stations, use the station with the maximum fuel when another refuelling stop is necessary.**

This gives an `O(n log n)` implementation and is substantially more efficient than trying every possible combination of stations.
