#include <stdio.h>
#include <stdlib.h>

static void heapify(int arr[], int n, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] < arr[smallest]) smallest = left;
    if (right < n && arr[right] < arr[smallest]) smallest = right;

    if (smallest != i) {
        int tmp = arr[i];
        arr[i] = arr[smallest];
        arr[smallest] = tmp;
        heapify(arr, n, smallest);
    }
}

void buildHeap(int arr[], int n) {
    int start = n / 2 - 1;
    for (int i = start; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

static int pop_min_from_heap_array(int arr[], int *n) {
    // Simple pop from array-based min-heap (not part of assignment, just for testing)
    if (*n == 0) return 0;

    int root = arr[0];
    (*n)--;
    if (*n > 0) {
        arr[0] = arr[*n];
        heapify(arr, *n, 0);
    }
    return root;
}

static void print_array(const char *label, int arr[], int n) {
    printf("%s", label);
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

int main(void) {
    int arr1[] = { 9, 4, 7, 1, 0, 3, 2 };
    int n1 = sizeof(arr1) / sizeof(arr1[0]);

    print_array("Original array: ", arr1, n1);

    buildHeap(arr1, n1);

    print_array("After buildHeap (min-heap array): ", arr1, n1);

    // Optional: Test extracting in sorted order to confirm heap validity
    printf("Extracting elements in sorted order: ");
    int size = n1;
    while (size > 0) {
        int x = pop_min_from_heap_array(arr1, &size);
        printf("%d ", x);
    }
    printf("\n");

    return 0;
}
