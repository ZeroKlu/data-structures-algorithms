#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/*
    This file implements "merge k sorted linked lists" using a custom
    min-heap of ListNode* (ordered by node->val).

    High-level approach:
      - Maintain a min-heap containing the current head node of each list.
      - Repeatedly extract the smallest node from the heap and append it to the
        merged result.
      - If the extracted node has a next pointer, push node->next into the heap.
      - Continue until the heap is empty.

    Why a min-heap?
      - Each extraction gives the next smallest value among the current k "front"
        nodes, making the merge efficient.

    Complexity:
      - Let N be the total number of nodes across all lists.
      - Heap size is at most k.
      - Each push/pop is O(log k).
      - Total time: O(N log k), extra space: O(k) for the heap array.

    Memory model:
      - mergeKLists() REUSES the original nodes; it does not allocate new nodes.
      - The test harness allocates the lists and frees the merged list afterward.
*/

/* ====== ListNode definition ====== */

/*
    Singly linked list node.
*/
typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

/* ====== NodeMinHeap definition ====== */

/*
    NodeMinHeap stores pointers to ListNode.
    Ordering is by node->val (min-heap: smallest value at index 0).
*/
typedef struct {
    ListNode **data;  // backing array of ListNode* in heap order
    int size;         // number of elements currently in heap
    int capacity;     // allocated capacity of data[]
} NodeMinHeap;

/* ====== Heap helpers ====== */

/*
    node_heap_swap
    --------------
    Swaps two ListNode* entries in the heap array.
*/
static void node_heap_swap(ListNode **a, ListNode **b) {
    ListNode *tmp = *a;
    *a = *b;
    *b = tmp;
}

/*
    node_heap_ensure_capacity
    -------------------------
    Grows heap backing storage when size reaches capacity.

    Strategy:
      - If size < capacity: no growth needed.
      - Else: double capacity (or set to 1 if current capacity is 0).
      - realloc the pointer array (NOT the nodes themselves).

    Safety:
      - Exits on allocation failure.
*/
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

/*
    node_heap_bubble_up
    -------------------
    Restores min-heap property after insertion.

    Min-heap invariant:
      - Every parent node has value <= its children.

    When we insert, we append the new item at the end (index i),
    then swap it upward while it's smaller than its parent.

    Complexity: O(log n) where n = heap size.
*/
static void node_heap_bubble_up(NodeMinHeap *h, int i) {
    while (i > 0) {
        int p = (i - 1) / 2; // parent index

        // If child >= parent, heap property is satisfied.
        if (h->data[i]->val >= h->data[p]->val) break;

        // Otherwise swap and continue upward.
        node_heap_swap(&h->data[i], &h->data[p]);
        i = p;
    }
}

/*
    node_heap_heapify_down
    ----------------------
    Restores min-heap property after removing the root.

    Pop operation typically:
      - Replace root with last element
      - Decrease size
      - Push the new root down by swapping with the smaller child until valid.

    Complexity: O(log n) where n = heap size.
*/
static void node_heap_heapify_down(NodeMinHeap *h, int i) {
    int n = h->size;

    for (;;) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        // Choose the smallest among i and its children (if children exist).
        if (left < n && h->data[left]->val < h->data[smallest]->val) {
            smallest = left;
        }
        if (right < n && h->data[right]->val < h->data[smallest]->val) {
            smallest = right;
        }

        // If i is already the smallest, we're done.
        if (smallest == i) break;

        // Swap downward and continue from the child index.
        node_heap_swap(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
}

/* ====== Public NodeMinHeap API ====== */

/*
    node_heap_init
    --------------
    Initializes a NodeMinHeap with a given initial capacity.

    Parameters:
      - initialCapacity: number of slots to pre-allocate for ListNode* pointers.
        If <= 0, defaults to 1.
*/
void node_heap_init(NodeMinHeap *h, int initialCapacity) {
    h->size = 0;
    h->capacity = (initialCapacity > 0) ? initialCapacity : 1;

    h->data = (ListNode **)malloc(h->capacity * sizeof(ListNode *));
    if (!h->data) {
        fprintf(stderr, "Out of memory in node_heap_init\n");
        exit(EXIT_FAILURE);
    }
}

/*
    node_heap_free
    --------------
    Frees the heap's backing array (not the ListNode objects!).
*/
void node_heap_free(NodeMinHeap *h) {
    free(h->data);
    h->data = NULL;
    h->size = 0;
    h->capacity = 0;
}

/*
    node_heap_push
    --------------
    Inserts a ListNode* into the heap (by node->val ordering).

    Important:
      - Caller must not pass NULL here; bubble_up compares node->val.
*/
void node_heap_push(NodeMinHeap *h, ListNode *node) {
    node_heap_ensure_capacity(h);
    h->data[h->size] = node;
    node_heap_bubble_up(h, h->size);
    h->size++;
}

/*
    node_heap_peek
    --------------
    Returns the smallest node (root) without removing it, or NULL if empty.
*/
ListNode *node_heap_peek(const NodeMinHeap *h) {
    if (h->size == 0) return NULL;
    return h->data[0];
}

/*
    node_heap_pop
    -------------
    Removes and returns the smallest node (root), or NULL if empty.

    Steps:
      - Save root
      - Decrease size
      - Move last item to root (if any remain)
      - Heapify down

    Note:
      - The returned node is still part of the original lists; we do not free it.
*/
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

/*
    mergeKLists
    -----------
    Merges k sorted singly linked lists into one sorted list.

    Parameters:
      - lists: array of ListNode* heads (may include NULLs)
      - k: number of lists in the array

    Returns:
      - head pointer to the merged sorted list, or NULL if all lists are empty.

    Implementation details:
      - Push the head of each non-empty list into the min-heap.
      - Repeatedly pop the smallest node and append it to the output list.
      - When popping a node, push its next node (if any) into the heap.

    Memory:
      - Uses a stack-allocated dummy node to simplify output list construction.
      - Reuses existing nodes (does not allocate new nodes).
*/
ListNode *mergeKLists(ListNode **lists, int k) {
    // Initialize heap with capacity k: at most one node per list is in the heap at once.
    NodeMinHeap h;
    node_heap_init(&h, k);

    // Seed heap with the head of each non-empty list.
    for (int i = 0; i < k; i++) {
        if (lists[i] != NULL) {
            node_heap_push(&h, lists[i]);
        }
    }

    // Dummy head simplifies appending: output starts at dummy.next
    ListNode dummy = {0, NULL};
    ListNode *tail = &dummy;

    // Keep extracting smallest node and pushing its successor.
    while (h.size > 0) {
        ListNode *node = node_heap_pop(&h);

        // Append node to merged list
        tail->next = node;
        tail = tail->next;

        // If this node came from a list that still has nodes, push the next one.
        if (node->next != NULL) {
            node_heap_push(&h, node->next);
        }
    }

    // IMPORTANT:
    // Ensure the merged list terminates (usually already true, but safe if inputs
    // had unexpected sharing/cycles).
    tail->next = NULL;

    node_heap_free(&h);
    return dummy.next;
}

/* ====== Test helpers ====== */

/*
    create_list_from_array
    ----------------------
    Allocates a new linked list whose values match arr[0..n-1].

    Returns:
      - head pointer (or NULL if n == 0).

    Ownership:
      - Caller owns the returned list and must free it (free_list),
        unless the nodes are reused/merged and freed elsewhere.
*/
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
            // First node becomes head and tail
            head = tail = node;
        } else {
            // Append to the tail
            tail->next = node;
            tail = node;
        }
    }

    return head;
}

/*
    print_list
    ----------
    Prints a linked list as space-separated values, preceded by a label.

    If head is NULL, it prints just the label and a newline.
*/
static void print_list(const char *label, ListNode *head) {
    printf("%s", label);
    for (ListNode *cur = head; cur != NULL; cur = cur->next) {
        printf("%d ", cur->val);
    }
    printf("\n");
}

/*
    free_list
    ---------
    Frees all nodes in a singly linked list.

    Assumes:
      - Nodes were allocated with malloc.
      - List is acyclic.

    Important:
      - In these tests, mergeKLists reuses the original nodes,
        so freeing the merged list frees the original lists too.
*/
static void free_list(ListNode *head) {
    while (head) {
        ListNode *next = head->next;
        free(head);
        head = next;
    }
}

/* ====== Test function ====== */

/*
    test_mergeKLists
    ----------------
    Runs a few basic tests of mergeKLists:

      Test 1: 3 non-empty sorted lists
      Test 2: mixture of empty and non-empty lists
      Test 3: all lists empty

    Each test prints input lists and merged output.
*/
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

    // merged list reuses original nodes, so freeing merged1 frees l1/l2/l3 too
    free_list(merged1);

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

    // merged3 is NULL, nothing to free
    (void)merged3;
    (void)k3; // k3 is computed for completeness; not otherwise used
}

/* ====== main ====== */

int main(void) {
    test_mergeKLists();
    return 0;
}
