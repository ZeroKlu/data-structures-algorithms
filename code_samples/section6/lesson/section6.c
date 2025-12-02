#include <stdbool.h>
#include <stdio.h>

#define HEAP_MAX 1024

typedef struct {
    int data[HEAP_MAX];
    int size; // number of elements in heap
} MinHeap;

void heap_init(MinHeap* h) {
    h->size = 0;
}

static void swap_int(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

bool heap_insert(MinHeap* h, int value) {
    if (h->size >= HEAP_MAX) {
        return false; // heap full
    }
    int i = h->size;
    h->data[i] = value;
    h->size++;

    // heapify up
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (h->data[parent] <= h->data[i]) {
            break;
        }
        swap_int(&h->data[parent], &h->data[i]);
        i = parent;
    }
    return true;
}

bool heap_extract_min(MinHeap* h, int* out) {
    if (h->size == 0) {
        return false;
    }
    *out = h->data[0];
    h->size--;
    if (h->size == 0) {
        return true;
    }
    h->data[0] = h->data[h->size];

    int i = 0;
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < h->size && h->data[left] < h->data[smallest]) {
            smallest = left;
        }
        if (right < h->size && h->data[right] < h->data[smallest]) {
            smallest = right;
        }
        if (smallest == i) break;
        swap_int(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
    return true;
}

static void heapify_down_array(int data[], int n, int i) {
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < n && data[left] < data[smallest]) {
            smallest = left;
        }
        if (right < n && data[right] < data[smallest]) {
            smallest = right;
        }
        if (smallest == i) break;
        swap_int(&data[i], &data[smallest]);
        i = smallest;
    }
}

void build_min_heap(int data[], int n) {
    for (int i = (n / 2) - 1; i >= 0; i--) {
        heapify_down_array(data, n, i);
    }
}

// Using build_min_heap and heapify_down_array from above.
// Heapsort with a min-heap produces descending order.
void heapsort_desc_minheap(int a[], int n) {
    build_min_heap(a, n);
    for (int end = n - 1; end > 0; end--) {
        swap_int(&a[0], &a[end]);      // move min to end
        heapify_down_array(a, end, 0);  // restore heap on prefix [0, end)
    }
}

int main(void) {
    // ---------- Test MinHeap insert & extract ----------
    printf("=== Testing MinHeap insert / extract ===\n");

    MinHeap h;
    heap_init(&h);

    int values_to_insert[] = { 5, 3, 8, 1, 7, 2 };
    int count = sizeof(values_to_insert) / sizeof(values_to_insert[0]);

    printf("Inserting: ");
    for (int i = 0; i < count; i++) {
        printf("%d ", values_to_insert[i]);
        heap_insert(&h, values_to_insert[i]);
    }
    printf("\n");

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

    heapsort_desc_minheap(arr2, n2);

    printf("After heapsort_desc_minheap (descending): ");
    for (int i = 0; i < n2; i++) printf("%d ", arr2[i]);
    printf("\n");

    return 0;
}
