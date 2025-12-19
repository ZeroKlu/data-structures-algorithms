#include <stdbool.h>
#include <stdio.h>

#define HEAP_MAX 1024  // Maximum number of elements this fixed-size heap can store

// ==========================
// MIN-HEAP STRUCTURE
// ==========================
//
// This heap is stored in an array using the standard binary-heap indexing:
// - Parent of i:      (i - 1) / 2
// - Left child of i:  2*i + 1
// - Right child of i: 2*i + 2
//
// Min-heap property:
// - Every parent <= its children
typedef struct {
    int data[HEAP_MAX]; // backing array for heap elements
    int size;           // current number of elements in heap
} MinHeap;

// Initialize heap to be empty
void heap_init(MinHeap* h) {
    h->size = 0;
}

// Swap two ints (utility)
static void swap_int(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// ==========================
// HEAP INSERT
// ==========================
//
// Insert value into the min-heap.
// Steps:
// 1) Place value at the end of the array (index = size)
// 2) "Heapify up" (bubble up) while the heap property is violated
//
// Returns false if heap is full; true on success.
bool heap_insert(MinHeap* h, int value) {
    // Check capacity
    if (h->size >= HEAP_MAX) {
        return false; // heap full
    }

    // Put new element at the next free position
    int i = h->size;
    h->data[i] = value;
    h->size++;

    // Heapify up:
    // While new node is smaller than its parent, swap it upward.
    while (i > 0) {
        int parent = (i - 1) / 2;

        // If parent already <= child, heap property holds
        if (h->data[parent] <= h->data[i]) {
            break;
        }

        // Otherwise swap and continue moving up
        swap_int(&h->data[parent], &h->data[i]);
        i = parent;
    }
    return true;
}

// ==========================
// EXTRACT MIN
// ==========================
//
// Remove the smallest element (root at index 0).
// Steps:
// 1) Save data[0] as the answer
// 2) Move last element to data[0]
// 3) Decrease size
// 4) "Heapify down" (bubble down) to restore heap property
//
// Returns false if heap is empty; true on success (min placed in *out).
bool heap_extract_min(MinHeap* h, int* out) {
    // No elements to extract
    if (h->size == 0) {
        return false;
    }

    // The minimum is always at the root
    *out = h->data[0];

    // Reduce heap size (logically remove last element)
    h->size--;

    // If heap is now empty, we're done
    if (h->size == 0) {
        return true;
    }

    // Move last element to root (index 0) to fill the gap
    h->data[0] = h->data[h->size];

    // Heapify down:
    // Compare the current node with its children and swap with the smaller child
    // until heap property is restored.
    int i = 0;
    while (true) {
        int left = 2 * i + 1;   // left child index
        int right = 2 * i + 2;  // right child index
        int smallest = i;       // assume current node is smallest

        // If left child exists and is smaller, update smallest
        if (left < h->size && h->data[left] < h->data[smallest]) {
            smallest = left;
        }

        // If right child exists and is smaller than the current smallest, update
        if (right < h->size && h->data[right] < h->data[smallest]) {
            smallest = right;
        }

        // If no child is smaller, heap property holds
        if (smallest == i) break;

        // Otherwise swap down to the smallest child and continue
        swap_int(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
    return true;
}

// ==========================
// HEAPIFY DOWN (ARRAY VERSION)
// ==========================
//
// This helper is used for building a heap from an arbitrary array,
// and for heapsort. It performs the same "bubble down" logic as
// heap_extract_min, but works directly on a plain array.
//
// Parameters:
// - data: array representing a heap
// - n:    number of elements in the heap portion
// - i:    index to heapify down from
static void heapify_down_array(int data[], int n, int i) {
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        // Compare with left child
        if (left < n && data[left] < data[smallest]) {
            smallest = left;
        }

        // Compare with right child
        if (right < n && data[right] < data[smallest]) {
            smallest = right;
        }

        // If current node is already the smallest, stop
        if (smallest == i) break;

        // Swap down and continue
        swap_int(&data[i], &data[smallest]);
        i = smallest;
    }
}

// ==========================
// BUILD MIN-HEAP FROM ARRAY
// ==========================
//
// Turns an arbitrary array into a valid min-heap in-place.
// This is the classic O(n) bottom-up heap construction:
//
// - All indices >= n/2 are leaves (already heaps).
// - So we start from the last parent (n/2 - 1) and heapify down to 0.
void build_min_heap(int data[], int n) {
    // Start from last internal node and heapify down each
    for (int i = (n / 2) - 1; i >= 0; i--) {
        heapify_down_array(data, n, i);
    }
}

// ==========================
// HEAPSORT (DESCENDING USING MIN-HEAP)
// ==========================
//
// Using a MIN-heap for heapsort naturally places the smallest element
// at the end each iteration, producing DESCENDING order.
//
// Process:
// 1) Build a min-heap on the whole array
// 2) Repeatedly swap heap root (min) with the last element in the heap
// 3) Reduce heap size by 1
// 4) Heapify down from root to restore min-heap property
//
// Result: array sorted in descending order.
void heapsort_desc_minheap(int a[], int n) {
    build_min_heap(a, n);

    // end marks the last index of the active heap
    for (int end = n - 1; end > 0; end--) {
        // Move current min (a[0]) to its final position at the end
        swap_int(&a[0], &a[end]);

        // Restore heap property in the reduced heap [0, end)
        heapify_down_array(a, end, 0);
    }
}

// ==========================
// TEST HARNESS
// ==========================

int main(void) {
    // ---------- Test MinHeap insert & extract ----------
    printf("=== Testing MinHeap insert / extract ===\n");

    MinHeap h;
    heap_init(&h);

    int values_to_insert[] = { 5, 3, 8, 1, 7, 2 };
    int count = sizeof(values_to_insert) / sizeof(values_to_insert[0]);

    // Insert values into heap
    printf("Inserting: ");
    for (int i = 0; i < count; i++) {
        printf("%d ", values_to_insert[i]);
        heap_insert(&h, values_to_insert[i]);
    }
    printf("\n");

    // Extract values in increasing order (min-heap property)
    printf("Extracting in sorted order: ");
    int x;
    while (heap_extract_min(&h, &x)) {
        printf("%d ", x);
    }
    printf("\n\n");

    // ---------- Test build_min_heap ----------
    printf("=== Testing build_min_heap ===\n");

    int arr1[] = { 9, 4, 7, 1, 0, 3, 2 };
    int n1 = sizeof(arr1) / sizeof(arr1[0]);

    printf("Original array: ");
    for (int i = 0; i < n1; i++) printf("%d ", arr1[i]);
    printf("\n");

    // Convert array into a min-heap (in-place)
    build_min_heap(arr1, n1);

    printf("After build_min_heap (min-heap array): ");
    for (int i = 0; i < n1; i++) printf("%d ", arr1[i]);
    printf("\n\n");

    // ---------- Test heapsort_desc_minheap ----------
    printf("=== Testing heapsort_desc_minheap (descending sort) ===\n");

    int arr2[] = { 12, 3, 17, 8, 34, 25, 1 };
    int n2 = sizeof(arr2) / sizeof(arr2[0]);

    printf("Original array: ");
    for (int i = 0; i < n2; i++) printf("%d ", arr2[i]);
    printf("\n");

    // Heapsort using min-heap produces descending order
    heapsort_desc_minheap(arr2, n2);

    printf("After heapsort_desc_minheap (descending): ");
    for (int i = 0; i < n2; i++) printf("%d ", arr2[i]);
    printf("\n");

    return 0;
}
