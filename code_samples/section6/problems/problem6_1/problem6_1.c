#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

typedef struct {
    int *data;
    int size;
    int capacity;
} MinHeap;

static void heap_init(MinHeap *h, int cap) {
    h->size = 0;
    h->capacity = cap;
    h->data = (int *)malloc(sizeof(int) * cap);
}

static void heap_free(MinHeap *h) {
    free(h->data);
    h->data = NULL;
    h->size = h->capacity = 0;
}

static void heap_ensure_capacity(MinHeap *h) {
    if (h->size < h->capacity) return;
    h->capacity *= 2;
    h->data = (int *)realloc(h->data, sizeof(int) * h->capacity);
}

static void heap_swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

static void bubble_up(MinHeap *h, int i) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (h->data[i] >= h->data[p]) break;
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

void heap_insert(MinHeap *h, int x) {
    heap_ensure_capacity(h);
    h->data[h->size] = x;
    bubble_up(h, h->size);
    h->size++;
}

int heap_peek(MinHeap *h) {
    return (h->size == 0) ? INT_MIN : h->data[0];
}

int heap_pop(MinHeap *h) {
    if (h->size == 0) return INT_MIN;
    int root = h->data[0];
    h->size--;
    if (h->size > 0) {
        h->data[0] = h->data[h->size];
        heapify_down(h, 0);
    }
    return root;
}

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

    // Pop all elements to verify sorted order
    printf("Popping all elements in sorted order: ");
    while (1) {
        int x = heap_pop(&h);
        if (x == INT_MIN) {
            break;  // heap empty
        }
        printf("%d ", x);
    }
    printf("\n\n");

    printf("After popping everything:\n");
    printf("  size: %d, capacity: %d\n", h.size, h.capacity);
    printf("  peek on empty heap: %d\n", heap_peek(&h));
    printf("  pop on empty heap: %d\n\n", heap_pop(&h));

    // Test inserting again after being emptied
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

    heap_free(&h);
    printf("\nHeap freed. size: %d, capacity: %d\n", h.size, h.capacity);

    return 0;
}
