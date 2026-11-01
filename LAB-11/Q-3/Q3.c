
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
                        ownDistance[next] =
                            ownDistance[current] + 1;

                        queue[*tail] = next;
                        (*tail)++;
                    }

                    if (otherDistance[next] != -1)
                        return ownDistance[next]
                               + otherDistance[next];
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
        {
            answer = expandOneLevel(
                queueA, &headA, &tailA,
                distanceA, distanceB
            );
        }
        else
        {
            answer = expandOneLevel(
                queueB, &headB, &tailB,
                distanceB, distanceA
            );
        }

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

    /* The starting word has a special index outside the dictionary. */
    int startIndex = dictionarySize;

    int answer = bidirectionalBFS(startIndex, targetIndex);

    printf("Minimum mutations: %d\n", answer);

    return 0;
}
