#nullable enable

/*
    This file demonstrates building a MIN-HEAP in-place from an integer array,
    then validating correctness by repeatedly popping the minimum from a COPY
    of the heap.

    Key heap facts (0-based indexing):
        parent(i) = (i - 1) / 2
        left(i)   = 2*i + 1
        right(i)  = 2*i + 2

    Min-heap invariant:
        arr[parent(i)] <= arr[i] for all i > 0

    Structure:
        1) Helper functions for printing, popping min, and heapifying down
           (implemented as top-level local functions).
        2) A simple test harness that:
            - prints original array
            - calls HeapUtils.BuildHeap(arr) to heapify in place
            - prints heap array (heap order, not sorted)
            - extracts mins from a clone to produce sorted output
        3) HeapUtils class that contains:
            - Heapify (recursive sift-down for a single subtree)
            - BuildHeap (bottom-up heap construction, O(n))

    Notes about #nullable enable:
        - This code does not use nullable reference types explicitly, but enabling
          it is fine and can help catch issues in larger programs.
*/

// ===== Helpers =====

// Print helper
void PrintArray(string label, int[] arr) =>
    // Prints: <label><space-separated elements>
    // Example: "Original array: 9 4 7 1 0 3 2"
    Console.WriteLine(label + string.Join(" ", arr));

// Correct popMin for array-based min-heap using prefix size
int PopMin(int[] arr, ref int size) {
    /*
        PopMin
        ------
        Removes and returns the minimum element from an array that is assumed
        to be a min-heap over the prefix [0, size).

        Why "prefix size" matters:
            - The heap may occupy only the first "size" elements of the array.
            - We must NOT use arr.Length as the heap size during extraction,
              because extraction shrinks the heap.

        Parameters:
            arr  : array containing heap elements in indices [0, size)
            size : (by ref) current heap size; decremented after removal

        Returns:
            - int.MinValue if heap is empty (size == 0)
            - otherwise the removed minimum

        Caveat:
            - int.MinValue is used as an "empty" sentinel. This is fine for
              testing, but ambiguous if int.MinValue could be a legitimate value
              stored in the heap. A production API might instead use:
                  * bool TryPop(out int value)
                  * or int? (nullable)
                  * or throw an exception on empty
    */
    if (size == 0) return int.MinValue;

    // In a min-heap, the minimum element is always at index 0.
    int root = arr[0];

    // Reduce heap size by one (logically removing last element).
    size--;

    if (size > 0) {
        // Move the last element in the heap prefix to the root position.
        // This may violate heap ordering at the root, so we sift down.
        arr[0] = arr[size];
        HeapifyDown(arr, size, 0);
    }

    return root;
}

// Local heapify-down using given size (not arr.Length)
void HeapifyDown(int[] arr, int size, int i) {
    /*
        HeapifyDown (iterative sift-down)
        --------------------------------
        Restores the min-heap invariant starting from index i, assuming that
        the subtrees below i are already heaps.

        Parameters:
            arr  : array containing heap data
            size : heap size over prefix [0, size)
            i    : index to sift down from (commonly 0)

        Algorithm:
            - Repeatedly compare node i with its left/right children.
            - Swap with the smallest child if that child is smaller.
            - Continue from the swapped child index.
            - Stop when node i is <= both children, or i has no children.

        Complexity:
            O(log n) worst case (heap height).
    */
    while (true) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        // If left child exists within heap prefix and is smaller, mark it.
        if (left < size && arr[left] < arr[smallest]) smallest = left;

        // If right child exists within heap prefix and is smaller, mark it.
        if (right < size && arr[right] < arr[smallest]) smallest = right;

        // If no child is smaller, heap property holds at i.
        if (smallest == i) break;

        // Swap current node with the smaller child.
        (arr[i], arr[smallest]) = (arr[smallest], arr[i]);

        // Continue sifting down from the child position.
        i = smallest;
    }
}

// ===== Your Test =====

/*
    Test input:
        An arbitrary unsorted array.

    We will:
        1) Print it
        2) Heapify it in-place using HeapUtils.BuildHeap
        3) Print the heap array (heap order, not sorted)
        4) Clone the heap and repeatedly PopMin to verify ascending output
*/
int[] arr = [9, 4, 7, 1, 0, 3, 2];

PrintArray("Original array: ", arr);

// BuildHeap transforms the array into a valid min-heap in-place.
HeapUtils.BuildHeap(arr);

// Note: This printed array is in heap layout, not sorted order.
PrintArray("After BuildHeap (min-heap array): ", arr);

// Extract values to test correctness
Console.WriteLine("Extracting elements in sorted order:");

// Copy the heap because PopMin mutates the array contents and shrinks "size".
int[] heapCopy = (int[])arr.Clone();
int size = heapCopy.Length;

// Collect extracted values so we can print them neatly afterwards.
List<int> output = [];

while (size > 0) {
    // PopMin removes the current minimum and reduces "size" by 1.
    int v = PopMin(heapCopy, ref size);
    output.Add(v);
}

// Expected result for the sample input: 0 1 2 3 4 7 9
PrintArray("", output.ToArray());

class HeapUtils {
    /*
        HeapUtils provides:
          - Heapify: a recursive heapify-down for a subtree
          - BuildHeap: bottom-up heap construction in O(n)

        This matches the classic heap construction approach:
          - Start from last internal node (n/2 - 1)
          - Heapify each node going backwards to the root
    */

    private static void Heapify(int[] arr, int n, int i) {
        /*
            Heapify (recursive)
            -------------------
            Ensures the subtree rooted at i satisfies the min-heap property,
            given that its children subtrees already satisfy the property.

            Parameters:
                arr : heap array
                n   : heap size (for build phase this is arr.Length)
                i   : subtree root index

            Steps:
                - pick smallest among i, left child, right child
                - if smallest isn't i, swap and recurse into smallest

            Complexity:
                O(log n) worst case.
        */
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && arr[left] < arr[smallest]) smallest = left;
        if (right < n && arr[right] < arr[smallest]) smallest = right;

        if (smallest != i) {
            // Swap current node with smallest child
            (arr[smallest], arr[i]) = (arr[i], arr[smallest]);

            // Continue heapifying at the child position
            Heapify(arr, n, smallest);
        }
    }

    public static void BuildHeap(int[] arr) {
        /*
            BuildHeap
            ---------
            Converts an arbitrary array into a min-heap in-place.

            Why start at n/2 - 1?
                - Indices >= n/2 are leaves (no children) and already valid heaps.
                - Internal nodes may violate the heap invariant and must be heapified.

            Complexity:
                O(n) overall.
        */
        int n = arr.Length;

        // Heapify from the last internal node down to the root.
        for (int i = n / 2 - 1; i >= 0; i--) {
            Heapify(arr, n, i);
        }
    }
}
