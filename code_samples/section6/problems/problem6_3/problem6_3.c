#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* ---------- MaxHeap definition ---------- */

typedef struct {
    int *data;
    int size;
    int capacity;
} MaxHeap;

// Helpers for MaxHeap
static void maxheap_swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

static void maxheap_ensure_capacity(MaxHeap *h) {
    if (h->size < h->capacity) return;
    int newCap = (h->capacity > 0) ? h->capacity * 2 : 1;
    int *nd = (int *)realloc(h->data, newCap * sizeof(int));
    if (!nd) {
        fprintf(stderr, "Out of memory in maxheap_ensure_capacity\n");
        exit(EXIT_FAILURE);
    }
    h->data = nd;
    h->capacity = newCap;
}

static void maxheap_bubble_up(MaxHeap *h, int i) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->data[i] <= h->data[p]) break; // max-heap: child <= parent is ok
        maxheap_swap(&h->data[i], &h->data[p]);
        i = p;
    }
}

static void maxheap_heapify_down(MaxHeap *h, int i) {
    int n = h->size;
    for (;;) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && h->data[left] > h->data[largest]) largest = left;
        if (right < n && h->data[right] > h->data[largest]) largest = right;
        if (largest == i) break;
        maxheap_swap(&h->data[i], &h->data[largest]);
        i = largest;
    }
}

/* Public MaxHeap API (as assumed in the comment) */

void maxheap_init(MaxHeap *h, int cap) {
    h->size = 0;
    h->capacity = cap > 0 ? cap : 1;
    h->data = (int *)malloc(sizeof(int) * h->capacity);
    if (!h->data) {
        fprintf(stderr, "Out of memory in maxheap_init\n");
        exit(EXIT_FAILURE);
    }
}

void maxheap_free(MaxHeap *h) {
    free(h->data);
    h->data = NULL;
    h->size = 0;
    h->capacity = 0;
}

void maxheap_insert(MaxHeap *h, int x) {
    maxheap_ensure_capacity(h);
    h->data[h->size] = x;
    maxheap_bubble_up(h, h->size);
    h->size++;
}

int maxheap_peek(const MaxHeap *h) {
    return (h->size == 0) ? INT_MIN : h->data[0];
}

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

void kSmallest(const int *arr, int n, int k, MaxHeap *heap, int *outCount) {
    // heap is assumed initialized
    for (int i = 0; i < n; i++) {
        if (heap->size < k) {
            maxheap_insert(heap, arr[i]);
        } else if (arr[i] < maxheap_peek(heap)) {
            maxheap_pop(heap);
            maxheap_insert(heap, arr[i]);
        }
    }
    // Now heap contains the k smallest elements (unordered).
    *outCount = heap->size;
}

/* ---------- Test function ---------- */

static void print_array(const char *label, const int *a, int n) {
    printf("%s", label);
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

// This is the function that tests kSmallest:
void test_kSmallest(void) {
    int arr[] = { 7, 2, 9, 4, 1, 5, 8, 3, 6 };
    int n = (int)(sizeof(arr) / sizeof(arr[0]));
    int k = 4;

    printf("=== Test 1: k <= n ===\n");
    print_array("Input array: ", arr, n);
    printf("k = %d\n", k);

    MaxHeap heap;
    maxheap_init(&heap, k);  // initial capacity k is enough for this algorithm

    int outCount = 0;
    kSmallest(arr, n, k, &heap, &outCount);

    printf("Heap size after kSmallest: %d (expected %d)\n", outCount, k);

    // Pop all elements: they should be the k smallest elements (1,2,3,4),
    // but will come out in descending order because this is a max-heap.
    printf("Elements in heap (popped, should be k smallest): ");
    int *buf = (int *)malloc(sizeof(int) * outCount);
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

    maxheap_init(&heap, k);
    outCount = 0;
    kSmallest(arr, n, k, &heap, &outCount);

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
    test_kSmallest();
    return 0;
}
