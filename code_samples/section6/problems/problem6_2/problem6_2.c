#include <stdio.h>
#include <stdlib.h>

/*
    This program demonstrates how to build a MIN-HEAP "in place" from an
    existing array (i.e., without inserting elements one-by-one).

    Core idea:
      - A binary heap can be stored in an array using 0-based indexing.
      - For a node at index i:
            left child  = 2*i + 1
            right child = 2*i + 2
            parent      = (i - 1) / 2

    Min-heap invariant:
        arr[parent(i)] <= arr[i]   for all i > 0

    Building a heap efficiently:
      - The bottom-up heap construction runs in O(n) time.
      - We start from the last internal node (n/2 - 1) and heapify down each node.
        Leaves (indices >= n/2) are already heaps of size 1.

    Notes about this file:
      - heapify() is a "sift down" operation used both by buildHeap() and pop.
      - pop_min_from_heap_array() is just a helper for testing; it's not required
        for "buildHeap" itself.
      - This implementation uses recursion in heapify(); an iterative version
        avoids deep recursion for very large heaps.
*/

/*
    heapify
    -------
    Ensures the subtree rooted at index i satisfies the min-heap property,
    assuming the subtrees of its children already satisfy the heap property.

    Parameters:
      arr[] : array representing the heap
      n     : number of valid elements in arr[] (heap size)
      i     : index of the subtree root to heapify

    Behavior:
      - Compare arr[i] to its children.
      - If either child is smaller than arr[i], swap with the smallest child.
      - Recurse into the child position where we swapped, because that subtree
        may now violate the heap property.

    Complexity:
      - O(log n) in the worst case (height of the heap).
*/
static void heapify(int arr[], int n, int i) {
    int smallest = i;         // assume current node is smallest
    int left = 2 * i + 1;     // left child index
    int right = 2 * i + 2;    // right child index

    // If left child exists and is smaller than current smallest, update.
    if (left < n && arr[left] < arr[smallest]) smallest = left;

    // If right child exists and is smaller than current smallest, update.
    if (right < n && arr[right] < arr[smallest]) smallest = right;

    // If the smallest element is not the current node, swap and continue.
    if (smallest != i) {
        int tmp = arr[i];
        arr[i] = arr[smallest];
        arr[smallest] = tmp;

        // After swapping, the heap property may be violated in the child subtree,
        // so we heapify that subtree as well.
        heapify(arr, n, smallest);
    }
}

/*
    buildHeap
    ---------
    Converts an arbitrary array into a min-heap IN PLACE.

    Parameters:
      arr[] : array to transform into a heap
      n     : number of elements in arr[]

    Strategy (bottom-up heap construction):
      - Start at the last parent node: n/2 - 1
      - Move backwards to index 0, heapifying each index.
      - Leaves do not need heapify because they have no children.

    Complexity:
      - O(n) overall (more efficient than n inserts at O(log n) each).
*/
void buildHeap(int arr[], int n) {
    // The last internal node (last element that has at least one child).
    // For 0-based array heaps, children start at index 2*i+1, so any i >= n/2 has no children.
    int start = n / 2 - 1;

    // Heapify each internal node moving upward to the root.
    for (int i = start; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

/*
    pop_min_from_heap_array
    -----------------------
    Test helper: removes and returns the minimum element from an array-based
    min-heap.

    Parameters:
      arr[] : the heap array
      n     : pointer to heap size (will be decremented)

    Returns:
      - root value (minimum) if heap not empty
      - 0 if heap is empty

    IMPORTANT:
      - Returning 0 as the empty-heap value is ambiguous if 0 is a legitimate element.
        This is okay for a small demo but not robust for general use.
      - This function is not required for the "buildHeap" assignment; it exists
        to verify correctness by extracting in sorted order.

    Complexity:
      - O(log n) due to heapify() after removal.
*/
static int pop_min_from_heap_array(int arr[], int *n) {
    // Simple pop from array-based min-heap (not part of assignment, just for testing)
    if (*n == 0) return 0;

    // The minimum element in a min-heap is always at the root.
    int root = arr[0];

    // Reduce heap size by one (logically removing last element).
    (*n)--;

    if (*n > 0) {
        // Move last element to root position and restore heap property.
        arr[0] = arr[*n];
        heapify(arr, *n, 0);
    }

    return root;
}

/*
    print_array
    -----------
    Utility to print the first n elements of an integer array with a label.
*/
static void print_array(const char *label, int arr[], int n) {
    printf("%s", label);
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");
}

/*
    main
    ----
    Demonstration:
      1) Print an unsorted input array.
      2) Convert it to a min-heap using buildHeap().
      3) Print the resulting heap array (in heap order, not sorted order).
      4) Pop all elements to show they come out in ascending order (validation).
*/
int main(void) {
    int arr1[] = { 9, 4, 7, 1, 0, 3, 2 };
    int n1 = sizeof(arr1) / sizeof(arr1[0]);

    // Show the initial unsorted array
    print_array("Original array: ", arr1, n1);

    // Transform the array into a min-heap in place
    buildHeap(arr1, n1);

    // Show the array representation of the heap (heap order)
    print_array("After buildHeap (min-heap array): ", arr1, n1);

    // Optional correctness check:
    // Repeatedly popping should yield elements in sorted (ascending) order.
    printf("Extracting elements in sorted order: ");
    int size = n1;
    while (size > 0) {
        int x = pop_min_from_heap_array(arr1, &size);
        printf("%d ", x);
    }
    printf("\n");

    return 0;
}
