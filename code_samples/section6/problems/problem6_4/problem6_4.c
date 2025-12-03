#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/* ====== ListNode definition ====== */

typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

/* ====== NodeMinHeap definition ====== */

typedef struct {
    ListNode **data;  // array of ListNode* (min-heap by node->val)
    int size;
    int capacity;
} NodeMinHeap;

/* ====== Heap helpers ====== */

static void node_heap_swap(ListNode **a, ListNode **b) {
    ListNode *tmp = *a;
    *a = *b;
    *b = tmp;
}

static void node_heap_ensure_capacity(NodeMinHeap *h) {
    if (h->size < h->capacity) return;
    int newCap = (h->capacity > 0) ? h->capacity * 2 : 1;
    ListNode **nd = (ListNode **)realloc(h->data, newCap * sizeof(ListNode *));
    if (!nd) {
        fprintf(stderr, "Out of memory in node_heap_ensure_capacity\n");
        exit(EXIT_FAILURE);
    }
    h->data = nd;
    h->capacity = newCap;
}

static void node_heap_bubble_up(NodeMinHeap *h, int i) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->data[i]->val >= h->data[p]->val) break; // min-heap
        node_heap_swap(&h->data[i], &h->data[p]);
        i = p;
    }
}

static void node_heap_heapify_down(NodeMinHeap *h, int i) {
    int n = h->size;
    for (;;) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && h->data[left]->val < h->data[smallest]->val) {
            smallest = left;
        }
        if (right < n && h->data[right]->val < h->data[smallest]->val) {
            smallest = right;
        }
        if (smallest == i) break;
        node_heap_swap(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
}

/* ====== Public NodeMinHeap API ====== */

void node_heap_init(NodeMinHeap *h, int initialCapacity) {
    h->size = 0;
    h->capacity = (initialCapacity > 0) ? initialCapacity : 1;
    h->data = (ListNode **)malloc(h->capacity * sizeof(ListNode *));
    if (!h->data) {
        fprintf(stderr, "Out of memory in node_heap_init\n");
        exit(EXIT_FAILURE);
    }
}

void node_heap_free(NodeMinHeap *h) {
    free(h->data);
    h->data = NULL;
    h->size = 0;
    h->capacity = 0;
}

void node_heap_push(NodeMinHeap *h, ListNode *node) {
    node_heap_ensure_capacity(h);
    h->data[h->size] = node;
    node_heap_bubble_up(h, h->size);
    h->size++;
}

ListNode *node_heap_peek(const NodeMinHeap *h) {
    if (h->size == 0) return NULL;
    return h->data[0];
}

ListNode *node_heap_pop(NodeMinHeap *h) {
    if (h->size == 0) return NULL;
    ListNode *root = h->data[0];
    h->size--;
    if (h->size > 0) {
        h->data[0] = h->data[h->size];
        node_heap_heapify_down(h, 0);
    }
    return root;
}

/* ====== mergeKLists implementation ====== */

ListNode *mergeKLists(ListNode **lists, int k) {
    NodeMinHeap h;
    node_heap_init(&h, k);

    for (int i = 0; i < k; i++) {
        if (lists[i] != NULL) {
            node_heap_push(&h, lists[i]);
        }
    }

    ListNode dummy = {0, NULL};
    ListNode *tail = &dummy;

    while (h.size > 0) {
        ListNode *node = node_heap_pop(&h);
        tail->next = node;
        tail = tail->next;
        if (node->next != NULL) {
            node_heap_push(&h, node->next);
        }
    }

    node_heap_free(&h);
    return dummy.next;
}

/* ====== Test helpers ====== */

static ListNode *create_list_from_array(const int *arr, int n) {
    ListNode *head = NULL;
    ListNode *tail = NULL;
    for (int i = 0; i < n; i++) {
        ListNode *node = (ListNode *)malloc(sizeof(ListNode));
        if (!node) {
            fprintf(stderr, "Out of memory in create_list_from_array\n");
            exit(EXIT_FAILURE);
        }
        node->val = arr[i];
        node->next = NULL;
        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

static void print_list(const char *label, ListNode *head) {
    printf("%s", label);
    for (ListNode *cur = head; cur != NULL; cur = cur->next) {
        printf("%d ", cur->val);
    }
    printf("\n");
}

static void free_list(ListNode *head) {
    while (head) {
        ListNode *next = head->next;
        free(head);
        head = next;
    }
}

/* ====== Test function ====== */

void test_mergeKLists(void) {
    // Example 1: three non-empty sorted lists
    int a1[] = {1, 4, 7};
    int a2[] = {2, 5, 8};
    int a3[] = {3, 6, 9};

    ListNode *l1 = create_list_from_array(a1, (int)(sizeof(a1)/sizeof(a1[0])));
    ListNode *l2 = create_list_from_array(a2, (int)(sizeof(a2)/sizeof(a2[0])));
    ListNode *l3 = create_list_from_array(a3, (int)(sizeof(a3)/sizeof(a3[0])));

    ListNode *lists1[] = { l1, l2, l3 };
    int k1 = (int)(sizeof(lists1)/sizeof(lists1[0]));

    printf("=== Test 1: 3 sorted non-empty lists ===\n");
    print_list("List 1: ", l1);
    print_list("List 2: ", l2);
    print_list("List 3: ", l3);

    ListNode *merged1 = mergeKLists(lists1, k1);
    print_list("Merged: ", merged1);
    printf("(Expected: 1 2 3 4 5 6 7 8 9)\n\n");

    free_list(merged1); // merged list reuses original nodes

    // Example 2: some empty lists
    int b1[] = {1, 3, 5};
    int b3[] = {2, 4, 6, 8};

    ListNode *m1 = create_list_from_array(b1, (int)(sizeof(b1)/sizeof(b1[0])));
    ListNode *m2 = NULL;
    ListNode *m3 = create_list_from_array(b3, (int)(sizeof(b3)/sizeof(b3[0])));
    ListNode *m4 = NULL;

    ListNode *lists2[] = { m1, m2, m3, m4 };
    int k2 = (int)(sizeof(lists2)/sizeof(lists2[0]));

    printf("=== Test 2: mix of empty and non-empty lists ===\n");
    print_list("List 1: ", m1);
    print_list("List 2: ", m2);
    print_list("List 3: ", m3);
    print_list("List 4: ", m4);

    ListNode *merged2 = mergeKLists(lists2, k2);
    print_list("Merged: ", merged2);
    printf("(Expected: 1 2 3 4 5 6 8)\n\n");

    free_list(merged2);

    // Example 3: all empty
    ListNode *lists3[] = { NULL, NULL };
    int k3 = (int)(sizeof(lists3)/sizeof(lists3[0]));

    printf("=== Test 3: all lists empty ===\n");
    ListNode *merged3 = mergeKLists(lists3, k3);
    print_list("Merged: ", merged3);
    printf("(Expected: <empty line>)\n\n");
    // merged3 is NULL; nothing to free
}

/* ====== main ====== */

int main(void) {
    test_mergeKLists();
    return 0;
}
