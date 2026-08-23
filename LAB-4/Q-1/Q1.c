// 4. Algorithm
// Step 1
// Read n.

// Step 2
// Read the n pairs (number, colour).

// Step 3
// Create three arrays:
// red[]
// blue[]
// yellow[]

// Step 4
// Scan the input from left to right.
// For every pair:
// If colour is R, put it in red[].
// If colour is B, put it in blue[].
// If colour is Y, put it in yellow[]. 

// Step 5
// Print all elements of red[].

// Step 6
// Print all elements of blue[].

// Step 7
// Print all elements of yellow[].

// Because the input was sorted by number and we insert elements in the same order in each colour array,
//  the numbers for each colour remain sorted.

#include <stdio.h>

struct Item
{
    int number;
    char color;
};

int main()
{
    int n, i;
    int redCount = 0, blueCount = 0, yellowCount = 0;

    printf("Enter number of items: ");
    scanf("%d", &n);

    struct Item items[n];
    struct Item red[n], blue[n], yellow[n];

    printf("Enter number and color (R/B/Y) for each item:\n");

    for (i = 0; i < n; i++)
    {
        scanf("%d %c", &items[i].number, &items[i].color);

        if (items[i].color == 'R')
        {
            red[redCount] = items[i];
            redCount++;
        }
        else if (items[i].color == 'B')
        {
            blue[blueCount] = items[i];
            blueCount++;
        }
        else if (items[i].color == 'Y')
        {
            yellow[yellowCount] = items[i];
            yellowCount++;
        }
    }

    printf("\nSorted items by color:\n");

    for (i = 0; i < redCount; i++)
    {
        printf("(%d, Red)\n", red[i].number);
    }

    for (i = 0; i < blueCount; i++)
    {
        printf("(%d, Blue)\n", blue[i].number);
    }

    for (i = 0; i < yellowCount; i++)
    {
        printf("(%d, Yellow)\n", yellow[i].number);
    }

    return 0;
}