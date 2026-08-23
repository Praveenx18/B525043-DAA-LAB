// ALGORITHM

// 1. Read n.
// 2. For every interval (l, r):
//      Store (l, START).
//      Store (r, END).
// 3. Sort all 2n events by their coordinate.
// 4. Start with active = 0.
// 5. Move through the events from left to right.
// 6. For every group of events having the same coordinate:
//      Count how many intervals start there.
//      Count how many intervals end there.
//      Add all starts to active.
//      Check whether active is the maximum so far.
//      Store this coordinate as the answer if necessary.
//      Remove all intervals ending there.
// Print the point having the maximum number of overlapping intervals.

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int point;
    int type;
} Event;

void merge(Event events[], int left, int mid, int right)
{
    int i = left;
    int j = mid + 1;
    int k = 0;
    int size = right - left + 1;

    Event *temp = (Event *)malloc(size * sizeof(Event));

    while (i <= mid && j <= right)
    {
        if (events[i].point < events[j].point ||
            (events[i].point == events[j].point && events[i].type > events[j].type))
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
        events[i] = temp[k];

    free(temp);
}

void mergeSort(Event events[], int left, int right)
{
    if (left < right)
    {
        int mid = (left + right) / 2;

        mergeSort(events, left, mid);
        mergeSort(events, mid + 1, right);
        merge(events, left, mid, right);
    }
}

int main()
{
    int n, i;
    Event *events;
    int active = 0;
    int maxActive = 0;
    int answerPoint = 0;

    printf("Enter number of intervals: ");
    scanf("%d", &n);

    events = (Event *)malloc(2 * n * sizeof(Event));

    for (i = 0; i < n; i++)
    {
        int left, right;

        printf("Enter left and right endpoints of interval %d: ", i + 1);
        scanf("%d %d", &left, &right);

        events[2 * i].point = left;
        events[2 * i].type = 1;

        events[2 * i + 1].point = right;
        events[2 * i + 1].type = -1;
    }

    mergeSort(events, 0, 2 * n - 1);

    i = 0;

    while (i < 2 * n)
    {
        int currentPoint = events[i].point;
        int startCount = 0;
        int endCount = 0;

        while (i < 2 * n && events[i].point == currentPoint)
        {
            if (events[i].type == 1)
                startCount++;
            else
                endCount++;

            i++;
        }

        active = active + startCount;

        if (active > maxActive)
        {
            maxActive = active;
            answerPoint = currentPoint;
        }

        active = active - endCount;
    }

    printf("\nPoint with maximum overlap = %d\n", answerPoint);
    printf("Maximum number of intervals = %d\n", maxActive);

    free(events);

    return 0;
}