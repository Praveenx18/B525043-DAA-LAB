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

    /* Initially give one candy to every child. */
    for (i = 0; i < n; i++) {
        left[i] = 1;
        right[i] = 1;
    }

    /* Left-to-right pass */
    for (i = 1; i < n; i++) {
        if (rating[i] > rating[i - 1]) {
            left[i] = left[i - 1] + 1;
        }
    }

    /* Right-to-left pass */
    for (i = n - 2; i >= 0; i--) {
        if (rating[i] > rating[i + 1]) {
            right[i] = right[i + 1] + 1;
        }
    }

    /* Combine both requirements */
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