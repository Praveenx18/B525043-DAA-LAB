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

                int currentOverlap =
                    findOverlap(strings[i], strings[j]);

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

        /* Replace first string with merged string */
        strcpy(strings[bestI], merged);

        /* Remove second string */
        removeString(strings, &n, bestJ);
    }

    printf("\nGreedy Superstring: %s\n", strings[0]);

    return 0;
}