//  ALGORITHM
// Step-by-step

//1. Read n.
//2. Read the elements of S1.
//3. Read the elements of S2.
//4. Read x.
//5. Sort S2.
//6. For every element S1[i]:
//       Calculate required = x - S1[i].
//       Perform binary search for required in sorted S2.
//7. If binary search finds the required element:
//      A pair exists.
//      Print the pair.
//      Stop.
//8. If all elements of S1 are checked without finding a pair:
//      Print that no such pair exists.

#include <stdio.h>

void sort(int arr[], int n)
{
    int i, j, temp;

    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int binarySearch(int arr[], int n, int key)
{
    int low = 0, high = n - 1, mid;

    while (low <= high)
    {
        mid = (low + high) / 2;

        if (arr[mid] == key)
            return 1;
        else if (arr[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return 0;
}

int main()
{
    int n, x, i, required;
    int S1[100], S2[100];
    int found = 0;

    printf("Enter the size of the sets: ");
    scanf("%d", &n);

    printf("Enter elements of S1:\n");
    for (i = 0; i < n; i++)
        scanf("%d", &S1[i]);

    printf("Enter elements of S2:\n");
    for (i = 0; i < n; i++)
        scanf("%d", &S2[i]);

    printf("Enter x: ");
    scanf("%d", &x);

    sort(S2, n);

    for (i = 0; i < n; i++)
    {
        required = x - S1[i];

        if (binarySearch(S2, n, required))
        {
            printf("Pair found: (%d, %d)\n", S1[i], required);
            found = 1;
            break;
        }
    }

    if (found == 0)
        printf("No pair found whose sum is %d\n", x);

    return 0;
}