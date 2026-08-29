#include <stdio.h>

int weigh(int coins[], int left[], int n1, int right[], int n2)
{
    int leftWeight = 0;
    int rightWeight = 0;
    int i;

    for (i = 0; i < n1; i++)
        leftWeight += coins[left[i]];

    for (i = 0; i < n2; i++)
        rightWeight += coins[right[i]];

    if (leftWeight < rightWeight)
        return -1;      // Left is lighter
    else if (leftWeight > rightWeight)
        return 1;       // Right is lighter
    else
        return 0;       // Equal
}

/*
    candidates[] = coins which may contain the defective coin
    n             = number of candidates
    genuine       = index of a known genuine coin

    Returns:
    index of defective coin, or -1 if no defective coin
*/
int findDefective(int coins[], int candidates[], int n, int genuine)
{
    int i, mid, extra;
    int left[100], right[100];
    int leftCount, rightCount;
    int result;

    /* Base case */
    if (n == 1)
    {
        if (coins[candidates[0]] < coins[genuine])
            return candidates[0];

        return -1;
    }

    /*
        Divide candidates into two groups
    */
    mid = n / 2;

    leftCount = mid;
    rightCount = n - mid;

    for (i = 0; i < leftCount; i++)
        left[i] = candidates[i];

    for (i = 0; i < rightCount; i++)
        right[i] = candidates[mid + i];

    /*
        Make the two groups equal in size.
        If n is odd, keep one coin aside.
    */
    extra = -1;

    if (leftCount > rightCount)
    {
        extra = left[leftCount - 1];
        leftCount--;
    }
    else if (rightCount > leftCount)
    {
        extra = right[rightCount - 1];
        rightCount--;
    }

    /*
        Weigh the two groups
    */
    result = weigh(coins, left, leftCount, right, rightCount);

    /*
        Left side is lighter
        => defective coin is in left group
    */
    if (result == -1)
    {
        return findDefective(coins, left, leftCount, right[0]);
    }

    /*
        Right side is lighter
        => defective coin is in right group
    */
    else if (result == 1)
    {
        return findDefective(coins, right, rightCount, left[0]);
    }

    /*
        Both sides are equal.
        Therefore neither group contains the defective coin.
    */
    else
    {
        if (extra != -1)
        {
            if (coins[extra] < coins[genuine])
                return extra;
            else
                return -1;
        }

        return -1;
    }
}

int main()
{
    int coins[100];
    int candidates[100];

    int n;
    int i;
    int mid;
    int left[100], right[100];
    int leftCount, rightCount;
    int extra = -1;
    int result;
    int defective;

    printf("Enter number of coins: ");
    scanf("%d", &n);

    if (n < 2)
    {
        printf("At least 2 coins are required.\n");
        return 0;
    }

    printf("Enter weights of the coins:\n");

    for (i = 0; i < n; i++)
    {
        scanf("%d", &coins[i]);
        candidates[i] = i;
    }

    /*
        First weighing.
        We do this separately because initially
        we don't have a known genuine coin.
    */

    mid = n / 2;

    leftCount = mid;
    rightCount = n - mid;

    for (i = 0; i < leftCount; i++)
        left[i] = i;

    for (i = 0; i < rightCount; i++)
        right[i] = mid + i;

    /*
        If n is odd, keep one coin aside.
    */
    if (leftCount != rightCount)
    {
        extra = right[rightCount - 1];
        rightCount--;
    }

    /*
        First weighing
    */
    result = weigh(coins, left, leftCount, right, rightCount);

    if (result == -1)
    {
        /*
            Left is lighter.
            Therefore defective coin is in left.
            Any coin in right is genuine.
        */
        defective = findDefective(
            coins,
            left,
            leftCount,
            right[0]
        );
    }
    else if (result == 1)
    {
        /*
            Right is lighter.
            Therefore defective coin is in right.
            Any coin in left is genuine.
        */
        defective = findDefective(
            coins,
            right,
            rightCount,
            left[0]
        );
    }
    else
    {
        /*
            Both groups have equal weight.
            Therefore the defective coin, if present,
            must be the extra coin.
        */

        if (extra != -1)
        {
            if (coins[extra] < coins[left[0]])
                defective = extra;
            else
                defective = -1;
        }
        else
        {
            defective = -1;
        }
    }

    /*
        Print result
    */
    if (defective == -1)
    {
        printf("\nNo defective coin found.\n");
    }
    else
    {
        printf("\nDefective coin = %d\n", defective + 1);
        printf("Weight = %d\n", coins[defective]);
    }

    return 0;
}