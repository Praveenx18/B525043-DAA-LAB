# Reorganise String with K-Distance Apart

## 1. Problem Statement

Given a string `S` and an integer `K`, rearrange the characters of `S` so that the same characters are at least `K` positions apart.

If such a rearrangement is not possible, return an empty string / report that it is impossible.

### Example

Input:
```text
S = aabbcc
K = 3
```

One valid output is:
```text
abcabc
```

The two occurrences of every character are 3 positions apart.

---

## 2. Objective

The objective is to rearrange the string while maintaining at least `K` distance between equal characters.

The program should:

- Count the frequency of each character.
- Select the most frequent available character first.
- Temporarily keep a character unavailable for the next `K-1` positions.
- Detect when no valid character is available.
- Print the rearranged string if possible.

---

## 3. Approach Used

We use a **Greedy approach with a Max Heap (Priority Queue)**.

The max heap always gives us the character having the highest remaining frequency.

After using a character, it cannot be used again until `K` positions have passed. A simple queue is used to store characters that are temporarily unavailable.

### Why this approach is suitable

A character with a high frequency is the hardest character to place. Therefore, placing the most frequent currently available character first is a natural greedy choice.

The waiting queue ensures that a character is not selected again too early.

---

## 4. Step-by-Step Algorithm

1. Count the frequency of every character.
2. Insert every character with non-zero frequency into a max heap.
3. Create a queue for characters that have been used and must wait.
4. Repeat until all characters are placed:
   - Move the character whose waiting period has ended back into the heap.
   - If the heap is empty while characters still remain, the arrangement is impossible.
   - Remove the character with the largest frequency from the heap.
   - Add it to the answer.
   - Decrease its frequency.
   - Put it into the waiting queue if it still has remaining occurrences.
5. If all characters are placed, print the resulting string.

---

## 5. C Program

```c
#include <stdio.h>
#include <string.h>

#define MAX 1000
#define CHAR_SET 256

typedef struct {
    char ch;
    int freq;
} Node;

typedef struct {
    Node data[CHAR_SET];
    int size;
} MaxHeap;

typedef struct {
    Node data[CHAR_SET];
    int front;
    int rear;
    int size;
} Queue;

/* ---------- Max Heap Functions ---------- */

void swap(Node *a, Node *b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MaxHeap *heap, int index) {
    int parent;

    while (index > 0) {
        parent = (index - 1) / 2;

        if (heap->data[parent].freq >= heap->data[index].freq)
            break;

        swap(&heap->data[parent], &heap->data[index]);
        index = parent;
    }
}

void heapifyDown(MaxHeap *heap, int index) {
    int left, right, largest;

    while (1) {
        left = 2 * index + 1;
        right = 2 * index + 2;
        largest = index;

        if (left < heap->size &&
            heap->data[left].freq > heap->data[largest].freq)
            largest = left;

        if (right < heap->size &&
            heap->data[right].freq > heap->data[largest].freq)
            largest = right;

        if (largest == index)
            break;

        swap(&heap->data[index], &heap->data[largest]);
        index = largest;
    }
}

void insertHeap(MaxHeap *heap, Node item) {
    heap->data[heap->size] = item;
    heap->size++;
    heapifyUp(heap, heap->size - 1);
}

Node removeMax(MaxHeap *heap) {
    Node maxItem = heap->data[0];

    heap->size--;
    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];
        heapifyDown(heap, 0);
    }

    return maxItem;
}

/* ---------- Queue Functions ---------- */

void enqueue(Queue *queue, Node item) {
    queue->data[queue->rear] = item;
    queue->rear = (queue->rear + 1) % CHAR_SET;
    queue->size++;
}

Node dequeue(Queue *queue) {
    Node item = queue->data[queue->front];

    queue->front = (queue->front + 1) % CHAR_SET;
    queue->size--;

    return item;
}

/* ---------- Main Algorithm ---------- */

void reorganizeString(char str[], int k) {
    int frequency[CHAR_SET] = {0};
    MaxHeap heap;
    Queue waitQueue;
    char result[MAX];

    int n = strlen(str);
    int i;
    int resultIndex = 0;

    heap.size = 0;
    waitQueue.front = 0;
    waitQueue.rear = 0;
    waitQueue.size = 0;

    /* Count frequency of every character */
    for (i = 0; i < n; i++)
        frequency[(unsigned char)str[i]]++;

    /* Put all characters into the max heap */
    for (i = 0; i < CHAR_SET; i++) {
        if (frequency[i] > 0) {
            Node item;
            item.ch = (char)i;
            item.freq = frequency[i];
            insertHeap(&heap, item);
        }
    }

    /*
       Each item in the queue is kept for K positions.
       releaseAt tells us when the character can be used again.
       Here we store releaseAt indirectly by using a fixed-size
       queue of size K in the main algorithm below.
    */

    /* For simplicity, use arrays for the waiting characters. */
    Node waitingChar[MAX];
    int releaseAt[MAX];
    int waitFront = 0;
    int waitRear = 0;

    while (resultIndex < n) {

        /* Release a character whose K-distance has been completed */
        while (waitFront < waitRear &&
               releaseAt[waitFront] <= resultIndex) {

            insertHeap(&heap, waitingChar[waitFront]);
            waitFront++;
        }

        /* No character is available */
        if (heap.size == 0) {
            printf("Rearrangement is not possible.\n");
            return;
        }

        /* Choose the character with maximum remaining frequency */
        Node current = removeMax(&heap);

        result[resultIndex] = current.ch;
        resultIndex++;

        current.freq--;

        /*
           The same character can be used again only after
           K positions have been filled.
        */
        if (current.freq > 0) {
            waitingChar[waitRear] = current;
            releaseAt[waitRear] = resultIndex + k - 1;
            waitRear++;
        }
    }

    result[resultIndex] = '\0';

    printf("Rearranged string: %s\n", result);
}

int main() {
    char str[MAX];
    int k;

    printf("Enter the string: ");
    scanf("%s", str);

    printf("Enter K: ");
    scanf("%d", &k);

    if (k <= 1) {
        printf("Rearranged string: %s\n", str);
    } else {
        reorganizeString(str, k);
    }

    return 0;
}
```

---

## 6. Important Code Explanation

### `Node`

```c
typedef struct {
    char ch;
    int freq;
} Node;
```

A `Node` stores:

- `ch` → the character.
- `freq` → how many times that character is still available.

For example:

```text
ch = 'a'
freq = 3
```

means that three `a` characters remain.

---

### `MaxHeap`

```c
typedef struct {
    Node data[CHAR_SET];
    int size;
} MaxHeap;
```

The max heap stores available characters.

The character with the highest frequency stays at the top.

This is important because the greedy algorithm always selects the most frequent available character.

---

### `Queue`

The queue temporarily stores characters that cannot be used immediately again.

For example, when `K = 3` and we place:

```text
a
```

we cannot immediately place another `a`.

The `a` waits until at least two other positions have been filled.

---

### `insertHeap()`

```c
void insertHeap(MaxHeap *heap, Node item)
```

Adds a character to the max heap.

It calls `heapifyUp()` to move the new element to its correct position.

---

### `removeMax()`

```c
Node removeMax(MaxHeap *heap)
```

Removes and returns the character with the largest frequency.

This is the main greedy operation.

---

### `heapifyUp()`

After inserting an element, this function moves it upward while its frequency is greater than its parent's frequency.

---

### `heapifyDown()`

After removing the maximum element, the last element is moved to the root.

`heapifyDown()` restores the max-heap property.

---

### Frequency counting

```c
for (i = 0; i < n; i++)
    frequency[(unsigned char)str[i]]++;
```

This loop counts how many times every character occurs.

For:

```text
aabbcc
```

we get:

```text
a = 2
b = 2
c = 2
```

---

### Main greedy selection

```c
Node current = removeMax(&heap);
```

The most frequent available character is selected.

Then:

```c
result[resultIndex] = current.ch;
resultIndex++;
```

places it into the answer.

Its frequency is reduced:

```c
current.freq--;
```

---

### Waiting period

```c
releaseAt[waitRear] = resultIndex + k - 1;
```

This determines when the character can become available again.

The character is therefore prevented from being selected too early.

---

## 7. Dry Run

Consider:

```text
S = aabbcc
K = 3
```

Initial frequencies:

| Character | Frequency |
|---|---:|
| a | 2 |
| b | 2 |
| c | 2 |

The max heap contains:

```text
a(2), b(2), c(2)
```

One possible sequence of selections is:

| Position | Selected | Remaining Frequency | Waiting Character | Result |
|---:|---|---:|---|---|
| 0 | a | a=1 | a | a |
| 1 | b | b=1 | b | ab |
| 2 | c | c=1 | c | abc |
| 3 | a | a=0 | b,c | abca |
| 4 | b | b=0 | c | abcab |
| 5 | c | c=0 | — | abcabc |

Final answer:

```text
abcabc
```

Check the distance:

- `a` occurs at positions 0 and 3 → distance 3.
- `b` occurs at positions 1 and 4 → distance 3.
- `c` occurs at positions 2 and 5 → distance 3.

Therefore, the answer is valid.

### Impossible example

```text
S = aaabc
K = 3
```

There are three `a`s but only two other characters.

The `a`s would need enough positions between them. At some point the heap becomes empty while an `a` is still waiting, so the program reports:

```text
Rearrangement is not possible.
```

---

## 8. Sample Input

```text
Enter the string: aabbcc
Enter K: 3
```

## 9. Sample Output

```text
Rearranged string: abcabc
```

---

## 10. Complexity Analysis

Let:

- `n` = length of the string.
- `σ` = number of distinct characters.

### Frequency counting

We scan the string once:

```text
O(n)
```

### Building the heap

At most `σ` different characters are inserted.

Each insertion costs:

```text
O(log σ)
```

So this costs:

```text
O(σ log σ)
```

### Main loop

There are `n` output positions.

For each position, we perform heap removal/insertion operations. Each heap operation takes:

```text
O(log σ)
```

Therefore:

```text
O(n log σ)
```

Overall:

```text
O(n + σ log σ + n log σ)
```

which simplifies to:

```text
O(n log σ)
```

Since `σ <= 256` for the character set used by this program, `σ` is bounded by a constant. Therefore, for this particular implementation the practical asymptotic bound can be viewed as:

```text
O(n)
```

with a constant-factor heap overhead.

### Best case

```text
O(n log σ)
```

The algorithm still processes the characters that must be placed.

### Average case

```text
O(n log σ)
```

The same type of heap operations are required.

### Worst case

```text
O(n log σ)
```

If `σ` is treated as variable.

For the fixed 256-character set:

```text
O(n)
```

asymptotically.

### Space Complexity

The frequency array, heap, waiting arrays and result string require:

```text
O(n + σ)
```

space.

Because `σ <= 256`, this is effectively:

```text
O(n)
```

for the given implementation.

---

## 11. Complexity Graph

The accompanying PNG graph compares:

- `O(n)`
- `O(n log σ)` with `σ = 256`

The second curve matches the heap-based complexity of this implementation. Since the character set is fixed at at most 256 characters, `log₂(256) = 8`, so `O(n log σ)` is linear in `n` up to a constant factor.

The graph is theoretical; it does not represent measured execution times.

File:

`complexity_graph_reorganise_string_k_distance.png`

---

## 12. Advantages

1. Uses a greedy strategy that is easy to understand.
2. The max heap always selects the most frequent available character.
3. The waiting mechanism directly enforces the K-distance rule.
4. More efficient than checking every possible permutation.
5. Works well for large strings.

---

## 13. Disadvantages / Limitations

1. The C implementation is longer because C does not have a built-in priority queue.
2. The heap and waiting structures require additional memory.
3. The implementation assumes a character set of at most 256 characters.
4. The exact implementation uses arrays of fixed maximum size (`MAX = 1000`).

---

## 14. Viva Preparation

### Q1. What is the main idea of the algorithm?

We always choose the currently available character having the highest frequency and then temporarily prevent it from being selected again until K positions have passed.

### Q2. Why did you choose a greedy approach?

The most frequent character is the hardest one to place. Placing it whenever it is available helps avoid getting stuck later.

### Q3. Why is a max heap used?

A max heap allows us to quickly obtain the character with the highest remaining frequency.

### Q4. What does K mean?

K is the minimum distance required between two occurrences of the same character.

### Q5. What happens when K = 1?

There is no restriction because the same character can appear next to itself. Therefore, the original string is already valid.

### Q6. When is rearrangement impossible?

If characters are still left to place but the heap becomes empty because all remaining characters are waiting, no valid arrangement is possible.

### Q7. What is the time complexity?

`O(n log σ)`, where `σ` is the number of distinct characters. For a fixed 256-character alphabet, this behaves as `O(n)`.

### Q8. What is the space complexity?

`O(n + σ)`, mainly because of the result and waiting arrays.

### Q9. Why not generate all permutations?

Generating permutations takes extremely large time, approximately factorial growth, and is impractical even for moderately sized strings.

### Q10. What does `frequency[]` store?

It stores how many times each character occurs in the input string.

### Q11. Why is `removeMax()` used?

It returns the available character with the largest remaining frequency, which is the greedy choice.

### Q12. Why is `heapifyUp()` needed?

It restores the max-heap property after inserting a new character.

### Q13. Why is `heapifyDown()` needed?

It restores the max-heap property after removing the maximum element.

### Q14. What does `resultIndex` represent?

It represents the next position where a character will be placed in the result.

### Q15. What is the advantage over a simple brute-force approach?

The greedy heap approach avoids exploring all permutations and makes a locally best choice at every step, giving much better performance.

---

## 15. C-Code Specific Viva Questions

### Q1. Why do we use a structure called `Node`?

Because every character needs two pieces of information: the character itself and its remaining frequency.

### Q2. Why is `frequency[(unsigned char)str[i]]++` used?

The character's ASCII value is used as an index into the frequency array.

### Q3. Why do we use pointers in `insertHeap()`?

The function must modify the actual heap, so a pointer to the heap is passed.

### Q4. Why is `result[resultIndex] = '\0'` required?

C strings end with the null character `'\0'`. It tells `printf("%s")` where the string ends.

### Q5. Why do we check `heap.size == 0`?

If the heap is empty but some characters are still left, no character can legally be placed at the current position, so rearrangement is impossible.

---

## 16. How to Compile and Run

### Using GCC

Save the program as:

```text
reorganise_string.c
```

Compile:

```bash
gcc reorganise_string.c -o reorganise_string
```

Run:

```bash
./reorganise_string
```

On Windows:

```bash
reorganise_string.exe
```

---

## 17. Teacher-Explanation Version

You can explain the program like this:

> "The problem is to rearrange a string so that two equal characters have at least K distance between them. I use a greedy approach with a max heap. First, I count the frequency of every character and put the characters into the max heap according to their frequency. At every position, I select the character with the highest remaining frequency. After using a character, I temporarily keep it in a waiting list, so it cannot be selected again before K positions are completed. When its waiting period is over, I put it back into the heap. If the heap becomes empty while some characters are still left, then rearrangement is impossible. The main heap operations take O(log σ), so the overall complexity is O(n log σ), where σ is the number of distinct characters. Since this program uses a fixed 256-character set, it behaves approximately as O(n). The extra space used is O(n + σ)."

---

## 18. Conclusion

The greedy max-heap method provides an efficient way to reorganise a string while maintaining a minimum distance of `K` between equal characters.

The most important ideas to remember for the viva are:

1. Count character frequencies.
2. Always choose the most frequent currently available character.
3. Put used characters into a waiting list.
4. Release them only after K positions.
5. If no character is available before the string is complete, the arrangement is impossible.
