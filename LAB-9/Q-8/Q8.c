#include <stdio.h>

void sortArray(int arr[], int n)
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

int findMinimumRooms(int start[], int end[], int n)
{
    int i = 0;
    int j = 0;
    int rooms = 0;
    int maxRooms = 0;

    sortArray(start, n);
    sortArray(end, n);

    while (i < n)
    {
        if (start[i] < end[j])
        {
            rooms++;

            if (rooms > maxRooms)
            {
                maxRooms = rooms;
            }

            i++;
        }
        else
        {
            rooms--;
            j++;
        }
    }

    return maxRooms;
}

int main()
{
    int n, i;
    int start[100], end[100];

    printf("Enter number of meetings: ");
    scanf("%d", &n);

    printf("Enter start and end time of each meeting:\n");

    for (i = 0; i < n; i++)
    {
        scanf("%d %d", &start[i], &end[i]);
    }

    printf("Minimum number of rooms required = %d\n",
           findMinimumRooms(start, end, n));

    return 0;
}