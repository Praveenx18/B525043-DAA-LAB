// ALGORITHM

// 1.Read the number of people n.
// 2.Create an array of 2*n events.
// 3.For every person:
//      Store their entry time as an event with value +1.
//      Store their exit time as an event with value -1.
// 4.Sort all 2*n events according to their time using Merge Sort.
// 5.Set:
//      current = 0
//      maximum = 0
// 6.Traverse the sorted events:
//      If it is an entry, increase current.
//      If it is an exit, decrease current.
// 7.Whenever current > maximum:
//      Update maximum.
//      Store the current event's time as maxTime.
// 8.Print maximum and maxTime.

#include <stdio.h>

struct Event
{
    int time;
    int type;
};

void merge(struct Event events[], int left, int mid, int right)
{
    int i = left;
    int j = mid + 1;
    int k = 0;
    struct Event temp[right - left + 1];

    while (i <= mid && j <= right)
    {
        if (events[i].time < events[j].time)
        {
            temp[k] = events[i];
            i++;
        }
        else
        {
            temp[k] = events[j];
            j++;
        }
        k++;
    }

    while (i <= mid)
    {
        temp[k] = events[i];
        i++;
        k++;
    }

    while (j <= right)
    {
        temp[k] = events[j];
        j++;
        k++;
    }

    for (i = left, k = 0; i <= right; i++, k++)
    {
        events[i] = temp[k];
    }
}

void mergeSort(struct Event events[], int left, int right)
{
    int mid;

    if (left < right)
    {
        mid = (left + right) / 2;

        mergeSort(events, left, mid);
        mergeSort(events, mid + 1, right);

        merge(events, left, mid, right);
    }
}

int main()
{
    int n, i;
    int current = 0;
    int maximum = 0;
    int maxTime = 0;

    printf("Enter number of people: ");
    scanf("%d", &n);

    struct Event events[2 * n];

    for (i = 0; i < n; i++)
    {
        int entry, exit;

        printf("Enter entry and exit time for person %d: ", i + 1);
        scanf("%d %d", &entry, &exit);

        events[2 * i].time = entry;
        events[2 * i].type = 1;

        events[2 * i + 1].time = exit;
        events[2 * i + 1].type = -1;
    }

    mergeSort(events, 0, 2 * n - 1);

    for (i = 0; i < 2 * n; i++)
    {
        current = current + events[i].type;

        if (current > maximum)
        {
            maximum = current;
            maxTime = events[i].time;
        }
    }

    printf("\nMaximum number of people present = %d\n", maximum);
    printf("Time when maximum was first reached = %d\n", maxTime);

    return 0;
}