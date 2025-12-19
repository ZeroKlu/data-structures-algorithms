#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
    This file implements:
      1) A dynamic array-backed MIN-HEAP for integers.
      2) A "KthLargest" abstraction (like the common interview / LeetCode problem)
         that maintains the k-th largest element in a stream of values.

    Key idea (KthLargest):
      - Maintain a MIN-HEAP of size at most k containing the k largest elements seen so far.
      - The smallest element in that heap (heap root) is the k-th largest overall.
      - When a new value arrives:
          * If heap size < k: push it.
          * Else if value > heap root: pop root and push value.
          * Else: ignore it (not in the top k).
      - The k-th largest is always heap_peek().

    Complexity:
      - Heap operations are O(log k) when heap size is bounded by k.
      - kth_add runs in O(log k) time (amortized), O(k) space.
*/

/* ===== Min-heap for ints ===== */

/*
    MinHeap
    -------
    Array-backed binary heap storing integers.

    Fields:
      - data: pointer to contiguous int array storing heap elements
      - size: number of valid elements in the heap (prefix of data[])
      - capacity: allocated size of data[] (>= size)
*/
typedef struct {
    int *data;
    int size;
    int capacity;
} MinHeap;

/*
    heap_init
    ---------
    Initializes the heap with an initial capacity.

    Parameters:
      - h: heap to initialize
      - cap: desired initial capacity; if cap <= 0, we force capacity to 1

    Safety:
      - Exits if malloc fails.
*/
static void heap_init(MinHeap *h, int cap) {
    h->size = 0;
    h->capacity = (cap > 0) ? cap : 1;

    h->data = (int *)malloc(sizeof(int) * h->capacity);
    if (!h->data) {
        fprintf(stderr, "Out of memory in heap_init\n");
        exit(EXIT_FAILURE);
    }
}

/*
    heap_free
    ---------
    Releases heap backing storage and resets fields.

    Note:
      - Safe to call multiple times (free(NULL) is safe, but we also null out data).
*/
static void heap_free(MinHeap *h) {
    free(h->data);
    h->data = NULL;
    h->size = 0;
    h->capacity = 0;
}

/*
    heap_ensure_capacity
    --------------------
    Ensures there's space for one more element.

    Growth strategy:
      - If size < capacity: do nothing
      - Else: double the capacity and realloc

    Safety:
      - Exits on realloc failure.
*/
static void heap_ensure_capacity(MinHeap *h) {
    if (h->size < h->capacity) return;

    h->capacity *= 2;

    int *nd = (int *)realloc(h->data, sizeof(int) * h->capacity);
    if (!nd) {
        fprintf(stderr, "Out of memory in heap_ensure_capacity\n");
        exit(EXIT_FAILURE);
    }
    h->data = nd;
}

/*
    heap_swap
    ---------
    Swaps two integers in place.

    Used internally by bubble_up / heapify_down.
*/
static void heap_swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/*
    bubble_up
    ---------
    Restores min-heap property after inserting an element at index i.

    Min-heap invariant:
      - Parent value <= child value for all nodes.

    After insertion:
      - The new element is placed at the end (index i),
        then swapped upward while smaller than its parent.

    Complexity: O(log n), where n = heap size.
*/
static void bubble_up(MinHeap *h, int i) {
    while (i > 0) {
        int p = (i - 1) / 2; // parent index

        // If child >= parent, heap property is satisfied.
        if (h->data[i] >= h->data[p]) break;

        // Otherwise swap child and parent, and continue.
        heap_swap(&h->data[i], &h->data[p]);
        i = p;
    }
}

/*
    heapify_down
    ------------
    Restores min-heap property after removing the root.

    Pop pattern:
      - Move last element to index 0,
      - then "sift down" by swapping with the smaller child
        until the heap property holds.

    Complexity: O(log n), where n = heap size.
*/
static void heapify_down(MinHeap *h, int i) {
    int n = h->size;

    for (;;) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        // Find the smallest among i and its children.
        if (left < n && h->data[left] < h->data[smallest]) {
            smallest = left;
        }
        if (right < n && h->data[right] < h->data[smallest]) {
            smallest = right;
        }

        // If i is already the smallest, we're done.
        if (smallest == i) break;

        // Swap downward and continue.
        heap_swap(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
}

/*
    heap_insert
    -----------
    Inserts x into the min-heap.

    Steps:
      - Ensure capacity
      - Place x at end
      - Bubble up to restore heap property
      - Increment size
*/
static void heap_insert(MinHeap *h, int x) {
    heap_ensure_capacity(h);
    h->data[h->size] = x;
    bubble_up(h, h->size);
    h->size++;
}

/*
    heap_peek
    ---------
    Returns the minimum element (root) without removing it.

    Sentinel behavior:
      - Returns INT_MIN when heap is empty.

    NOTE:
      - Using INT_MIN as an "empty" sentinel can be ambiguous if INT_MIN is a valid data value.
      - In this file, it’s used mainly for simplicity in tests.
*/
static int heap_peek(MinHeap *h) {
    return (h->size == 0) ? INT_MIN : h->data[0];
}

/*
    heap_pop
    --------
    Removes and returns the minimum element (root).

    Sentinel behavior:
      - Returns INT_MIN if heap is empty.

    Steps:
      - Save root
      - Decrement size
      - Move last element to root (if any remain)
      - Heapify down

    Same caveat as heap_peek about INT_MIN.
*/
static int heap_pop(MinHeap *h) {
    if (h->size == 0) return INT_MIN;

    int root = h->data[0];
    h->size--;

    if (h->size > 0) {
        h->data[0] = h->data[h->size];
        heapify_down(h, 0);
    }

    return root;
}

/* ===== KthLargest abstraction ===== */

/*
    KthLargest
    ----------
    Maintains the k-th largest element of a stream.

    Invariant:
      - heap contains the k largest elements seen so far (or fewer if not enough elements).
      - heap is a MIN-HEAP, so heap_peek() gives the smallest among those kept values,
        i.e., the current k-th largest.

    Fields:
      - k: desired rank (e.g., k=3 means "3rd largest")
      - heap: min-heap storing the top-k largest values
*/
typedef struct {
    int k;
    MinHeap heap; // min-heap of size at most k
} KthLargest;

/*
    kth_init
    --------
    Initializes the KthLargest structure and processes an initial array of values.

    Parameters:
      - obj: pointer to KthLargest to initialize
      - k: rank of interest (k-th largest)
      - nums: initial values array (may be NULL if n==0)
      - n: number of elements in nums

    Behavior:
      - Builds the internal heap such that it contains the k largest values
        from nums (or all values if n < k).
*/
void kth_init(KthLargest *obj, int k, const int *nums, int n) {
    obj->k = k;

    // Initialize heap with capacity at least 1 to keep malloc safe.
    heap_init(&obj->heap, k > 0 ? k : 1);

    // Process initial numbers and maintain "top k" heap.
    for (int i = 0; i < n; i++) {
        if (obj->heap.size < k) {
            // Fill up heap until we have k elements.
            heap_insert(&obj->heap, nums[i]);
        } else if (k > 0 && nums[i] > heap_peek(&obj->heap)) {
            /*
                Heap already has k elements.

                heap_peek() is the smallest among the current top k.
                If nums[i] is larger, it belongs in the top k:
                  - remove the smallest
                  - insert the new value
            */
            heap_pop(&obj->heap);
            heap_insert(&obj->heap, nums[i]);
        }
        // Else nums[i] is not among the top k; ignore it.
    }
}

/*
    kth_add
    -------
    Adds a new value to the stream and returns the current k-th largest value.

    Parameters:
      - obj: KthLargest instance
      - val: new value to add

    Returns:
      - The current k-th largest value (heap root) after insertion.

    Edge cases:
      - If obj->k == 0, "k-th largest" is undefined.
        This implementation returns 0 as a placeholder to avoid undefined behavior.
        (A production API would likely reject k <= 0 at initialization.)
*/
int kth_add(KthLargest *obj, int val) {
    if (obj->k == 0) return 0; // undefined; placeholder behavior

    if (obj->heap.size < obj->k) {
        // Not enough elements yet to reach k: just insert.
        heap_insert(&obj->heap, val);
    } else if (val > heap_peek(&obj->heap)) {
        // If val belongs in the top k, replace the current cutoff.
        heap_pop(&obj->heap);
        heap_insert(&obj->heap, val);
    }

    // The root is the k-th largest (smallest among the top k).
    return heap_peek(&obj->heap);
}

/*
    kth_free
    --------
    Releases heap resources held by the KthLargest object.
*/
void kth_free(KthLargest *obj) {
    heap_free(&obj->heap);
}

/* ===== Test code ===== */

/*
    print_array
    -----------
    Prints an integer array with a label for debugging/tests.
*/
static void print_array(const char *label, const int *a, int n) {
    printf("%s", label);
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    printf("\n");
}

/*
    test_kthLargest
    ---------------
    Demonstrates KthLargest behavior using a standard example.

    Example:
      k = 3, initial nums = [4, 5, 8, 2]

    Expected output sequence:
      add(3)  -> 4
      add(5)  -> 5
      add(10) -> 5
      add(9)  -> 8
      add(4)  -> 8
*/
void test_kthLargest(void) {
    // Standard example: k = 3, nums = [4, 5, 8, 2]
    int nums[] = {4, 5, 8, 2};
    int n = (int)(sizeof(nums) / sizeof(nums[0]));
    int k = 3;

    KthLargest kth;
    kth_init(&kth, k, nums, n);

    printf("=== Test KthLargest ===\n");
    print_array("Initial nums: ", nums, n);
    printf("k = %d\n", k);

    // After init, heap_peek is the k-th largest among the initial numbers.
    printf("kth largest after init: %d\n\n",
           kth.heap.size > 0 ? heap_peek(&kth.heap) : INT_MIN);

    int to_add[] = {3, 5, 10, 9, 4};
    int m = (int)(sizeof(to_add) / sizeof(to_add[0]));

    printf("Adding values:\n");
    for (int i = 0; i < m; i++) {
        int val = to_add[i];
        int kth_val = kth_add(&kth, val);
        printf("add(%d) -> kth largest = %d\n", val, kth_val);
    }

    /*
      For k = 3 and nums = [4,5,8,2], the expected sequence is:
        add(3)  -> 4
        add(5)  -> 5
        add(10) -> 5
        add(9)  -> 8
        add(4)  -> 8
    */

    kth_free(&kth);
}

/*
    main
    ----
    Entry point that runs the test harness.
*/
int main(void) {
    test_kthLargest();
    return 0;
}
