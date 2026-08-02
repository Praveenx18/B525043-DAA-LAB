#include <stdio.h>

/* Global counter to track number of moves */
long long moveCount = 0;

/*
 * Recursively solve Towers of Hanoi for n discs,
 * moving them from 'from' rod to 'to' rod, using 'aux' as helper.
 * Set PRINT_MOVES to 1 if you want each move printed.
 */
#define PRINT_MOVES 0

void towersOfHanoi(int n, char from, char aux, char to) {
    if (n == 0) return;

    towersOfHanoi(n - 1, from, to, aux);

    moveCount++;
#if PRINT_MOVES
    printf("Move disc %d from %c to %c\n", n, from, to);
#endif

    towersOfHanoi(n - 1, aux, from, to);
}

int main(void) {
    int maxDiscs = 20;   /* change as needed */
    FILE *fp = fopen("hanoi_moves.csv", "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    fprintf(fp, "n,moves\n");
    printf("%-6s %-15s\n", "n", "Moves");

    for (int n = 1; n <= maxDiscs; n++) {
        moveCount = 0;
        towersOfHanoi(n, 'A', 'B', 'C');
        printf("%-6d %-15lld\n", n, moveCount);
        fprintf(fp, "%d,%lld\n", n, moveCount);
    }

    fclose(fp);
    printf("\nResults written to hanoi_moves.csv\n");
    return 0;
}