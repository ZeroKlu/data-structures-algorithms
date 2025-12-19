package code_samples.section6.lesson;

/*
============================================================
MIN-HEAP OVERVIEW
============================================================

We store a binary heap in an array / ArrayList using standard indexing:

  parent(i) = (i - 1) / 2
  left(i)   = 2*i + 1
  right(i)  = 2*i + 2

Min-heap property:
  Every parent value <= its children values

This file contains:
1) MinHeap (insert only)              -> demonstrates heapify-up
2) MinHeapWithExtract (insert+extract)-> adds heapify-down on extract
3) Array helpers:
   - heapifyDownArray
   - buildMinHeap
   - heapsortDescMinHeap (descending sort using a min-heap)
4) A test harness consistent with your earlier C/C++ style
*/

// ============================================================
// 1) MIN-HEAP (INSERT ONLY)
// ============================================================
//
// This class supports only insert(). It demonstrates heapify-up
// but doesn't provide extract/peek, so you can't directly observe
// ordering by removing elements.
class MinHeap {
    // Underlying storage for heap elements in level-order
    private java.util.ArrayList<Integer> data = new java.util.ArrayList<>();

    // Insert a value into the min-heap.
    //
    // Steps:
    // 1) Append value at the end of the array
    // 2) Heapify up: while parent > child, swap upward
    public void insert(int value) {
        // 1) Add to the end (next available position)
        data.add(value);

        // i is the index of the newly inserted value
        int i = data.size() - 1;

        // 2) Heapify up (bubble up)
        while (i > 0) {
            int parent = (i - 1) / 2; // compute parent index

            // If heap property holds, stop
            if (data.get(parent) <= data.get(i)) break;

            // Otherwise swap parent and child
            int tmp = data.get(parent);
            data.set(parent, data.get(i));
            data.set(i, tmp);

            // Continue from the parent's position
            i = parent;
        }
    }
}

// ============================================================
// 2) MIN-HEAP (INSERT + EXTRACT MIN)
// ============================================================
//
// Adds extractMin(), which removes the root (minimum).
class MinHeapWithExtract {
    // Underlying storage for heap elements in level-order
    private java.util.ArrayList<Integer> data = new java.util.ArrayList<>();

    // Insert value into the heap (same heapify-up logic as MinHeap)
    public void insert(int value) {
        data.add(value);
        int i = data.size() - 1;

        // Heapify up (bubble up)
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (data.get(parent) <= data.get(i)) break;

            int tmp = data.get(parent);
            data.set(parent, data.get(i));
            data.set(i, tmp);
            i = parent;
        }
    }

    // Remove and return the smallest element (root of the heap).
    //
    // Steps:
    // 1) Save root value (min)
    // 2) Remove last element
    // 3) If heap isn't empty, move last element to root
    // 4) Heapify down from root to restore heap property
    public int extractMin() {
        // Heap empty => underflow
        if (data.isEmpty()) {
            throw new IllegalStateException("heap underflow");
        }

        // 1) The minimum is always the root at index 0
        int minVal = data.get(0);

        // 2) Remove last element (we'll move it to the root if needed)
        int last = data.remove(data.size() - 1);

        // If we removed the only element, we're done
        if (data.isEmpty()) {
            return minVal;
        }

        // 3) Move last element to root
        data.set(0, last);

        // 4) Heapify down (bubble down)
        int n = data.size();
        int i = 0;

        while (true) {
            int left = 2 * i + 1;   // left child
            int right = 2 * i + 2;  // right child
            int smallest = i;       // assume current node is smallest

            // Find smallest among current node and its children
            if (left < n && data.get(left) < data.get(smallest)) {
                smallest = left;
            }
            if (right < n && data.get(right) < data.get(smallest)) {
                smallest = right;
            }

            // If no child is smaller, heap property is restored
            if (smallest == i) break;

            // Otherwise swap with the smaller child and continue down
            int tmp = data.get(i);
            data.set(i, data.get(smallest));
            data.set(smallest, tmp);

            i = smallest;
        }

        return minVal;
    }
}

// ============================================================
// 3) ARRAY-BASED HEAP HELPERS (build heap + heapsort)
// ============================================================

public class section6 {

    // Heapify down for an int[] treated as a heap.
    //
    // Parameters:
    // - a: array holding heap values
    // - n: heap size (active region is a[0..n-1])
    // - i: index to heapify down from
    void heapifyDownArray(int[] a, int n, int i) {
        // Sift element at i downward until min-heap property holds
        while (true) {
            int left = 2 * i + 1;   // left child index
            int right = 2 * i + 2;  // right child index
            int smallest = i;       // assume i is smallest

            // Compare with left child
            if (left < n && a[left] < a[smallest]) smallest = left;

            // Compare with right child
            if (right < n && a[right] < a[smallest]) smallest = right;

            // If i is already smallest, stop
            if (smallest == i) break;

            // Swap current node with smallest child
            int tmp = a[i];
            a[i] = a[smallest];
            a[smallest] = tmp;

            // Continue heapifying from the child position
            i = smallest;
        }
    }

    // Build a min-heap from an arbitrary array in-place.
    //
    // Bottom-up construction:
    // - Leaves are already heaps
    // - Start from last internal node (n/2 - 1) down to 0
    void buildMinHeap(int[] a) {
        int n = a.length;
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapifyDownArray(a, n, i);
        }
    }

    // Heapsort using a min-heap produces DESCENDING order.
    //
    // Why descending?
    // - Each iteration swaps the minimum (root) to the end.
    // - That places small elements at the back first.
    void heapsortDescMinHeap(int[] a) {
        // 1) Build min-heap
        buildMinHeap(a);

        // 2) Repeatedly move min to the end of the remaining heap region
        for (int end = a.length - 1; end > 0; end--) {
            // Swap root (min) with a[end]
            int tmp = a[0];
            a[0] = a[end];
            a[end] = tmp;

            // Restore min-heap in the reduced region [0..end-1]
            heapifyDownArray(a, end, 0);
        }
    }

    // ============================================================
    // 4) TESTS (same style as C / C++)
    // ============================================================

    public static void main(String[] args) {
        // --- Test MinHeap.insert (no extract) ---
        System.out.println("=== Testing MinHeap.insert ===");
        MinHeap h1 = new MinHeap();
        int[] valuesToInsert = { 5, 3, 8, 1, 7, 2 };

        // This test only verifies insert runs; there is no extraction API
        System.out.print("Inserting into MinHeap (no extract available): ");
        for (int v : valuesToInsert) {
            System.out.print(v + " ");
            h1.insert(v);  // exercise insert + heapify-up
        }
        System.out.println();
        System.out.println();

        // --- Test MinHeapWithExtract.insert / extractMin ---
        System.out.println("=== Testing MinHeapWithExtract insert / extractMin ===");
        MinHeapWithExtract h2 = new MinHeapWithExtract();

        // Insert values
        System.out.print("Inserting: ");
        for (int v : valuesToInsert) {
            System.out.print(v + " ");
            h2.insert(v);
        }
        System.out.println();

        // Extract until empty; should print ascending order: 1 2 3 5 7 8
        System.out.print("Extracting in sorted order: ");
        while (true) {
            try {
                int minVal = h2.extractMin();
                System.out.print(minVal + " ");
            } catch (IllegalStateException e) {
                // heap underflow => done
                break;
            }
        }
        System.out.println();
        System.out.println();

        // --- Test buildMinHeap (uses heapifyDownArray) ---
        System.out.println("=== Testing buildMinHeap ===");
        section6 s = new section6();

        int[] arr1 = { 9, 4, 7, 1, 0, 3, 2 };
        System.out.print("Original array: ");
        printArray(arr1);

        // Convert arr1 into a min-heap in-place
        s.buildMinHeap(arr1);

        System.out.print("After buildMinHeap (min-heap array): ");
        printArray(arr1);
        System.out.println();

        // --- Test heapsortDescMinHeap ---
        System.out.println("=== Testing heapsortDescMinHeap (descending sort) ===");
        int[] arr2 = { 12, 3, 17, 8, 34, 25, 1 };

        System.out.print("Original array: ");
        printArray(arr2);

        // Sort descending
        s.heapsortDescMinHeap(arr2);

        System.out.print("After heapsortDescMinHeap (descending): ");
        printArray(arr2);
    }

    // Print an int array on one line
    private static void printArray(int[] a) {
        for (int v : a) {
            System.out.print(v + " ");
        }
        System.out.println();
    }
}
