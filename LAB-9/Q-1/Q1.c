#include <stdio.h>
#include <stdlib.h>

#define MAX 10

int n;
double W;

double value[MAX], weight[MAX], decay[MAX];

int currentOrder[MAX];
int bestOrder[MAX];
int used[MAX];

double bestFraction[MAX];
double bestValue = -1.0;


/* Evaluate one complete scheduling order */
void evaluateOrder()
{
    double density[MAX];
    double fraction[MAX];

    double remaining = W;
    double totalValue = 0.0;

    int i;
    int bestIndex;
    double bestDensity;

    /* Calculate effective density of every item */
    for (i = 0; i < n; i++)
    {
        int item = currentOrder[i];
        int time = i + 1;

        density[item] = value[item] / weight[item]
                        - decay[item] * time;

        fraction[item] = 0.0;
    }

    /*
       Fractional Knapsack:
       repeatedly select the item with
       the highest positive effective density.
    */
    while (remaining > 1e-9)
    {
        bestIndex = -1;
        bestDensity = 0.0;

        for (i = 0; i < n; i++)
        {
            if (fraction[i] < weight[i] &&
                density[i] > bestDensity)
            {
                bestDensity = density[i];
                bestIndex = i;
            }
        }

        /* No useful item is left */
        if (bestIndex == -1)
            break;

        /* Complete item can fit */
        if (weight[bestIndex] <= remaining)
        {
            fraction[bestIndex] = weight[bestIndex];

            totalValue +=
                weight[bestIndex] * density[bestIndex];

            remaining -= weight[bestIndex];
        }
        else
        {
            /* Only a fraction can be taken */
            fraction[bestIndex] = remaining;

            totalValue +=
                remaining * density[bestIndex];

            remaining = 0.0;
        }
    }

    /* Save the best solution */
    if (totalValue > bestValue)
    {
        bestValue = totalValue;

        for (i = 0; i < n; i++)
            bestOrder[i] = currentOrder[i];

        for (i = 0; i < n; i++)
            bestFraction[i] = fraction[i];
    }
}


/* Generate all possible scheduling orders */
void generateOrders(int position)
{
    int i;

    /* A complete order has been generated */
    if (position == n)
    {
        evaluateOrder();
        return;
    }

    /* Try every unused item */
    for (i = 0; i < n; i++)
    {
        if (!used[i])
        {
            used[i] = 1;

            currentOrder[position] = i;

            generateOrders(position + 1);

            used[i] = 0;
        }
    }
}


int main()
{
    int i;

    printf("Enter number of items: ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX)
    {
        printf("Number of items must be between 1 and %d.\n", MAX);
        return 0;
    }

    printf("Enter knapsack capacity: ");
    scanf("%lf", &W);

    printf("Enter value, weight and decay rate for each item:\n");

    for (i = 0; i < n; i++)
    {
        printf("Item %d: ", i + 1);

        scanf("%lf %lf %lf",
              &value[i],
              &weight[i],
              &decay[i]);

        if (weight[i] <= 0 || decay[i] <= 0)
        {
            printf("Weight and decay rate must be positive.\n");
            return 0;
        }
    }

    /* Initially no item is used */
    for (i = 0; i < n; i++)
        used[i] = 0;

    /* Generate all schedules */
    generateOrders(0);

    printf("\nOptimal scheduling order:\n");

    for (i = 0; i < n; i++)
    {
        printf("Time %d -> Item %d\n",
               i + 1,
               bestOrder[i] + 1);
    }

    printf("\nOptimal fractional choices:\n");

    for (i = 0; i < n; i++)
    {
        if (bestFraction[i] > 1e-9)
        {
            double fractionTaken =
                bestFraction[i] / weight[i];

            printf("Item %d: weight taken = %.2f, "
                   "fraction = %.2f\n",
                   i + 1,
                   bestFraction[i],
                   fractionTaken);
        }
    }

    printf("\nMaximum total value = %.2f\n",
           bestValue);

    return 0;
}