#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/* ===== Min-heap for ints ===== */

typedef struct {
    int *data;
    int size;
    int capacity;
} MinHeap;

static void heap_init(MinHeap *h, int cap) {
    h->size = 0;
    h->capacity = (cap > 0) ? cap : 1;
    h->data = (int *)malloc(sizeof(int) * h->capacity);
    if (!h->data) {
        fprintf(stderr, "Out of memory in heap_init\n");
        exit(EXIT_FAILURE);
    }
}

static void heap_free(MinHeap *h) {
    free(h->data);
    h->data = NULL;
    h->size = 0;
    h->capacity = 0;
}

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

static void heap_swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

static void bubble_up(MinHeap *h, int i) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->data[i] >= h->data[p]) break;  // min-heap property
        heap_swap(&h->data[i], &h->data[p]);
        i = p;
    }
}

static void heapify_down(MinHeap *h, int i) {
    int n = h->size;
    for (;;) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && h->data[left] < h->data[smallest]) {
            smallest = left;
        }
        if (right < n && h->data[right] < h->data[smallest]) {
            smallest = right;
        }
        if (smallest == i) break;
        heap_swap(&h->data[i], &h->data[smallest]);
        i = smallest;
    }
}

static void heap_insert(MinHeap *h, int x) {
    heap_ensure_capacity(h);
    h->data[h->size] = x;
    bubble_up(h, h->size);
    h->size++;
}

static int heap_peek(MinHeap *h) {
    return (h->size == 0) ? INT_MIN : h->data[0];
}

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

typedef struct {
    int k;
    MinHeap heap; // min-heap of size at most k
} KthLargest;

void kth_init(KthLargest *obj, int k, const int *nums, int n) {
    obj->k = k;
    heap_init(&obj->heap, k > 0 ? k : 1);

    for (int i = 0; i < n; i++) {
        if (obj->heap.size < k) {
            heap_insert(&obj->heap, nums[i]);
        } else if (k > 0 && nums[i] > heap_peek(&obj->heap)) {
            heap_pop(&obj->heap);
            heap_insert(&obj->heap, nums[i]);
        }
    }
}

int kth_add(KthLargest *obj, int val) {
    if (obj->k == 0) return 0; // undefined, but avoid division by zero scenario
    if (obj->heap.size < obj->k) {
        heap_insert(&obj->heap, val);
    } else if (val > heap_peek(&obj->heap)) {
        heap_pop(&obj->heap);
        heap_insert(&obj->heap, val);
    }
    return heap_peek(&obj->heap);
}

void kth_free(KthLargest *obj) {
    heap_free(&obj->heap);
}

/* ===== Test code ===== */

static void print_array(const char *label, const int *a, int n) {
    printf("%s", label);
    for (int i = 0; i < n; i++) printf("%d ", a[i]);
    printf("\n");
}

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
    printf("kth largest after init: %d\n\n", kth.heap.size > 0 ? heap_peek(&kth.heap) : INT_MIN);

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

int main(void) {
    test_kthLargest();
    return 0;
}
