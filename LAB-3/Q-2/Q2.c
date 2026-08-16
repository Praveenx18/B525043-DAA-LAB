#include <stdio.h>

/* 
   Returns:
   -1 if left group is lighter
    0 if both groups are equal
    1 if right group is lighter
*/
int compareGroups(int coins[], int start1, int end1,
                  int start2, int end2)
{
    int sum1 = 0;
    int sum2 = 0;
    int i;

    for (i = start1; i <= end1; i++)
    {
        sum1 = sum1 + coins[i];
    }

    for (i = start2; i <= end2; i++)
    {
        sum2 = sum2 + coins[i];
    }

    if (sum1 < sum2)
        return -1;
    else if (sum1 > sum2)
        return 1;
    else
        return 0;
}

/*
   Searches for the defective coin between start and end.

   Returns:
   -1 if no defective coin is found
   index of defective coin otherwise
*/
int findDefective(int coins[], int start, int end)
{
    int n;
    int size;
    int aStart, aEnd;
    int bStart, bEnd;
    int cStart, cEnd;
    int result;

    n = end - start + 1;

    /* Base case: two coins */
    if (n == 2)
    {
        if (coins[start] < coins[end])
            return start;
        else if (coins[end] < coins[start])
            return end;
        else
            return -1;
    }

    /*
       Divide the coins into three groups.
       First two groups have equal size.
    */
    size = n / 3;

    aStart = start;
    aEnd = start + size - 1;

    bStart = aEnd + 1;
    bEnd = bStart + size - 1;

    cStart = bEnd + 1;
    cEnd = end;

    /* Compare Group A and Group B */
    result = compareGroups(coins, aStart, aEnd,
                            bStart, bEnd);

    /* Group A is lighter */
    if (result == -1)
    {
        return findDefective(coins, aStart, aEnd);
    }

    /* Group B is lighter */
    else if (result == 1)
    {
        return findDefective(coins, bStart, bEnd);
    }

    /* Group A and Group B are equal */
    else
    {
        int cSize;

        cSize = cEnd - cStart + 1;

        /*
           If Group C has no coins,
           then all coins are normal.
        */
        if (cSize == 0)
        {
            return -1;
        }

        /*
           If Group C has one coin,
           compare it with a known-good coin.
           
           Since A and B were equal, and there can be
           at most one defective coin, a coin from A
           is definitely good.
        */
        if (cSize == 1)
        {
            if (coins[cStart] < coins[aStart])
                return cStart;
            else
                return -1;
        }

        /* Search Group C */
        return findDefective(coins, cStart, cEnd);
    }
}

int main()
{
    int coins[100];
    int n;
    int i;
    int defective;

    printf("Enter number of coins: ");
    scanf("%d", &n);

    printf("Enter the weights of the coins:\n");

    for (i = 0; i < n; i++)
    {
        scanf("%d", &coins[i]);
    }

    defective = findDefective(coins, 0, n - 1);

    if (defective == -1)
    {
        printf("No defective coin found.\n");
    }
    else
    {
        printf("Defective coin is at position %d.\n", defective + 1);
        printf("Weight of defective coin = %d\n",
               coins[defective]);
    }

    return 0;
}