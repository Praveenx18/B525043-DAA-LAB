#include <stdio.h>
#include <stdlib.h>
#define MAX_N 100000
#define STEP 5000
long long operations;
typedef struct Node {
    int key;
    struct Node *next;
    struct Node *prev;
} Node;
Node *create_node(int key) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    node->key = key;
    node->next = NULL;
    node->prev = NULL;
    return node;
}
int unsorted_array_search(int array[], int n, int key) {
    for (int i = 0; i < n; i++) {
        operations++;
        if (array[i] == key)
            return i;
    }
    return -1;
}
void unsorted_array_insert(int array[], int *n, int key) {
    array[*n] = key;
    (*n)++;
    operations++;
}
void unsorted_array_delete(int array[], int *n, int index) {
    array[index] = array[*n - 1];
    (*n)--;
    operations++;
}
int unsorted_array_minimum(int array[], int n) {
    int minimum = array[0];
    for (int i = 1; i < n; i++) {
        operations++;
        if (array[i] < minimum)
            minimum = array[i];
    }
    return minimum;
}
int unsorted_array_maximum(int array[], int n) {
    int maximum = array[0];
    for (int i = 1; i < n; i++) {
        operations++;
        if (array[i] > maximum)
            maximum = array[i];
    }
    return maximum;
}
int unsorted_array_predecessor(int array[], int n, int key) {
    int predecessor = -1;
    for (int i = 0; i < n; i++) {
        operations++;
        if (array[i] < key &&
            (predecessor == -1 || array[i] > predecessor)) {
            predecessor = array[i];
        }
    }
    return predecessor;
}
int unsorted_array_successor(int array[], int n, int key) {
    int successor = -1;
    for (int i = 0; i < n; i++) {
        operations++;
        if (array[i] > key &&
            (successor == -1 || array[i] < successor)) {
            successor = array[i];
        }
    }
    return successor;
}
int sorted_array_search(int array[], int n, int key) {
    int left = 0;
    int right = n - 1;
    while (left <= right) {
        int middle = left + (right - left) / 2;
        operations++;
        if (array[middle] == key)
            return middle;
        if (array[middle] < key)
            left = middle + 1;
        else
            right = middle - 1;
    }
    return -1;
}
void sorted_array_insert(int array[], int *n, int key) {
    int i = *n - 1;
    while (i >= 0 && array[i] > key) {
        array[i + 1] = array[i];
        i--;
        operations++;
    }
    array[i + 1] = key;
    (*n)++;
    operations++;
}
void sorted_array_delete(int array[], int *n, int index) {
    for (int i = index; i < *n - 1; i++) {
        array[i] = array[i + 1];
        operations++;
    }
    (*n)--;
}
int sorted_array_minimum(int array[], int n) {
    operations++;
    return array[0];
}
int sorted_array_maximum(int array[], int n) {
    operations++;
    return array[n - 1];
}
int sorted_array_predecessor(int array[], int n, int index) {
    (void)n;
    if (index <= 0)
        return -1;
    operations++;
    return array[index - 1];
}
int sorted_array_successor(int array[], int n, int index) {
    if (index < 0 || index >= n - 1)
        return -1;
    operations++;
    return array[index + 1];
}
Node *singly_unsorted_search(Node *head, int key) {
    Node *current = head;
    while (current != NULL) {
        operations++;
        if (current->key == key)
            return current;
        current = current->next;
    }
    return NULL;
}
void singly_unsorted_insert(Node **head, int key) {
    Node *node = create_node(key);
    node->next = *head;
    *head = node;
    operations++;
}
void singly_unsorted_delete(Node **head, Node *target) {
    if (*head == NULL || target == NULL)
        return;
    if (*head == target) {
        *head = target->next;
        free(target);
        operations++;
        return;
    }
    Node *current = *head;
    while (current->next != NULL &&
           current->next != target) {
        current = current->next;
        operations++;
    }
    if (current->next == target) {
        current->next = target->next;
        free(target);
        operations++;
    }
}
Node *singly_unsorted_minimum(Node *head) {
    Node *current = head;
    Node *minimum = head;
    while (current != NULL) {
        operations++;
        if (current->key < minimum->key)
            minimum = current;
        current = current->next;
    }
    return minimum;
}
Node *singly_unsorted_maximum(Node *head) {
    Node *current = head;
    Node *maximum = head;
    while (current != NULL) {
        operations++;
        if (current->key > maximum->key)
            maximum = current;
        current = current->next;
    }
    return maximum;
}
Node *singly_unsorted_predecessor(Node *head, Node *target) {
    Node *current = head;
    Node *predecessor = NULL;
    while (current != NULL) {
        operations++;
        if (current->key < target->key &&
            (predecessor == NULL ||
             current->key > predecessor->key)) {
            predecessor = current;
        }
        current = current->next;
    }
    return predecessor;
}
Node *singly_unsorted_successor(Node *head, Node *target) {
    Node *current = head;
    Node *successor = NULL;
    while (current != NULL) {
        operations++;
        if (current->key > target->key &&
            (successor == NULL ||
             current->key < successor->key)) {
            successor = current;
        }
        current = current->next;
    }
    return successor;
}
Node *singly_sorted_search(Node *head, int key) {
    Node *current = head;
    while (current != NULL && current->key <= key) {
        operations++;
        if (current->key == key)
            return current;
        current = current->next;
    }
    return NULL;
}
void singly_sorted_insert(Node **head, int key) {
    Node *node = create_node(key);
    if (*head == NULL || (*head)->key >= key) {
        node->next = *head;
        *head = node;
        operations++;
        return;
    }
    Node *current = *head;
    while (current->next != NULL &&
           current->next->key < key) {
        current = current->next;
        operations++;
    }
    node->next = current->next;
    current->next = node;
    operations++;
}
void singly_sorted_delete(Node **head, Node *target) {
    if (*head == NULL || target == NULL)
        return;
    if (*head == target) {
        *head = target->next;
        free(target);
        operations++;
        return;
    }
    Node *current = *head;
    while (current->next != NULL &&
           current->next != target) {
        current = current->next;
        operations++;
    }
    if (current->next == target) {
        current->next = target->next;
        free(target);
        operations++;
    }
}
Node *singly_sorted_minimum(Node *head) {
    operations++;
    return head;
}
Node *singly_sorted_maximum(Node *head) {
    Node *current = head;
    while (current != NULL && current->next != NULL) {
        current = current->next;
        operations++;
    }
    return current;
}
Node *singly_sorted_predecessor(Node *head, Node *target) {
    Node *current = head;
    Node *previous = NULL;
    while (current != NULL && current != target) {
        previous = current;
        current = current->next;
        operations++;
    }
    return previous;
}
Node *singly_sorted_successor(Node *target) {
    operations++;
    return target == NULL ? NULL : target->next;
}
Node *doubly_unsorted_search(Node *head, int key) {
    Node *current = head;
    while (current != NULL) {
        operations++;
        if (current->key == key)
            return current;
        current = current->next;
    }
    return NULL;
}
void doubly_unsorted_insert(Node **head, int key) {
    Node *node = create_node(key);
    node->next = *head;
    if (*head != NULL)
        (*head)->prev = node;
    *head = node;
    operations++;
}
void doubly_unsorted_delete(Node **head, Node *target) {
    if (target == NULL)
        return;
    if (target->prev != NULL)
        target->prev->next = target->next;
    else
        *head = target->next;
    if (target->next != NULL)
        target->next->prev = target->prev;
    free(target);
    operations++;
}
Node *doubly_unsorted_minimum(Node *head) {
    Node *current = head;
    Node *minimum = head;
    while (current != NULL) {
        operations++;
        if (current->key < minimum->key)
            minimum = current;
        current = current->next;
    }
    return minimum;
}
Node *doubly_unsorted_maximum(Node *head) {
    Node *current = head;
    Node *maximum = head;
    while (current != NULL) {
        operations++;
        if (current->key > maximum->key)
            maximum = current;
        current = current->next;
    }
    return maximum;
}
Node *doubly_unsorted_predecessor(Node *head, Node *target) {
    Node *current = head;
    Node *predecessor = NULL;
    while (current != NULL) {
        operations++;
        if (current->key < target->key &&
            (predecessor == NULL ||
             current->key > predecessor->key)) {
            predecessor = current;
        }
        current = current->next;
    }
    return predecessor;
}
Node *doubly_unsorted_successor(Node *head, Node *target) {
    Node *current = head;
    Node *successor = NULL;
    while (current != NULL) {
        operations++;
        if (current->key > target->key &&
            (successor == NULL ||
             current->key < successor->key)) {
            successor = current;
        }
        current = current->next;
    }
    return successor;
}
Node *doubly_sorted_search(Node *head, int key) {
    Node *current = head;
    while (current != NULL && current->key <= key) {
        operations++;
        if (current->key == key)
            return current;
        current = current->next;
    }
    return NULL;
}
void doubly_sorted_insert(Node **head, int key) {
    Node *node = create_node(key);
    if (*head == NULL || (*head)->key >= key) {
        node->next = *head;
        if (*head != NULL)
            (*head)->prev = node;
        *head = node;
        operations++;
        return;
    }
    Node *current = *head;
    while (current->next != NULL &&
           current->next->key < key) {
        current = current->next;
        operations++;
    }
    node->next = current->next;
    node->prev = current;
    if (current->next != NULL)
        current->next->prev = node;
    current->next = node;
    operations++;
}
void doubly_sorted_delete(Node **head, Node *target) {
    if (target == NULL)
        return;
    if (target->prev != NULL)
        target->prev->next = target->next;
    else
        *head = target->next;
    if (target->next != NULL)
        target->next->prev = target->prev;
    free(target);
    operations++;
}
Node *doubly_sorted_minimum(Node *head) {
    operations++;
    return head;
}
Node *doubly_sorted_maximum(Node *head) {
    Node *current = head;
    while (current != NULL && current->next != NULL) {
        current = current->next;
        operations++;
    }
    return current;
}
Node *doubly_sorted_predecessor(Node *target) {
    operations++;
    return target == NULL ? NULL : target->prev;
}
Node *doubly_sorted_successor(Node *target) {
    operations++;
    return target == NULL ? NULL : target->next;
}
void free_list(Node *head) {
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}
void build_singly_sorted(Node **head, int n) {
    Node *tail = NULL;
    for (int i = 0; i < n; i++) {
        Node *node = create_node(i);
        if (tail) tail->next = node; else *head = node;
        tail = node;
    }
}
void build_doubly_sorted(Node **head, int n) {
    Node *tail = NULL;
    for (int i = 0; i < n; i++) {
        Node *node = create_node(i);
        node->prev = tail;
        if (tail) tail->next = node; else *head = node;
        tail = node;
    }
}
static const char *OP_NAMES[7] = {
    "Search", "Insert", "Delete", "Minimum", "Maximum", "Predecessor", "Successor"
};
static const char *COL_NAMES[6] = {
    "UnsortArr", "SortArr", "SglyUnsort", "SglySort", "DblyUnsort", "DblySort"
};
void print_table_for_n(FILE *out, int n, long long table[7][6]) {
    fprintf(out, "\n================ n = %d ================\n", n);
    fprintf(out, "%-12s", "Operation");
    for (int c = 0; c < 6; c++) fprintf(out, "%12s", COL_NAMES[c]);
    fprintf(out, "\n");
    for (int r = 0; r < 7; r++) {
        fprintf(out, "%-12s", OP_NAMES[r]);
        for (int c = 0; c < 6; c++) fprintf(out, "%12lld", table[r][c]);
        fprintf(out, "\n");
    }
}
int main(void)
{
    FILE *search_csv = fopen("search.csv", "w");
    FILE *insert_csv = fopen("insert.csv", "w");
    FILE *delete_csv = fopen("delete.csv", "w");
    FILE *minimum_csv = fopen("minimum.csv", "w");
    FILE *maximum_csv = fopen("maximum.csv", "w");
    FILE *predecessor_csv = fopen("predecessor.csv", "w");
    FILE *successor_csv = fopen("successor.csv", "w");
    FILE *tables_txt = fopen("tables.txt", "w");
    if (!search_csv || !insert_csv || !delete_csv ||
        !minimum_csv || !maximum_csv ||
        !predecessor_csv || !successor_csv || !tables_txt)
    {
        printf("Error creating CSV files.\n");
        return EXIT_FAILURE;
    }
    fprintf(search_csv,
            "n,Unsorted Array,Sorted Array,Singly Unsorted,Singly Sorted,Doubly Unsorted,Doubly Sorted\n");
    fprintf(insert_csv,
            "n,Unsorted Array,Sorted Array,Singly Unsorted,Singly Sorted,Doubly Unsorted,Doubly Sorted\n");
    fprintf(delete_csv,
            "n,Unsorted Array,Sorted Array,Singly Unsorted,Singly Sorted,Doubly Unsorted,Doubly Sorted\n");
    fprintf(minimum_csv,
            "n,Unsorted Array,Sorted Array,Singly Unsorted,Singly Sorted,Doubly Unsorted,Doubly Sorted\n");
    fprintf(maximum_csv,
            "n,Unsorted Array,Sorted Array,Singly Unsorted,Singly Sorted,Doubly Unsorted,Doubly Sorted\n");
    fprintf(predecessor_csv,
            "n,Unsorted Array,Sorted Array,Singly Unsorted,Singly Sorted,Doubly Unsorted,Doubly Sorted\n");
    fprintf(successor_csv,
            "n,Unsorted Array,Sorted Array,Singly Unsorted,Singly Sorted,Doubly Unsorted,Doubly Sorted\n");
    long long table[7][6];
    for (int n = STEP; n <= MAX_N; n += STEP)
    {
        int *unsorted_array = malloc((n + 2) * sizeof(int));
        int *sorted_array = malloc((n + 2) * sizeof(int));
        Node *singly_unsorted = NULL;
        Node *singly_sorted = NULL;
        Node *doubly_unsorted = NULL;
        Node *doubly_sorted = NULL;
        if (!unsorted_array || !sorted_array)
        {
            printf("Memory allocation failed.\n");
            return EXIT_FAILURE;
        }
        for (int i = 0; i < n; i++)
        {
            unsorted_array[i] = i;
            sorted_array[i] = i;
            singly_unsorted_insert(&singly_unsorted, i);
            doubly_unsorted_insert(&doubly_unsorted, i);
        }
        build_singly_sorted(&singly_sorted, n);
        build_doubly_sorted(&doubly_sorted, n);
        operations = 0;
        unsorted_array_search(unsorted_array, n, -1);
        long long ua = operations;
        operations = 0;
        sorted_array_search(sorted_array, n, -1);
        long long sa = operations;
        operations = 0;
        singly_unsorted_search(singly_unsorted, -1);
        long long sus = operations;
        operations = 0;
        singly_sorted_search(singly_sorted, n);
        long long sss = operations;
        operations = 0;
        doubly_unsorted_search(doubly_unsorted, -1);
        long long dus = operations;
        operations = 0;
        doubly_sorted_search(doubly_sorted, n);
        long long dss = operations;
        fprintf(search_csv, "%d,%lld,%lld,%lld,%lld,%lld,%lld\n",
                n, ua, sa, sus, sss, dus, dss);
        table[0][0]=ua; table[0][1]=sa; table[0][2]=sus; table[0][3]=sss; table[0][4]=dus; table[0][5]=dss;
        int n1 = n;
        int n2 = n;
        operations = 0;
        unsorted_array_insert(unsorted_array, &n1, n);
        ua = operations;
        operations = 0;
        sorted_array_insert(sorted_array, &n2, -1);
        sa = operations;
        operations = 0;
        singly_unsorted_insert(&singly_unsorted, n);
        sus = operations;
        operations = 0;
        singly_sorted_insert(&singly_sorted, n);
        sss = operations;
        operations = 0;
        doubly_unsorted_insert(&doubly_unsorted, n);
        dus = operations;
        operations = 0;
        doubly_sorted_insert(&doubly_sorted, n);
        dss = operations;
        fprintf(insert_csv, "%d,%lld,%lld,%lld,%lld,%lld,%lld\n",
                n, ua, sa, sus, sss, dus, dss);
        table[1][0]=ua; table[1][1]=sa; table[1][2]=sus; table[1][3]=sss; table[1][4]=dus; table[1][5]=dss;
        operations = 0;
        unsorted_array_delete(unsorted_array, &n1, 0);
        ua = operations;
        operations = 0;
        sorted_array_delete(sorted_array, &n2, 0);
        sa = operations;
        Node *target = singly_unsorted;
        while (target != NULL && target->next != NULL)
            target = target->next;
        operations = 0;
        singly_unsorted_delete(&singly_unsorted, target);
        sus = operations;
        target = singly_sorted;
        while (target != NULL && target->next != NULL)
            target = target->next;
        operations = 0;
        singly_sorted_delete(&singly_sorted, target);
        sss = operations;
        target = doubly_unsorted;
        operations = 0;
        doubly_unsorted_delete(&doubly_unsorted, target);
        dus = operations;
        target = doubly_sorted;
        operations = 0;
        doubly_sorted_delete(&doubly_sorted, target);
        dss = operations;
        fprintf(delete_csv, "%d,%lld,%lld,%lld,%lld,%lld,%lld\n",
                n, ua, sa, sus, sss, dus, dss);
        table[2][0]=ua; table[2][1]=sa; table[2][2]=sus; table[2][3]=sss; table[2][4]=dus; table[2][5]=dss;
        operations = 0;
        unsorted_array_minimum(unsorted_array, n1);
        ua = operations;
        operations = 0;
        sorted_array_minimum(sorted_array, n2);
        sa = operations;
        operations = 0;
        singly_unsorted_minimum(singly_unsorted);
        sus = operations;
        operations = 0;
        singly_sorted_minimum(singly_sorted);
        sss = operations;
        operations = 0;
        doubly_unsorted_minimum(doubly_unsorted);
        dus = operations;
        operations = 0;
        doubly_sorted_minimum(doubly_sorted);
        dss = operations;
        fprintf(minimum_csv, "%d,%lld,%lld,%lld,%lld,%lld,%lld\n",
                n, ua, sa, sus, sss, dus, dss);
        table[3][0]=ua; table[3][1]=sa; table[3][2]=sus; table[3][3]=sss; table[3][4]=dus; table[3][5]=dss;
        operations = 0;
        unsorted_array_maximum(unsorted_array, n1);
        ua = operations;
        operations = 0;
        sorted_array_maximum(sorted_array, n2);
        sa = operations;
        operations = 0;
        singly_unsorted_maximum(singly_unsorted);
        sus = operations;
        operations = 0;
        singly_sorted_maximum(singly_sorted);
        sss = operations;
        operations = 0;
        doubly_unsorted_maximum(doubly_unsorted);
        dus = operations;
        operations = 0;
        doubly_sorted_maximum(doubly_sorted);
        dss = operations;
        fprintf(maximum_csv, "%d,%lld,%lld,%lld,%lld,%lld,%lld\n",
                n, ua, sa, sus, sss, dus, dss);
        table[4][0]=ua; table[4][1]=sa; table[4][2]=sus; table[4][3]=sss; table[4][4]=dus; table[4][5]=dss;
        int key = n / 2;
        operations = 0;
        unsorted_array_predecessor(unsorted_array, n1, key);
        ua = operations;
        int idx_pred = sorted_array_search(sorted_array, n2, key);
        operations = 0;
        sorted_array_predecessor(sorted_array, n2, idx_pred);
        sa = operations;
        target = singly_unsorted;
        while (target != NULL && target->key != key)
            target = target->next;
        operations = 0;
        singly_unsorted_predecessor(singly_unsorted, target);
        sus = operations;
        target = singly_sorted;
        while (target != NULL && target->key != key)
            target = target->next;
        operations = 0;
        singly_sorted_predecessor(singly_sorted, target);
        sss = operations;
        target = doubly_unsorted;
        while (target != NULL && target->key != key)
            target = target->next;
        operations = 0;
        doubly_unsorted_predecessor(doubly_unsorted, target);
        dus = operations;
        target = doubly_sorted;
        while (target != NULL && target->key != key)
            target = target->next;
        operations = 0;
        doubly_sorted_predecessor(target);
        dss = operations;
        fprintf(predecessor_csv, "%d,%lld,%lld,%lld,%lld,%lld,%lld\n",
                n, ua, sa, sus, sss, dus, dss);
        table[5][0]=ua; table[5][1]=sa; table[5][2]=sus; table[5][3]=sss; table[5][4]=dus; table[5][5]=dss;
        operations = 0;
        unsorted_array_successor(unsorted_array, n1, key);
        ua = operations;
        int idx_succ = sorted_array_search(sorted_array, n2, key);
        operations = 0;
        sorted_array_successor(sorted_array, n2, idx_succ);
        sa = operations;
        target = singly_unsorted;
        while (target != NULL && target->key != key)
            target = target->next;
        operations = 0;
        singly_unsorted_successor(singly_unsorted, target);
        sus = operations;
        target = singly_sorted;
        while (target != NULL && target->key != key)
            target = target->next;
        operations = 0;
        singly_sorted_successor(target);
        sss = operations;
        target = doubly_unsorted;
        while (target != NULL && target->key != key)
            target = target->next;
        operations = 0;
        doubly_unsorted_successor(doubly_unsorted, target);
        dus = operations;
        target = doubly_sorted;
        while (target != NULL && target->key != key)
            target = target->next;
        operations = 0;
        doubly_sorted_successor(target);
        dss = operations;
        fprintf(successor_csv, "%d,%lld,%lld,%lld,%lld,%lld,%lld\n",
                n, ua, sa, sus, sss, dus, dss);
        table[6][0]=ua; table[6][1]=sa; table[6][2]=sus; table[6][3]=sss; table[6][4]=dus; table[6][5]=dss;
        print_table_for_n(stdout, n, table);
        print_table_for_n(tables_txt, n, table);
        free(unsorted_array);
        free(sorted_array);
        free_list(singly_unsorted);
        free_list(singly_sorted);
        free_list(doubly_unsorted);
        free_list(doubly_sorted);
    }
    fclose(search_csv);
    fclose(insert_csv);
    fclose(delete_csv);
    fclose(minimum_csv);
    fclose(maximum_csv);
    fclose(predecessor_csv);
    fclose(successor_csv);
    fclose(tables_txt);
    printf("\nCSV files created successfully:\n");
    return EXIT_SUCCESS;
}