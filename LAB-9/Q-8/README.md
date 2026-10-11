# Minimum Number of Meeting Rooms

## 1. Problem Title

**Minimum Number of Meeting Rooms**

## 2. Problem Statement

Given an array of meeting intervals `[start, end]`, find the minimum number of conference rooms required so that no two meetings that overlap are placed in the same room.

For example:

```text
Meetings:
[0, 30], [5, 10], [15, 20]
```

The first meeting overlaps with both other meetings, so at one point two meetings are running at the same time. Therefore, **2 rooms** are required.

---

## 3. Objective

The objective is to find the minimum number of rooms required to schedule all meetings without overlapping meetings in the same room.

---

## 4. Input Representation

We use two separate arrays:

- `start[]` stores all meeting start times.
- `end[]` stores all meeting end times.

For example:

```text
start = [0, 5, 15]
end   = [30, 10, 20]
```

This representation makes it easy to sort start and end times independently.

---

## 5. Algorithm / Approach

We use a **sorting + two-pointer / sweep-line approach**.

### Basic Idea

1. Store all start times in one array and all end times in another array.
2. Sort both arrays.
3. Use two pointers:
   - `i` points to the next meeting that starts.
   - `j` points to the meeting that ends earliest.
4. If the next meeting starts **before** the earliest current meeting ends, another room is needed.
5. Otherwise, a room becomes free, so we move the end pointer.
6. Keep track of the maximum number of rooms used at any time.
7. That maximum is the answer.

### Why This Approach Is Suitable

A brute-force approach could compare every pair of meetings, but that can take `O(n²)` time.

Sorting the start and end times lets us process all events in order. The sorting takes `O(n log n)` time and the final scan takes `O(n)`, giving an overall complexity of:

```text
O(n log n)
```

This is efficient and still simple enough to explain in a DAA lab viva.

---

## 6. Step-by-Step Algorithm

1. Read the number of meetings `n`.
2. Read all start times into `start[]`.
3. Read all end times into `end[]`.
4. Sort `start[]`.
5. Sort `end[]`.
6. Set:
   - `i = 0`
   - `j = 0`
   - `rooms = 0`
   - `maxRooms = 0`
7. Repeat while `i < n`:
   - If `start[i] < end[j]`:
     - A new meeting starts before an existing meeting ends.
     - Increase `rooms`.
     - Update `maxRooms`.
     - Move `i`.
   - Otherwise:
     - A meeting has ended.
     - Decrease `rooms`.
     - Move `j`.
8. Print `maxRooms`.

### Important Boundary Condition

We use:

```c
start[i] < end[j]
```

If:

```text
start[i] == end[j]
```

the previous meeting has already ended, so the same room can be reused.

---

## 7. C Program

```c
#include <stdio.h>

void sortArray(int arr[], int n)
{
    int i, j, temp;

    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int findMinimumRooms(int start[], int end[], int n)
{
    int i = 0;
    int j = 0;
    int rooms = 0;
    int maxRooms = 0;

    sortArray(start, n);
    sortArray(end, n);

    while (i < n)
    {
        if (start[i] < end[j])
        {
            rooms++;

            if (rooms > maxRooms)
            {
                maxRooms = rooms;
            }

            i++;
        }
        else
        {
            rooms--;
            j++;
        }
    }

    return maxRooms;
}

int main()
{
    int n, i;
    int start[100], end[100];

    printf("Enter number of meetings: ");
    scanf("%d", &n);

    printf("Enter start and end time of each meeting:\n");

    for (i = 0; i < n; i++)
    {
        scanf("%d %d", &start[i], &end[i]);
    }

    printf("Minimum number of rooms required = %d\n",
           findMinimumRooms(start, end, n));

    return 0;
}
```

---

## 8. Explanation of the Program

### Header File

```c
#include <stdio.h>
```

This includes the standard input/output library so that we can use `printf()` and `scanf()`.

---

### `sortArray()` Function

```c
void sortArray(int arr[], int n)
```

This function sorts an integer array in ascending order.

Parameters:

- `arr[]` → array to be sorted.
- `n` → number of elements.

It does not return a value because the array is modified directly.

The function uses **Bubble Sort**, which is beginner-friendly.

```c
for (i = 0; i < n - 1; i++)
```

This controls the number of passes.

```c
for (j = 0; j < n - i - 1; j++)
```

This compares neighboring elements.

```c
if (arr[j] > arr[j + 1])
```

If two neighboring elements are in the wrong order, they are swapped.

---

### `findMinimumRooms()` Function

```c
int findMinimumRooms(int start[], int end[], int n)
```

This is the main algorithm function.

It receives:

- `start[]` → meeting start times.
- `end[]` → meeting end times.
- `n` → number of meetings.

It returns the minimum number of rooms required.

---

### Variables

```c
int i = 0;
int j = 0;
```

- `i` points to the next start time.
- `j` points to the next end time.

```c
int rooms = 0;
```

This stores the number of rooms currently occupied.

```c
int maxRooms = 0;
```

This stores the maximum number of rooms occupied at any point.

---

### Sorting

```c
sortArray(start, n);
sortArray(end, n);
```

Both arrays are sorted so that events can be processed chronologically.

Example:

```text
Start: 10  0  5  15
End:   20  30 10  25

After sorting:

Start: 0  5  10  15
End:   10 20  25  30
```

---

### Main Comparison

```c
if (start[i] < end[j])
```

This asks:

> Does the next meeting start before the earliest current meeting ends?

If yes, another room is needed.

```c
rooms++;
i++;
```

We increase the number of occupied rooms and move to the next start time.

---

### Updating Maximum

```c
if (rooms > maxRooms)
{
    maxRooms = rooms;
}
```

We store the largest number of simultaneously occupied rooms.

That value is the final answer.

---

### When a Meeting Ends

```c
else
{
    rooms--;
    j++;
}
```

If the next start time is not before the next end time, a meeting has ended first.

Therefore:

- one room becomes free;
- `rooms` decreases;
- `j` moves to the next end time.

---

### `main()` Function

```c
int start[100], end[100];
```

These arrays store the start and end times.

```c
scanf("%d", &n);
```

Reads the number of meetings.

The loop reads every meeting:

```c
for (i = 0; i < n; i++)
{
    scanf("%d %d", &start[i], &end[i]);
}
```

Finally:

```c
findMinimumRooms(start, end, n)
```

calculates the answer and prints it.

---

# 9. Sample Input

```text
Enter number of meetings: 6
Enter start and end time of each meeting:
0 30
5 10
15 20
20 25
25 35
30 40
```

# 10. Sample Output

```text
Minimum number of rooms required = 2
```

---

# 11. Complete Dry Run

Consider:

```text
Meetings:
[0, 30]
[5, 10]
[15, 20]
```

### Step 1: Separate Arrays

```text
Start = [0, 5, 15]
End   = [30, 10, 20]
```

### Step 2: Sort

```text
Start = [0, 5, 15]
End   = [10, 20, 30]
```

Initial values:

```text
i = 0
j = 0
rooms = 0
maxRooms = 0
```

### Dry Run Table

| Step | `start[i]` | `end[j]` | Condition | Action | `rooms` | `maxRooms` |
|---|---:|---:|---|---|---:|---:|
| 1 | 0 | 10 | 0 < 10 | New room | 1 | 1 |
| 2 | 5 | 10 | 5 < 10 | New room | 2 | 2 |
| 3 | 15 | 10 | False | Room becomes free | 1 | 2 |
| 4 | 15 | 20 | 15 < 20 | New room | 2 | 2 |

Final:

```text
maxRooms = 2
```

Therefore:

```text
Minimum rooms required = 2
```

---

# 12. Understanding the Dry Run

At time `0`:

```text
Meeting 1 starts
Rooms = 1
```

At time `5`:

```text
Meeting 2 starts
Meeting 1 is still running
Rooms = 2
```

At time `10`:

```text
Meeting 2 ends
One room becomes free
Rooms = 1
```

At time `15`:

```text
Meeting 3 starts
Meeting 1 is still running
Rooms = 2
```

So the maximum number of rooms needed at any time is **2**.

---

# 13. Complexity Analysis

## Sorting Complexity

The program uses Bubble Sort.

For an array of `n` elements, Bubble Sort takes:

```text
O(n²)
```

in the worst and average cases.

Therefore, because we sort both `start[]` and `end[]` arrays:

```text
O(n²) + O(n²) = O(n²)
```

## Two-Pointer Scan

After sorting, the two-pointer loop processes the meetings in order.

Each pointer moves forward and never moves backward.

Therefore, the scan takes:

```text
O(n)
```

## Overall Time Complexity

The dominant part is Bubble Sort:

```text
O(n²) + O(n)
```

Therefore:

```text
Overall = O(n²)
```

### Best Case

With the Bubble Sort implementation shown above, the code does not use an early-exit flag. Therefore, it still performs the nested loops.

So the best-case time complexity of this exact implementation is:

```text
O(n²)
```

### Average Case

```text
O(n²)
```

### Worst Case

```text
O(n²)
```

### Space Complexity

The algorithm uses two arrays of size `n`:

```text
start[n]
end[n]
```

Therefore the extra input storage is:

```text
O(n)
```

Apart from these arrays, only a constant number of variables are used.

So the space complexity is:

```text
O(n)
```

---

# 14. Important Note About Complexity

The **meeting-room algorithm itself** is based on sorting plus a linear scan.

If an efficient sorting algorithm such as Merge Sort or Quick Sort is used, the standard overall complexity becomes:

```text
O(n log n)
```

However, the program above intentionally uses **Bubble Sort** because the requirement is for a simple, beginner-friendly DAA lab implementation.

Therefore, for this exact C program:

```text
Time Complexity = O(n²)
Space Complexity = O(n)
```

The generated complexity graph represents the exact `O(n²)` growth of the **provided C program**, because it uses Bubble Sort. It is a theoretical complexity curve, not fabricated experimental timing data.

---

# 15. Complexity Graph

The accompanying PNG graph plots:

```text
O(n log n)
```

against input size `n`.

It represents the standard efficient implementation where the sorting step is performed using an `O(n log n)` sorting algorithm.

File:

`minimum_meeting_rooms_complexity.png`

---

# 16. Advantages

1. Easy to understand.
2. Uses simple arrays.
3. Uses a simple two-pointer technique.
4. Avoids complicated data structures.
5. Efficient standard approach can achieve `O(n log n)`.
6. Easy to explain in a viva.
7. Correctly handles overlapping meetings.

---

# 17. Disadvantages / Limitations

1. The provided lab implementation uses Bubble Sort, so its time complexity is `O(n²)`.
2. Fixed arrays of size 100 are used.
3. The program assumes valid meeting intervals.
4. It does not dynamically allocate memory.
5. It only calculates the number of rooms; it does not assign particular meetings to particular rooms.

---

# 18. Viva Preparation

### Q1. What is the main idea of the algorithm?

We separate start and end times, sort both arrays, and use two pointers to count how many meetings are active at the same time.

### Q2. Why do we need the minimum number of rooms?

Because overlapping meetings cannot use the same room at the same time.

### Q3. Why are start and end times stored separately?

It allows us to sort them independently and process the events chronologically.

### Q4. What does `i` represent?

`i` points to the next meeting start time that has not been processed.

### Q5. What does `j` represent?

`j` points to the earliest meeting end time that has not been processed.

### Q6. When do we need a new room?

When:

```text
start[i] < end[j]
```

The new meeting starts before an existing meeting ends.

### Q7. Why is the condition not `<=`?

If the start time equals the end time, one meeting has already ended, so its room can be reused.

### Q8. What does `rooms` store?

It stores the number of rooms currently occupied.

### Q9. What does `maxRooms` store?

It stores the maximum number of simultaneously occupied rooms.

### Q10. What is the time complexity of this exact program?

`O(n²)` because Bubble Sort is used.

### Q11. What would be the complexity with an efficient `O(n log n)` sorting algorithm?

The total would be `O(n log n)` because sorting dominates the `O(n)` two-pointer scan.

### Q12. What is the space complexity?

`O(n)` because two arrays of size `n` are used.

### Q13. What is the advantage of the two-pointer technique?

It processes sorted start and end events in linear time after sorting.

### Q14. What happens if there are no overlapping meetings?

Only one room is required.

### Q15. What happens if all meetings overlap?

The number of rooms required equals the number of meetings.

---

# 19. C-Code-Specific Viva Questions

### Q16. Why is `sortArray()` declared as `void`?

Because it directly modifies the array and does not need to return a value.

### Q17. Why do we pass `start[]` and `end[]` to the function?

The function needs the meeting start and end times to calculate the number of rooms.

### Q18. Why are `i` and `j` initialized to zero?

C arrays start from index `0`, so both pointers initially point to the first element.

### Q19. Why do we use a temporary variable in `sortArray()`?

The `temp` variable is used to safely swap two array elements.

### Q20. Why do we use `rooms > maxRooms`?

To remember the largest number of rooms occupied at any point during the scan.

---

# 20. How to Compile and Run

Save the program as:

```text
meeting_rooms.c
```

Compile using GCC:

```bash
gcc meeting_rooms.c -o meeting_rooms
```

Run:

```bash
./meeting_rooms
```

On Windows using GCC/MinGW:

```bash
gcc meeting_rooms.c -o meeting_rooms.exe
meeting_rooms.exe
```

---

# 21. Teacher-Explanation Version

You can explain the program like this:

> "The problem is to find the minimum number of conference rooms needed for a set of meetings. The main idea is to find the maximum number of meetings happening at the same time.
>
> I store all start times in one array and all end times in another array. Then I sort both arrays. I use two pointers, `i` for start times and `j` for end times. If the next meeting starts before the earliest meeting ends, I need one more room, so I increase `rooms`. Otherwise, a meeting has ended, so I decrease `rooms` and move the end pointer.
>
> I keep the maximum value of `rooms` in `maxRooms`, because that is the maximum number of rooms needed simultaneously.
>
> In my lab implementation I use Bubble Sort, so the time complexity is `O(n²)`, while the two-pointer scan is `O(n)`. If an `O(n log n)` sorting algorithm is used, the standard efficient version has `O(n log n)` time complexity. The space complexity is `O(n)` because I use two arrays."

---

# 22. Conclusion

The Minimum Number of Meeting Rooms problem can be solved efficiently by sorting all start and end times and scanning them using two pointers.

The important idea is that the answer is the **maximum number of meetings active at the same time**.

The provided implementation is intentionally simple and suitable for a DAA lab and viva. Its exact complexity is `O(n²)` because Bubble Sort is used, while replacing the sorting method with an `O(n log n)` sort gives the standard `O(n log n)` solution.
