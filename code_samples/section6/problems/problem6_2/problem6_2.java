package code_samples.section6.problems.problem6_2;

/*
    Problem 6.2 - Build a Min-Heap from an Array (Bottom-Up Heap Construction)

    This file contains:
      1) HeapUtils.buildHeap(arr): converts an arbitrary int[] into a MIN-HEAP in place
      2) A small test harness (problem6_2.main) that:
           - prints the original array
           - heapifies it in place
           - prints the heap array representation (heap order, not sorted order)
           - clones and repeatedly pops min to validate ascending output

    Min-heap invariant (0-based):
        For every i > 0:
            arr[parent(i)] <= arr[i]

    Index formulas:
        parent(i) = (i - 1) / 2
        left(i)   = 2*i + 1
        right(i)  = 2*i + 2

    Complexity:
        - buildHeap: O(n) (bottom-up)
        - heapify / heapifyDown: O(log n)
        - popMin (test helper): O(log n)

    Notes:
        - HeapUtils.heapify() is implemented recursively.
          For very large arrays, an iterative approach can avoid deep recursion.
        - popMin uses Integer.MIN_VALUE as a sentinel for "empty".
          That’s fine for this demo, but ambiguous if MIN_VALUE is a valid element.
          Production code often uses:
            * OptionalInt
            * a boolean return + out parameter
            * or exceptions
*/

class HeapUtils {

    /*
        heapify (recursive sift-down)
        -----------------------------
        Ensures the subtree rooted at index i satisfies the min-heap property,
        assuming its children subtrees already satisfy the property.

        Parameters:
            arr : backing array containing heap elements
            n   : heap size (valid elements are arr[0..n-1])
            i   : subtree root index to fix

        Algorithm:
            1) Find the smallest among i, left child, right child.
            2) If smallest != i, swap and recurse at smallest.
            3) Stop when i is already <= both children.

        Complexity:
            O(log n) worst case.
    */
    private static void heapify(int[] arr, int n, int i) {
        int smallest = i;
        int left = 2 * i + 1;   // left child
        int right = 2 * i + 2;  // right child

        // Choose the smallest among current node and children (if they exist)
        if (left < n && arr[left] < arr[smallest]) smallest = left;
        if (right < n && arr[right] < arr[smallest]) smallest = right;

        // If a child is smaller, swap and continue fixing downward
        if (smallest != i) {
            int tmp = arr[i];
            arr[i] = arr[smallest];
            arr[smallest] = tmp;

            // After swap, the heap property might be violated further down
            heapify(arr, n, smallest);
        }
    }

    /*
        buildHeap
        ---------
        Converts an arbitrary array into a min-heap in place using bottom-up construction.

        Key insight:
            - All leaves are already valid heaps (size 1).
            - Leaves begin at index n/2 (integer division).
            - Therefore, the last internal node is at index n/2 - 1.
            - Heapify each internal node from n/2 - 1 down to 0.

        Complexity:
            O(n) overall.
    */
    public static void buildHeap(int[] arr) {
        int n = arr.length;

        // Start from last internal node and heapify down to the root
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(arr, n, i);
        }
    }
}

public class problem6_2 {

    /*
        heapifyDown (iterative sift-down)
        --------------------------------
        Similar to HeapUtils.heapify(), but:
          - iterative (no recursion)
          - operates on a heap stored in the prefix arr[0..size-1]

        This is used by popMin() while extracting values, because the "logical heap size"
        shrinks after each pop. Using arr.length would be incorrect during extraction.

        Parameters:
            arr  : heap array
            size : current heap size (prefix length)
            i    : index to sift down from (usually 0)

        Complexity:
            O(log size).
    */
    private static void heapifyDown(int[] arr, int size, int i) {
        while (true) {
            int smallest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            // Compare against children within the active heap prefix
            if (left < size && arr[left] < arr[smallest]) smallest = left;
            if (right < size && arr[right] < arr[smallest]) smallest = right;

            // If i is already the smallest, heap property is satisfied
            if (smallest == i) break;

            // Otherwise swap with the smaller child and continue downward
            int tmp = arr[i];
            arr[i] = arr[smallest];
            arr[smallest] = tmp;

            i = smallest;
        }
    }

    /*
        popMin
        ------
        Removes and returns the minimum element from an array-based min-heap stored
        in arr[0..size-1], where size is tracked externally.

        Why sizeRef is an int[]:
            - Java passes primitives by value. Using a single-element array is a common
              workaround to emulate "pass-by-reference" for an int.
            - sizeRef[0] is the current heap size and gets decremented.

        Parameters:
            arr     : heap array
            sizeRef : single-element array holding current heap size

        Returns:
            - Integer.MIN_VALUE if the heap is empty
            - otherwise the removed minimum

        Caveat:
            Integer.MIN_VALUE is ambiguous if it could appear as a real heap element.
            For robust APIs consider returning OptionalInt or throwing on empty.
    */
    private static int popMin(int[] arr, int[] sizeRef) {
        int size = sizeRef[0];
        if (size == 0) return Integer.MIN_VALUE;

        // Minimum is always at the root
        int root = arr[0];

        // Shrink heap
        size--;
        sizeRef[0] = size;

        if (size > 0) {
            // Move last element to root and restore heap property
            arr[0] = arr[size];
            heapifyDown(arr, size, 0);
        }

        return root;
    }

    /*
        printArray
        ----------
        Utility function to print an array with a label.
        (Remember: printing a heap array shows heap-order, not sorted order.)
    */
    private static void printArray(String label, int[] arr) {
        System.out.print(label);
        for (int x : arr) System.out.print(x + " ");
        System.out.println();
    }

    public static void main(String[] args) {
        /*
            Test harness

            Steps:
              1) Create an unsorted array
              2) Print it
              3) Build a min-heap in-place
              4) Print heap representation
              5) Clone heap and repeatedly pop min to verify sorted output

            Expected sorted output for {9,4,7,1,0,3,2}:
                0 1 2 3 4 7 9
        */
        int[] arr = {9, 4, 7, 1, 0, 3, 2};

        printArray("Original array: ", arr);

        // Build the heap in-place
        HeapUtils.buildHeap(arr);

        // This is heap layout, not sorted layout
        printArray("After buildHeap (min-heap array): ", arr);

        System.out.print("Extracting elements in sorted order: ");

        // Use a copy because extraction mutates the heap
        int[] heapCopy = arr.clone();

        // Track logical heap size separately as we pop elements
        int[] sizeRef = { heapCopy.length };

        while (sizeRef[0] > 0) {
            int v = popMin(heapCopy, sizeRef);
            System.out.print(v + " ");
        }
        System.out.println();
    }
}
