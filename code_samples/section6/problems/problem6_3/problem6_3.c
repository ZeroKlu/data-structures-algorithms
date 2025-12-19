#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
    This file implements:
      1) A dynamic, array-backed MaxHeap for integers
      2) kSmallest(): finds the k smallest elements in an array using a max-heap
      3) A test harness that validates kSmallest for:
           - k <= n
           - k > n

    High-level idea for kSmallest:
      - Maintain a max-heap of at most k elements.
      - The heap holds the k smallest elements seen so far.
      - The root of the max-heap is the *largest* among those k smallest.
      - When a new number x arrives:
          * If heap has fewer than k items, push x.
          * Else if x < heap.max (root), replace the root with x.
          * Else ignore x (it's not among the k smallest so far).

    Complexity:
      - Each insert/pop is O(log k)
      - We process n elements → O(n log k) time, O(k) extra space

    IMPORTANT NOTE ABOUT SENTINELS:
      - maxheap_peek/pop return INT_MIN when the heap is empty.
      - This is convenient for demos but ambiguous if INT_MIN is a valid element
        you might store. For production, prefer:
          * bool return + out parameter
          * or an error code
          * or assertions / exceptions (in other languages)
*/

/* ---------- MaxHeap definition ---------- */

/*
    MaxHeap structure:
      - data: backing array storing elements in heap order
      - size: number of elements currently in the heap
      - capacity: allocated length of data[]
*/
typedef struct {
    int *data;
    int size;
    int capacity;
} MaxHeap;

// Helpers for MaxHeap

/*
    maxheap_swap
    ------------
    Swaps two integers in-place. Used by bubble-up and heapify-down.
*/
static void maxheap_swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/*
    maxheap_ensure_capacity
    ----------------------
    Ensures the heap has room for at least one more element.

    Strategy:
      - If size < capacity, there is space; do nothing.
      - Otherwise grow capacity (double; or set to 1 if it was 0)
      - Use realloc to resize the backing array.

    Safety:
      - Checks realloc result and exits on allocation failure.
*/
static void maxheap_ensure_capacity(MaxHeap *h) {
    if (h->size < h->capacity) return;

    // Double capacity; if capacity is 0 (shouldn't happen after init), bump to 1.
    int newCap = (h->capacity > 0) ? h->capacity * 2 : 1;

    int *nd = (int *)realloc(h->data, newCap * sizeof(int));
    if (!nd) {
        fprintf(stderr, "Out of memory in maxheap_ensure_capacity\n");
        exit(EXIT_FAILURE);
    }

    h->data = nd;
    h->capacity = newCap;
}

/*
    maxheap_bubble_up
    -----------------
    Restores the max-heap invariant after insertion.

    Max-heap invariant:
      - Every parent >= its children.

    When a new element is appended at index i, it may be greater than its parent.
    We swap it upward until:
      - it reaches the root, or
      - parent >= child.

    Complexity: O(log n) worst case.
*/
static void maxheap_bubble_up(MaxHeap *h, int i) {
    while (i > 0) {
        int p = (i - 1) / 2;                 // parent index

        // For a max-heap, if child <= parent, the property is satisfied.
        if (h->data[i] <= h->data[p]) break;

        // Otherwise swap and continue upward.
        maxheap_swap(&h->data[i], &h->data[p]);
        i = p;
    }
}

/*
    maxheap_heapify_down
    --------------------
    Restores the max-heap invariant after removing the root.

    Typical pop:
      - Replace root with last element
      - Decrease size
      - Push the new root down until parent >= both children

    At each step:
      - Find the largest among node i and its children
      - Swap with the largest child if needed

    Complexity: O(log n) worst case.
*/
static void maxheap_heapify_down(MaxHeap *h, int i) {
    int n = h->size;

    for (;;) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        // Choose the largest among i, left child, right child (if children exist).
        if (left < n && h->data[left] > h->data[largest]) largest = left;
        if (right < n && h->data[right] > h->data[largest]) largest = right;

        // If i is already the largest, heap property holds here.
        if (largest == i) break;

        // Otherwise swap downward and continue.
        maxheap_swap(&h->data[i], &h->data[largest]);
        i = largest;
    }
}

/* Public MaxHeap API (as assumed in the comment) */

/*
    maxheap_init
    ------------
    Initializes a MaxHeap with an initial capacity.

    Parameters:
      - h: pointer to heap struct to initialize
      - cap: requested initial capacity; if <= 0 we choose 1

    Safety:
      - Checks malloc result and exits on failure.
*/
void maxheap_init(MaxHeap *h, int cap) {
    h->size = 0;
    h->capacity = cap > 0 ? cap : 1;

    h->data = (int *)malloc(sizeof(int) * h->capacity);
    if (!h->data) {
        fprintf(stderr, "Out of memory in maxheap_init\n");
        exit(EXIT_FAILURE);
    }
}

/*
    maxheap_free
    ------------
    Releases heap memory and resets fields to a safe empty state.
*/
void maxheap_free(MaxHeap *h) {
    free(h->data);
    h->data = NULL;
    h->size = 0;
    h->capacity = 0;
}

/*
    maxheap_insert
    --------------
    Inserts x into the heap.

    Steps:
      1) Ensure capacity
      2) Write x at index size (append position)
      3) Bubble up from that index
      4) Increment size
*/
void maxheap_insert(MaxHeap *h, int x) {
    maxheap_ensure_capacity(h);
    h->data[h->size] = x;
    maxheap_bubble_up(h, h->size);
    h->size++;
}

/*
    maxheap_peek
    ------------
    Returns the maximum element (root) without removing it.

    Returns INT_MIN if empty (sentinel).
*/
int maxheap_peek(const MaxHeap *h) {
    return (h->size == 0) ? INT_MIN : h->data[0];
}

/*
    maxheap_pop
    -----------
    Removes and returns the maximum element (root).

    Returns INT_MIN if empty (sentinel).

    Steps:
      - Save root
      - Decrease size
      - Move last element to root
      - Heapify down
*/
int maxheap_pop(MaxHeap *h) {
    if (h->size == 0) return INT_MIN;

    int root = h->data[0];
    h->size--;

    if (h->size > 0) {
        h->data[0] = h->data[h->size];
        maxheap_heapify_down(h, 0);
    }

    return root;
}

/* ---------- kSmallest as given ---------- */

/*
    kSmallest
    ---------
    Finds the k smallest elements of arr[0..n-1] using a max-heap of size at most k.

    Parameters:
      - arr: input array (read-only)
      - n: number of elements in arr
      - k: number of smallest elements to keep
      - heap: pointer to an initialized MaxHeap (will be mutated)
      - outCount: output parameter set to how many elements ended up in heap

    Behavior:
      - If k <= 0, the heap should remain empty (outCount will be 0).
      - If k > n, the heap will end up holding all n elements.

    Why max-heap for k smallest?
      - Keeping the *largest* among the current k smallest at the root makes it
        cheap to decide whether a new element should replace it.
*/
void kSmallest(const int *arr, int n, int k, MaxHeap *heap, int *outCount) {
    // heap is assumed initialized (and ideally empty) when passed in
    for (int i = 0; i < n; i++) {
        if (heap->size < k) {
            // Fill the heap until it contains k elements.
            maxheap_insert(heap, arr[i]);
        } else if (arr[i] < maxheap_peek(heap)) {
            // If current element is smaller than the largest of our kept k,
            // replace that largest with this smaller element.
            maxheap_pop(heap);
            maxheap_insert(heap, arr[i]);
        }
        // Else: arr[i] is not among the k smallest seen so far; ignore it.
    }

    // Now heap contains the k smallest elements (unordered in the array representation).
    // They are not sorted; popping will yield them in descending order (max-heap behavior).
    *outCount = heap->size;
}

/* ---------- Test function ---------- */

/*
    print_array
    -----------
    Utility function to print a label followed by n integers from array a.
*/
static void print_array(const char *label, const int *a, int n) {
    printf("%s", label);
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

/*
    test_kSmallest
    --------------
    Exercises kSmallest with two scenarios:
      1) k <= n (normal case)
      2) k > n (heap should contain all elements)

    Note:
      - For test 1, the k smallest elements of the input should be 1,2,3,4.
        Because the heap is a MAX-heap, popping prints them in descending order: 4 3 2 1.
*/
void test_kSmallest(void) {
    int arr[] = { 7, 2, 9, 4, 1, 5, 8, 3, 6 };
    int n = (int)(sizeof(arr) / sizeof(arr[0]));
    int k = 4;

    printf("=== Test 1: k <= n ===\n");
    print_array("Input array: ", arr, n);
    printf("k = %d\n", k);

    MaxHeap heap;

    // Initial capacity of k is enough because the algorithm never needs > k elements
    maxheap_init(&heap, k);

    int outCount = 0;
    kSmallest(arr, n, k, &heap, &outCount);

    printf("Heap size after kSmallest: %d (expected %d)\n", outCount, k);

    /*
        Pop all elements:
          - These should be the k smallest elements {1,2,3,4}
          - They will come out in descending order because this is a max-heap
    */
    printf("Elements in heap (popped, should be k smallest): ");

    // buf is not strictly needed for printing, but can be useful for further checks.
    int *buf = (int *)malloc(sizeof(int) * outCount);
    if (!buf) {
        fprintf(stderr, "Out of memory allocating buf in test_kSmallest\n");
        maxheap_free(&heap);
        exit(EXIT_FAILURE);
    }

    int m = outCount;
    for (int i = 0; i < m; i++) {
        buf[i] = maxheap_pop(&heap);
        printf("%d ", buf[i]);
    }
    printf("\n");

    free(buf);
    maxheap_free(&heap);

    // Second test: k > n
    printf("\n=== Test 2: k > n ===\n");
    k = 20;

    print_array("Input array: ", arr, n);
    printf("k = %d\n", k);

    // Note: Initial capacity is set to k; heap will grow if needed anyway.
    maxheap_init(&heap, k);

    outCount = 0;
    kSmallest(arr, n, k, &heap, &outCount);

    // When k > n, we should end up with all n elements.
    printf("Heap size after kSmallest: %d (expected %d)\n", outCount, n);

    printf("Elements in heap (popped, should be all elements): ");
    m = outCount;
    for (int i = 0; i < m; i++) {
        int v = maxheap_pop(&heap);
        printf("%d ", v);
    }
    printf("\n");

    maxheap_free(&heap);
}

/* ---------- main ---------- */

int main(void) {
    // Run the test harness
    test_kSmallest();
    return 0;
}
