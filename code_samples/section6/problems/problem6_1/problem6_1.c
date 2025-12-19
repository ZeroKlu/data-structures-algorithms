#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

/*
    Dynamic Min-Heap (Priority Queue) implementation for integers.

    A min-heap maintains the invariant:
        Every parent node value <= its children values.

    With this invariant:
        - The minimum element is always at index 0.
        - Insert is O(log n) (bubble up).
        - Pop-min is O(log n) (heapify down).
        - Peek-min is O(1).

    Storage is backed by a dynamically-resized array:
        - "size"     = number of elements currently in heap
        - "capacity" = allocated length of the data array
        - "data"     = heap elements stored in array form

    Array index relationships (0-based):
        parent(i) = (i - 1) / 2
        left(i)   = 2*i + 1
        right(i)  = 2*i + 2

    NOTE ABOUT EMPTY-HEAP SENTINEL:
        This implementation returns INT_MIN on peek/pop when empty.
        This is easy for demos, but ambiguous if INT_MIN is a valid value
        the caller may want to store.
        In production, you’d typically:
          - return a bool + out-parameter,
          - or keep an error flag,
          - or use errno,
          - or assert non-empty for pop/peek.
*/

typedef struct {
    int *data;      // Dynamic array storing heap elements in heap order
    int size;       // Number of valid elements currently stored in data[]
    int capacity;   // Allocated length of data[]
} MinHeap;

/*
    heap_init
    ---------
    Initializes a heap with an initial capacity (cap).

    Responsibilities:
      - Set size to 0
      - Set capacity to cap
      - Allocate backing array

    Assumptions / caveats:
      - cap should be > 0. If cap == 0, malloc(0) behavior is implementation-defined.
      - No error handling for malloc failure is shown (for simplicity).
*/
static void heap_init(MinHeap *h, int cap) {
    h->size = 0;
    h->capacity = cap;
    h->data = (int *)malloc(sizeof(int) * cap);
}

/*
    heap_free
    ---------
    Releases heap memory and resets fields to a safe "empty" state.
    Safe to call once after heap_init; calling multiple times is also safe
    because free(NULL) is defined (but this function sets data to NULL anyway).
*/
static void heap_free(MinHeap *h) {
    free(h->data);
    h->data = NULL;
    h->size = h->capacity = 0;
}

/*
    heap_ensure_capacity
    --------------------
    Ensures there is room for at least one more element.

    If size < capacity:
        - There is space; do nothing.
    Else:
        - Double the capacity and realloc the array.

    Notes:
      - This uses the classic doubling strategy for amortized O(1) growth.
      - No handling for realloc failure is included.
      - If capacity were ever 0, doubling would keep it 0; in production you’d
        guard and set to 1 or a minimum.
*/
static void heap_ensure_capacity(MinHeap *h) {
    if (h->size < h->capacity) return;

    h->capacity *= 2;
    h->data = (int *)realloc(h->data, sizeof(int) * h->capacity);
}

/*
    heap_swap
    ---------
    Swaps two integers in-place. Used by bubble_up and heapify_down.
*/
static void heap_swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/*
    bubble_up
    ---------
    Restores heap invariant after insertion.

    When inserting, we place the new value at the end of the array.
    This may violate the min-heap property with its parent.

    bubble_up repeatedly compares the node at index i with its parent:
        - If node < parent: swap and continue at parent's index.
        - Otherwise: heap property satisfied; stop.

    Complexity: O(log n) in the worst case (height of heap).
*/
static void bubble_up(MinHeap *h, int i) {
    while (i > 0) {
        int p = (i - 1) / 2;               // parent index
        if (h->data[i] >= h->data[p]) break; // heap property satisfied
        heap_swap(&h->data[i], &h->data[p]);
        i = p;                             // move up to continue checking
    }
}

/*
    heapify_down
    ------------
    Restores heap invariant after removal of the minimum element.

    When popping, we typically:
        - Save root value (min).
        - Move last element to root (index 0).
        - Decrease size.
        - "Heapify down" from root because the moved element might be larger
          than one or both children.

    heapify_down repeatedly:
        - Finds the smallest among (i, left child, right child).
        - If a child is smaller than current node, swap with the smallest child.
        - Continue from the swapped child index.
        - Stop when current node is smaller than both children (or no children).

    Complexity: O(log n) worst case.
*/
static void heapify_down(MinHeap *h, int i) {
    int n = h->size; // snapshot of current heap size for bounds checking
    for (;;) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        // If left child exists and is smaller, update smallest
        if (left < n && h->data[left] < h->data[smallest]) {
            smallest = left;
        }

        // If right child exists and is smaller than current smallest, update smallest
        if (right < n && h->data[right] < h->data[smallest]) {
            smallest = right;
        }

        // If the current node is already the smallest, heap property holds here
        if (smallest == i) break;

        // Otherwise, swap downwards and continue
        heap_swap(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
}

/*
    heap_insert
    -----------
    Inserts an element into the heap.

    Steps:
      1) Ensure array has space (grow if needed).
      2) Place the new element at the end (index size).
      3) bubble_up from that index to restore heap invariant.
      4) Increment size.

    Note:
      - bubble_up is done before size++ here, using the old size as index,
        which is correct because the new element is written at data[size].
*/
void heap_insert(MinHeap *h, int x) {
    heap_ensure_capacity(h);
    h->data[h->size] = x;          // append at end
    bubble_up(h, h->size);         // restore heap property
    h->size++;                     // commit the insertion
}

/*
    heap_peek
    ---------
    Returns the minimum element without removing it.

    If heap is empty:
      - returns INT_MIN as a sentinel.

    Complexity: O(1).
*/
int heap_peek(MinHeap *h) {
    return (h->size == 0) ? INT_MIN : h->data[0];
}

/*
    heap_pop
    --------
    Removes and returns the minimum element (root).

    If heap is empty:
      - returns INT_MIN sentinel.

    Steps:
      1) Save root (min).
      2) Decrease size.
      3) If there are still elements:
           - Move last element into root position.
           - heapify_down from root to restore heap property.
      4) Return saved root.

    Complexity: O(log n) when non-empty.
*/
int heap_pop(MinHeap *h) {
    if (h->size == 0) return INT_MIN;

    int root = h->data[0];      // min element
    h->size--;                  // remove one element from the heap

    if (h->size > 0) {
        // Move last element to root and fix heap property
        h->data[0] = h->data[h->size];
        heapify_down(h, 0);
    }

    return root;
}

/*
    Demo / test harness
    -------------------
    This main() exercises:
      - initialization with small capacity to trigger growth
      - insertion and peeking after each insert
      - popping all elements (should come out sorted ascending)
      - behavior of peek/pop on empty heap
      - reuse of heap after emptying
      - freeing heap memory
*/
int main(void) {
    MinHeap h;
    heap_init(&h, 4);  // small initial capacity to test growth

    printf("=== Testing dynamic MinHeap ===\n");
    printf("Initial size: %d, capacity: %d\n\n", h.size, h.capacity);

    int values_to_insert[] = {5, 3, 8, 1, 7, 2};
    int count = (int)(sizeof(values_to_insert) / sizeof(values_to_insert[0]));

    printf("Inserting values: ");
    for (int i = 0; i < count; i++) {
        printf("%d ", values_to_insert[i]);

        // Insert and immediately report the heap status
        heap_insert(&h, values_to_insert[i]);
        printf("\n  After inserting %d -> size: %d, capacity: %d, min: %d",
               values_to_insert[i],
               h.size,
               h.capacity,
               heap_peek(&h));
    }
    printf("\n\n");

    printf("Heap after all inserts:\n");
    printf("  size: %d, capacity: %d\n", h.size, h.capacity);
    printf("  peek (min): %d\n\n", heap_peek(&h));

    /*
        Pop all elements to verify sorted order.

        For a correct min-heap, repeatedly popping should produce non-decreasing
        output (ascending order).
    */
    printf("Popping all elements in sorted order: ");
    while (1) {
        int x = heap_pop(&h);
        if (x == INT_MIN) {
            break;  // heap empty (sentinel check)
        }
        printf("%d ", x);
    }
    printf("\n\n");

    printf("After popping everything:\n");
    printf("  size: %d, capacity: %d\n", h.size, h.capacity);

    // Demonstrate empty-heap sentinel behavior
    printf("  peek on empty heap: %d\n", heap_peek(&h));
    printf("  pop on empty heap: %d\n\n", heap_pop(&h));

    /*
        Test inserting again after being emptied.
        Capacity remains whatever it grew to; size is currently 0.
    */
    printf("Inserting again after emptying heap: 10, 4, 6\n");
    heap_insert(&h, 10);
    heap_insert(&h, 4);
    heap_insert(&h, 6);

    printf("  size: %d, capacity: %d\n", h.size, h.capacity);
    printf("  peek (min): %d\n", heap_peek(&h));

    printf("Final pop sequence: ");
    while (1) {
        int x = heap_pop(&h);
        if (x == INT_MIN) break;
        printf("%d ", x);
    }
    printf("\n");

    // Clean up dynamic memory
    heap_free(&h);
    printf("\nHeap freed. size: %d, capacity: %d\n", h.size, h.capacity);

    return 0;
}
