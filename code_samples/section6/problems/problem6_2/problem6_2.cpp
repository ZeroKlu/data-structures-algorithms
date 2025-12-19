#include <iostream>
#include <vector>
using namespace std;

/*
    This program demonstrates building a MIN-HEAP "in place" from an existing
    array-like container (std::vector<int>).

    Core heap facts (0-based indexing):
        parent(i) = (i - 1) / 2
        left(i)   = 2*i + 1
        right(i)  = 2*i + 2

    Min-heap invariant:
        arr[parent(i)] <= arr[i]  for all i > 0

    Why buildHeap is efficient:
        - Bottom-up heap construction runs in O(n) time.
        - It starts from the last internal node (n/2 - 1) and heapifies down
          each node moving toward the root.
        - Leaves (indices >= n/2) are already valid heaps of size 1.

    Notes:
        - heapify() here is recursive. For very large heaps, an iterative
          version can avoid deep recursion.
        - popMinFromHeapArray() is a testing helper, not part of the core
          "buildHeap" requirement.
*/

static void heapify(vector<int> &arr, int n, int i) {
    /*
        heapify (a.k.a. sift-down)
        --------------------------
        Ensures the subtree rooted at index i satisfies the min-heap property,
        assuming its children subtrees are already heaps.

        Parameters:
            arr : vector storing heap in array form
            n   : number of valid heap elements (heap size)
            i   : index of the node to sift down

        Algorithm:
            1) Identify the smallest among node i and its children.
            2) If one child is smaller than arr[i], swap with the smallest child.
            3) Recurse into the child index we swapped with, since that subtree
               may now violate the heap property.

        Complexity:
            O(log n) worst case (height of heap).
    */
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // If left child exists and is smaller than current smallest, update smallest.
    if (left < n && arr[left] < arr[smallest]) smallest = left;

    // If right child exists and is smaller than current smallest, update smallest.
    if (right < n && arr[right] < arr[smallest]) smallest = right;

    // If the smallest is not the current node, swap and continue heapifying downward.
    if (smallest != i) {
        swap(arr[i], arr[smallest]);
        heapify(arr, n, smallest);
    }
}

void buildHeap(vector<int> &arr) {
    /*
        buildHeap
        ---------
        Converts an arbitrary vector into a min-heap in place.

        Strategy (bottom-up):
            - Compute n = arr.size()
            - Start from the last internal node: n/2 - 1
            - Heapify down each node going backwards to the root (index 0)

        Why start at n/2 - 1?
            - All indices >= n/2 are leaves (no children), so they are already heaps.
            - Internal nodes may violate the heap property, so we fix them bottom-up.

        Complexity:
            O(n) overall.
    */
    int n = (int)arr.size();

    // Iterate over all internal nodes from last parent down to root.
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

static int popMinFromHeapArray(vector<int> &arr, int &n) {
    /*
        popMinFromHeapArray
        -------------------
        Testing helper: removes and returns the minimum element from an array-based heap.

        Parameters:
            arr : vector storing the heap array
            n   : current heap size (passed by reference and decremented)

        Returns:
            - root (minimum) if n > 0
            - 0 if the heap is empty

        Important caveat:
            Returning 0 for empty is ambiguous if 0 could be a valid element.
            For robust code, prefer:
                - bool pop(int& out),
                - std::optional<int>,
                - or throwing on empty.
    */

    // Simple pop from array-based min-heap (not part of spec, just for testing)
    if (n == 0) return 0;

    // The minimum element is always at index 0 in a min-heap.
    int root = arr[0];

    // Shrink heap size by one (logically removing last element).
    n--;

    if (n > 0) {
        // Move the last element into the root position.
        // This may violate the heap property at the root, so heapify down.
        arr[0] = arr[n];
        heapify(arr, n, 0);
    }

    return root;
}

static void printArray(const string &label, const vector<int> &arr) {
    /*
        printArray
        ----------
        Utility function to print all elements of a vector with a label.
        (When printing a heap array, remember it's "heap order", not sorted order.)
    */
    cout << label;
    for (int x : arr) cout << x << " ";
    cout << "\n";
}

int main() {
    /*
        Demo / test harness

        Steps:
            1) Create an unsorted vector.
            2) Print it.
            3) Call buildHeap() to convert it into a min-heap in place.
            4) Print the resulting heap array representation.
            5) Pop repeatedly from a copy to confirm values come out in ascending order.
    */
    vector<int> arr1 = {9, 4, 7, 1, 0, 3, 2};
    int n1 = (int)arr1.size();

    printArray("Original array: ", arr1);

    // Transform arr1 into a min-heap
    buildHeap(arr1);

    // This print shows heap layout in array form, not sorted order.
    printArray("After buildHeap (min-heap array): ", arr1);

    // Optional correctness test:
    // Repeatedly extracting min should yield a sorted ascending sequence.
    cout << "Extracting elements in sorted order: ";
    int size = n1;

    // Use a temporary copy since extraction mutates the heap structure
    vector<int> heapCopy = arr1;

    while (size > 0) {
        int x = popMinFromHeapArray(heapCopy, size);
        cout << x << " ";
    }
    cout << "\n";

    return 0;
}
