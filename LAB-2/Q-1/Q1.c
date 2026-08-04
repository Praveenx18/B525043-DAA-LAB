/*
 * dict_simple_timing.c
 * ------------------------------------------------------------
 * Same two structures as dict_simple.c:
 *   1. UNSORTED ARRAY
 *   2. SORTED SINGLY LINKED LIST
 *
 * This version TIMES each operation for growing values of n
 * (100, 1000, 5000, ... 20000) and writes the results to a CSV
 * file so we can plot them and SEE the O(1) vs O(n) difference
 * with our own eyes, instead of just trusting the theory.
 *
 * How the timing works (kept deliberately simple):
 *   1. Build a fresh structure with n elements.
 *   2. Run the operation REPEAT times in a loop.
 *   3. Divide the total time by REPEAT to get the average time
 *      of a single call (this smooths out timer noise, since
 *      one single operation can be too fast to measure reliably).
 *
 * Compile: gcc -O2 -o dict_simple_timing dict_simple_timing.c
 * Run:     ./dict_simple_timing > timing_simple.csv
 * ------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define REPEAT 2000   /* how many times we repeat an operation to time it accurately */

/* ============================================================
 *  UNSORTED ARRAY
 * ============================================================ */
typedef struct {
    int *data;
    int count;
} Array;

Array array_build(int n) {
    Array a;
    a.data = malloc(sizeof(int) * (n + REPEAT + 10)); /* extra room for inserts */
    a.count = n;
    for (int i = 0; i < n; i++) {
        a.data[i] = i + 1;      /* just fill with 1, 2, 3, ..., n */
    }
    return a;
}

void array_free(Array *a) { free(a->data); }

int array_search(Array *a, int value) {
    for (int i = 0; i < a->count; i++) {
        if (a->data[i] == value) return i;
    }
    return -1;
}

void array_insert(Array *a, int value) {
    a->data[a->count] = value;   /* O(1): just add to the end */
    a->count++;
}

void array_delete_front(Array *a) {
    /* delete the FIRST element -> worst case, must shift everything left */
    for (int i = 0; i < a->count - 1; i++) {
        a->data[i] = a->data[i + 1];
    }
    a->count--;
}

int array_max(Array *a) {
    int m = a->data[0];
    for (int i = 1; i < a->count; i++) if (a->data[i] > m) m = a->data[i];
    return m;
}

int array_min(Array *a) {
    int m = a->data[0];
    for (int i = 1; i < a->count; i++) if (a->data[i] < m) m = a->data[i];
    return m;
}

/* smallest value bigger than 'value' */
int array_successor(Array *a, int value) {
    int best = -1;
    for (int i = 0; i < a->count; i++) {
        if (a->data[i] > value && (best == -1 || a->data[i] < best)) best = a->data[i];
    }
    return best;
}

/* largest value smaller than 'value' */
int array_predecessor(Array *a, int value) {
    int best = -1;
    for (int i = 0; i < a->count; i++) {
        if (a->data[i] < value && (best == -1 || a->data[i] > best)) best = a->data[i];
    }
    return best;
}

/* ============================================================
 *  SORTED SINGLY LINKED LIST
 * ============================================================ */
typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *list_build(int n) {
    Node *head = NULL, *tail = NULL;
    for (int i = 1; i <= n; i++) {
        Node *node = malloc(sizeof(Node));
        node->data = i;
        node->next = NULL;
        if (head == NULL) head = node; else tail->next = node;
        tail = node;
    }
    return head;
}

void list_free(Node *head) {
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

Node *list_search(Node *head, int value) {
    Node *current = head;
    while (current != NULL && current->data < value) current = current->next;
    if (current != NULL && current->data == value) return current;
    return NULL;
}

/* insert 'value' at the FRONT of an already-sorted list (worst case:
   value is smaller than everything, so no need to search a position
   -- BUT we still walk the whole list here to find where it goes,
   to mimic worst-case insert of a value bigger than everything) */
Node *list_insert_worst(Node *head, int value) {
    Node *node = malloc(sizeof(Node));
    node->data = value;
    node->next = NULL;
    if (head == NULL || head->data > value) {
        node->next = head;
        return node;
    }
    Node *current = head;
    while (current->next != NULL && current->next->data < value) current = current->next;
    node->next = current->next;
    current->next = node;
    return head;
}

/* delete the LAST node -> worst case, must walk from head to find it */
Node *list_delete_last(Node *head) {
    if (head == NULL) return NULL;
    if (head->next == NULL) { free(head); return NULL; }
    Node *current = head;
    while (current->next->next != NULL) current = current->next;
    free(current->next);
    current->next = NULL;
    return head;
}

int list_min(Node *head) { return head->data; }

int list_max(Node *head) {
    Node *current = head;
    while (current->next != NULL) current = current->next;
    return current->data;
}

int list_successor(Node *node) { return node->next ? node->next->data : -1; }

int list_predecessor(Node *head, Node *target) {
    if (head == target) return -1;
    Node *current = head;
    while (current->next != target) current = current->next;
    return current->data;
}

/* ============================================================
 *  TIMING HELPERS
 * ============================================================ */
double time_now(void) {
    return (double)clock() / CLOCKS_PER_SEC;
}

void print_row(const char *structure, const char *operation, int n, double seconds_per_call) {
    printf("%s,%s,%d,%.9f\n", structure, operation, n, seconds_per_call);
}

/* ============================================================
 *  MAIN: time both structures at several sizes of n
 * ============================================================ */
int main(void) {
    int n_values[] = {100, 1000, 2000, 5000, 8000, 10000, 15000, 20000};
    int num_sizes = sizeof(n_values) / sizeof(n_values[0]);

    /* IMPORTANT: a smart compiler can notice that a function like
       array_search() has no side effects, and if we never USE its
       return value, the compiler is allowed to delete the whole loop
       (this is called "dead code elimination"). That would make our
       timings show 0 even though real work should be happening!
       The fix: always add the result into this "volatile" variable,
       which forces the compiler to actually keep the computation. */
    volatile long sink = 0;

    printf("structure,operation,n,seconds_per_call\n");

    for (int s = 0; s < num_sizes; s++) {
        int n = n_values[s];
        double start, end;

        /* ----------- ARRAY: Search ----------- */
        Array a = array_build(n);
        start = time_now();
        for (int r = 0; r < REPEAT; r++) sink += array_search(&a, n + 1); /* n+1 is never in the array */
        end = time_now();
        print_row("Array", "Search", n, (end - start) / REPEAT);
        array_free(&a);

        /* ----------- ARRAY: Insert ----------- */
        a = array_build(n);
        start = time_now();
        for (int r = 0; r < REPEAT; r++) array_insert(&a, 999999);
        end = time_now();
        print_row("Array", "Insert", n, (end - start) / REPEAT);
        array_free(&a);

        /* ----------- ARRAY: Delete ----------- */
        a = array_build(n);
        start = time_now();
        for (int r = 0; r < REPEAT; r++) array_delete_front(&a);
        end = time_now();
        print_row("Array", "Delete", n, (end - start) / REPEAT);
        array_free(&a);

        /* ----------- ARRAY: Max, Min, Successor, Predecessor ----------- */
        a = array_build(n);
        start = time_now();
        for (int r = 0; r < REPEAT; r++) sink += array_max(&a);
        end = time_now();
        print_row("Array", "Max", n, (end - start) / REPEAT);

        start = time_now();
        for (int r = 0; r < REPEAT; r++) sink += array_min(&a);
        end = time_now();
        print_row("Array", "Min", n, (end - start) / REPEAT);

        start = time_now();
        for (int r = 0; r < REPEAT; r++) sink += array_successor(&a, n / 2);
        end = time_now();
        print_row("Array", "Successor", n, (end - start) / REPEAT);

        start = time_now();
        for (int r = 0; r < REPEAT; r++) sink += array_predecessor(&a, n / 2);
        end = time_now();
        print_row("Array", "Predecessor", n, (end - start) / REPEAT);
        array_free(&a);

        /* ----------- LIST: Search ----------- */
        Node *head = list_build(n);
        start = time_now();
        for (int r = 0; r < REPEAT; r++) sink += (long)(size_t)list_search(head, n + 1); /* never found */
        end = time_now();
        print_row("SortedList", "Search", n, (end - start) / REPEAT);
        list_free(head);

        /* ----------- LIST: Insert ----------- */
        head = list_build(n);
        start = time_now();
        for (int r = 0; r < REPEAT; r++) head = list_insert_worst(head, 999999 + r);
        end = time_now();
        print_row("SortedList", "Insert", n, (end - start) / REPEAT);
        list_free(head);

        /* ----------- LIST: Delete ----------- */
        head = list_build(n);
        start = time_now();
        for (int r = 0; r < REPEAT; r++) head = list_delete_last(head);
        end = time_now();
        print_row("SortedList", "Delete", n, (end - start) / REPEAT);
        list_free(head);

        /* ----------- LIST: Max, Min, Successor, Predecessor ----------- */
        head = list_build(n);
        start = time_now();
        for (int r = 0; r < REPEAT; r++) sink += list_max(head);
        end = time_now();
        print_row("SortedList", "Max", n, (end - start) / REPEAT);

        start = time_now();
        for (int r = 0; r < REPEAT; r++) sink += list_min(head);
        end = time_now();
        print_row("SortedList", "Min", n, (end - start) / REPEAT);

        Node *mid = list_search(head, n / 2);
        start = time_now();
        for (int r = 0; r < REPEAT; r++) sink += list_successor(mid);
        end = time_now();
        print_row("SortedList", "Successor", n, (end - start) / REPEAT);

        start = time_now();
        for (int r = 0; r < REPEAT; r++) sink += list_predecessor(head, mid);
        end = time_now();
        print_row("SortedList", "Predecessor", n, (end - start) / REPEAT);
        list_free(head);
    }

    fprintf(stderr, "(ignore this number, it just stops the compiler cheating: %ld)\n", sink);
    return 0;
}