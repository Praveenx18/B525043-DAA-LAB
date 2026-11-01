# Bidirectional BFS for Shortest Transformation Sequence in a Word Graph

## 1. Problem statement

We are given a dictionary of `N` unique lowercase words, all having the same length `L`. We are also given a starting word `A` and a target word `B`. One mutation changes exactly one character, and every intermediate word must belong to the dictionary. The starting word `A` is allowed to be outside the dictionary, but `B` must be in it.

Find the minimum number of mutations needed to transform `A` into `B`. If no valid sequence exists, print `-1`.

## 2. Objective

Use Bidirectional Breadth-First Search (BFS) to find the shortest transformation length.

## 3. Algorithm and why it is suitable

Think of every word as a vertex in a graph. Two words are connected when they differ at exactly one character. Each mutation has cost 1, so the graph is unweighted and BFS finds a shortest path.

Ordinary BFS searches from `A` only. Bidirectional BFS searches from `A` and `B` at the same time and stops when the two searches meet. It often explores fewer words, especially when many words are reachable at each step.

This implementation sorts the dictionary and uses binary search to check whether a generated word exists. It does not need to build every graph edge in advance.

## 4. Step-by-step algorithm

1. Read the dictionary, starting word `A`, and target word `B`.
2. Check that all words have the same length and sort the dictionary.
3. Use binary search to check that `B` exists. If not, print `-1`.
4. If `A` equals `B`, print `0`.
5. Initialize two BFS queues and two distance arrays. Start the first search at `A` and the second at `B`.
6. Choose the search with the smaller current frontier and expand one complete BFS level.
7. For each word, change each character to each other lowercase letter. Keep a generated word only if it is `A` or appears in the dictionary.
8. Record its distance. If the other search has already visited that word, add the two distances and return the result.
9. If either search runs out of words before meeting the other, print `-1`.

## 5. C program

Save the following as `solution.c`.

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXN 100005
#define MAXL 11

char words[MAXN][MAXL];
char startWord[MAXL];
int wordLength;
int dictionarySize;

int distanceA[MAXN];
int distanceB[MAXN];
int queueA[MAXN];
int queueB[MAXN];

int compareWords(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char *)b);
}

int findWord(char word[])
{
    int low = 0, high = dictionarySize - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        int result = strcmp(words[mid], word);

        if (result == 0)
            return mid;
        else if (result < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return -1;
}

int expandOneLevel(int queue[], int *head, int *tail,
                   int ownDistance[], int otherDistance[])
{
    int levelEnd = *tail;

    while (*head < levelEnd)
    {
        int current = queue[*head];
        (*head)++;

        char changedWord[MAXL];
        char *currentWord;

        if (current == dictionarySize)
            currentWord = startWord;
        else
            currentWord = words[current];

        strcpy(changedWord, currentWord);

        for (int i = 0; i < wordLength; i++)
        {
            char originalCharacter = changedWord[i];

            for (char ch = 'a'; ch <= 'z'; ch++)
            {
                if (ch == originalCharacter)
                    continue;

                changedWord[i] = ch;
                int next;

                if (strcmp(changedWord, startWord) == 0)
                    next = dictionarySize;
                else
                    next = findWord(changedWord);

                if (next != -1)
                {
                    if (ownDistance[next] == -1)
                    {
                        ownDistance[next] = ownDistance[current] + 1;
                        queue[*tail] = next;
                        (*tail)++;
                    }

                    if (otherDistance[next] != -1)
                        return ownDistance[next] + otherDistance[next];
                }
            }

            changedWord[i] = originalCharacter;
        }
    }

    return -1;
}

int bidirectionalBFS(int startIndex, int targetIndex)
{
    for (int i = 0; i <= dictionarySize; i++)
    {
        distanceA[i] = -1;
        distanceB[i] = -1;
    }

    int headA = 0, tailA = 0;
    int headB = 0, tailB = 0;

    distanceA[startIndex] = 0;
    distanceB[targetIndex] = 0;
    queueA[tailA++] = startIndex;
    queueB[tailB++] = targetIndex;

    while (headA < tailA && headB < tailB)
    {
        int answer;

        if (tailA - headA <= tailB - headB)
            answer = expandOneLevel(queueA, &headA, &tailA,
                                    distanceA, distanceB);
        else
            answer = expandOneLevel(queueB, &headB, &tailB,
                                    distanceB, distanceA);

        if (answer != -1)
            return answer;
    }

    return -1;
}

int main(void)
{
    char targetWord[MAXL];

    printf("Enter number of dictionary words: ");
    if (scanf("%d", &dictionarySize) != 1 ||
        dictionarySize < 1 || dictionarySize >= MAXN)
    {
        printf("Invalid dictionary size.\n");
        return 1;
    }

    printf("Enter the dictionary words (same length, lowercase):\n");
    for (int i = 0; i < dictionarySize; i++)
    {
        if (scanf("%10s", words[i]) != 1)
            return 1;
    }

    printf("Enter starting word A: ");
    if (scanf("%10s", startWord) != 1)
        return 1;

    printf("Enter target word B: ");
    if (scanf("%10s", targetWord) != 1)
        return 1;

    wordLength = (int)strlen(startWord);

    if (strlen(targetWord) != (size_t)wordLength)
    {
        printf("Words must have the same length.\n");
        return 1;
    }

    for (int i = 0; i < dictionarySize; i++)
    {
        if (strlen(words[i]) != (size_t)wordLength)
        {
            printf("All dictionary words must have the same length.\n");
            return 1;
        }
    }

    qsort(words, dictionarySize, sizeof(words[0]), compareWords);

    int targetIndex = findWord(targetWord);
    if (targetIndex == -1)
    {
        printf("Minimum mutations: -1\n");
        return 0;
    }

    if (strcmp(startWord, targetWord) == 0)
    {
        printf("Minimum mutations: 0\n");
        return 0;
    }

    int startIndex = dictionarySize;
    int answer = bidirectionalBFS(startIndex, targetIndex);

    printf("Minimum mutations: %d\n", answer);
    return 0;
}
```

## 6. Important functions and variables

- `compareWords`: tells `qsort` to sort words alphabetically using `strcmp`.
- `findWord(word)`: binary-searches the sorted dictionary. It returns the array index if found, otherwise `-1`.
- `expandOneLevel(...)`: explores one BFS level. It generates all one-character mutations, checks dictionary membership, records distances, and detects when the two searches meet.
- `bidirectionalBFS(startIndex, targetIndex)`: initializes the queues and distances, expands the smaller frontier, and returns the shortest mutation count or `-1`.
- `words`: stores the dictionary.
- `startWord`: stores `A`, which may not be in the dictionary.
- `distanceA` and `distanceB`: store distances from the start and target searches. `-1` means not visited yet.
- `queueA` and `queueB`: hold words waiting to be explored by each search.
- `headA`/`tailA` and `headB`/`tailB`: track the next queue item and the next free queue position.
- `dictionarySize`: also serves as a special index for `startWord`; dictionary indices are `0` through `dictionarySize - 1`.
- `wordLength`: the common word length.

## 7. Sample input

```text
6
hot
dot
dog
lot
log
cog
hit
cog
```

## 8. Sample output

```text
Enter number of dictionary words: Enter the dictionary words (same length, lowercase):
Enter starting word A: Enter target word B: Minimum mutations: 4
```

Prompts and user input may appear on the same line depending on the terminal. The important result is `Minimum mutations: 4`.

## 9. Dry run

Dictionary: `hot, dot, dog, lot, log, cog`  
Start: `hit`  
Target: `cog`

| Step | Search action | Important result |
|---|---|---|
| 1 | Initialize both searches | `hit` has distance 0 from A; `cog` has distance 0 from B |
| 2 | Expand from `hit` | `hot` is found at distance 1 from A |
| 3 | Expand from `cog` | `dog` and `log` are found at distance 1 from B |
| 4 | Expand the smaller available frontier | The search from `hot` can find `dot` at distance 2 from A, or the other side can continue through `dog`/`log` |
| 5 | Search frontiers meet | For example, `dog` is 3 mutations from `hit` and 1 mutation from `cog` |
| 6 | Add the distances | `3 + 1 = 4` mutations |

One shortest path is `hit -> hot -> dot -> dog -> cog`. The implementation returns the distance, not the actual path.

## 10. Complexity analysis

Let `N` be the number of dictionary words and `L` the word length. There are at most 26 replacement choices per character, so at most `25L` different candidate mutations are tried per expanded word.

- Sorting: `O(N log N * L)` because comparing two words can take up to `L` character comparisons.
- One candidate lookup: `O(L log N)` using binary search, including string comparisons.
- Worst-case search: `O(N * L * 25 * L log N)`, or `O(N L^2 log N)` when the constant 25 is omitted.
- Total worst-case time: `O(N log N * L + N L^2 log N)`, which is usually written as `O(N L^2 log N)` for `L >= 1`.
- Space: `O(NL)` for the stored dictionary and `O(N)` for queues and distance arrays, so total `O(NL)`.

Best case: `O(N log N * L)` due to sorting and checking the target, when the target is absent or the start equals the target. If the start equals the target, the target check still happens after sorting. For a normal connected case, search cost depends on how quickly the frontiers meet. Worst case: the search may examine almost all dictionary words.

The complexity graph plots theoretical growth functions for the sorting phase and the worst-case search phase with `L` fixed at 10. These are normalized mathematical curves, not measured runtime. Bidirectional BFS can reduce the number of explored words in practice, but its worst-case asymptotic bound can still be `O(N L^2 log N)`.

## 11. Advantages

- Finds a shortest path because every mutation has equal cost.
- Often visits fewer words than one-sided BFS.
- Does not need to construct the entire graph in advance.
- Binary search makes dictionary membership checks faster than scanning all words.

## 12. Disadvantages and limitations

- Requires extra queues and distance arrays for both directions.
- The implementation returns the number of mutations, not the full transformation sequence.
- Binary search requires sorting the dictionary first.
- Worst-case time can still be high when many dictionary words are explored.
- Input is limited by the fixed array sizes and `L <= 10`.

## 13. Viva questions and answers

1. **What is Bidirectional BFS?**  It runs BFS from the start and target at the same time and stops when the searches meet.
2. **Why use BFS?** Every mutation costs one step, so BFS finds the minimum number of steps in an unweighted graph.
3. **Why search from both ends?** It can reduce the number of explored levels when the graph branches widely.
4. **What does a vertex represent?** A word.
5. **When are two words adjacent?** When they differ in exactly one character.
6. **Why must the target be in the dictionary?** The problem states that the target must be a valid dictionary word.
7. **Can the start be outside the dictionary?** Yes. The program stores it separately and gives it a special index.
8. **What does `-1` in a distance array mean?** That the word has not been visited from that search side.
9. **Why use two distance arrays?** One stores distances from the start and the other stores distances from the target.
10. **Why use queues?** BFS explores words in increasing distance order.
11. **Why sort the dictionary?** So `findWord` can use binary search.
12. **What is the worst-case time complexity?** `O(N L^2 log N)` for this implementation, plus sorting, which is included in that bound for normal `L >= 1`.
13. **What is the space complexity?** `O(NL)` for the dictionary, plus `O(N)` queues and distances.
14. **What if no sequence exists?** One frontier becomes empty before the searches meet, and the program returns `-1`.
15. **Does the code print the actual sequence?** No, it prints only the minimum number of mutations.

### C-code-specific viva questions

1. **Why is `strcmp` used?** It compares two strings, such as a candidate word and a dictionary word.
2. **Why is `qsort` used?** It sorts the dictionary so binary search works correctly.
3. **Why does `findWord` return `-1`?** It indicates that the word is absent from the dictionary.
4. **Why pass `head` and `tail` as pointers?** The function must update the caller's queue positions.
5. **Why restore `changedWord[i]` after each position?** So the next character position is tested using the original word rather than a previously modified candidate.

## 14. Compile and run

Using GCC:

```bash
gcc solution.c -o solution
./solution
```

On Windows with MinGW, run:

```text
gcc solution.c -o solution.exe
solution.exe
```

Enter the number of dictionary words, each word on its own line, then the starting word and target word.

## 15. Conclusion

Bidirectional BFS is a suitable shortest-path method for this word-transformation problem because every valid mutation has the same cost. Searching from both the starting and target words can reduce the amount of exploration, while sorting and binary search provide efficient dictionary membership checks.
