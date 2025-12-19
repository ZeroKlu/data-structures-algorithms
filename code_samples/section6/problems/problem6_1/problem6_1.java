package code_samples.section6.problems.problem6_1;

/*
    MinHeap implementation backed by a dynamically resized int array.

    A min-heap maintains the invariant:
        For every node i (except the root),
            data[parent(i)] <= data[i]

    With this invariant:
        - The minimum element is always at index 0
        - insert runs in O(log n)
        - pop (remove-min) runs in O(log n)
        - peek runs in O(1)

    Array index relationships (0-based):
        parent(i) = (i - 1) / 2
        left(i)   = 2*i + 1
        right(i)  = 2*i + 2

    Empty-heap behavior:
        - peek() returns null
        - pop() returns null

    This avoids sentinel values and forces callers to handle emptiness explicitly.
*/

class MinHeap {

    // Backing array storing heap elements in heap-order
    private int[] data;

    // Number of valid elements currently stored in the heap
    private int size;

    /*
        Constructor
        -----------
        Creates a MinHeap with an initial capacity.

        capacity:
            - initial size of the backing array
            - should be > 0 for practical use
    */
    public MinHeap(int capacity) {
        data = new int[capacity];
        size = 0;
    }

    /*
        ensureCapacity
        --------------
        Ensures that the backing array has room for at least one more element.

        Strategy:
            - If size < data.length, there is space → do nothing.
            - Otherwise, allocate a new array with double the capacity and
              copy existing elements into it.

        This doubling strategy provides amortized O(1) growth.
    */
    private void ensureCapacity() {
        if (size < data.length) return;

        int[] nd = new int[data.length * 2];
        System.arraycopy(data, 0, nd, 0, data.length);
        data = nd;
    }

    /*
        swap
        ----
        Swaps the elements at indices i and j in the backing array.

        Assumes:
            - i and j are valid indices within [0, size).
    */
    private void swap(int i, int j) {
        int tmp = data[i];
        data[i] = data[j];
        data[j] = tmp;
    }

    /*
        bubbleUp
        --------
        Restores the heap invariant after inserting a new element.

        After insertion, the new element is placed at index i (initially = size).
        While it is smaller than its parent, it is swapped upward.

        Stops when:
            - i becomes 0 (root), or
            - data[parent(i)] <= data[i].

        Worst-case complexity: O(log n).
    */
    private void bubbleUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2; // parent index

            // If parent is already <= current, heap property holds
            if (data[i] >= data[p]) break;

            // Otherwise, swap with parent and continue upward
            swap(i, p);
            i = p;
        }
    }

    /*
        heapifyDown
        -----------
        Restores the heap invariant after removing the root element.

        When pop() removes the minimum:
            - The last element is moved to index 0.
            - That element may be larger than its children.
            - heapifyDown pushes it downward until heap property is restored.

        At each step:
            - Compare node i with its left and right children.
            - Swap with the smallest of the three if needed.
            - Continue from the swapped child index.

        Worst-case complexity: O(log n).
    */
    private void heapifyDown(int i) {
        int n = size;

        while (true) {
            int smallest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            // Choose the smallest among current node and its children
            if (left < n && data[left] < data[smallest]) smallest = left;
            if (right < n && data[right] < data[smallest]) smallest = right;

            // If the current node is already the smallest, stop
            if (smallest == i) break;

            // Swap downward and continue
            swap(i, smallest);
            i = smallest;
        }
    }

    /*
        insert
        ------
        Inserts a new value into the heap.

        Steps:
            1) Ensure backing array has space.
            2) Place value at index size.
            3) bubbleUp from that index to restore heap invariant.
            4) Increment size.

        Complexity: O(log n).
    */
    public void insert(int x) {
        ensureCapacity();
        data[size] = x;
        bubbleUp(size);
        size++;
    }

    /*
        peek
        ----
        Returns the minimum element without removing it.

        Returns:
            - null if the heap is empty
            - otherwise, the value at data[0]

        Complexity: O(1).
    */
    public Integer peek() {
        return size == 0 ? null : data[0];
    }

    /*
        pop
        ---
        Removes and returns the minimum element (root).

        Cases:
            1) Empty heap:
                - return null
            2) Non-empty heap:
                - save root
                - decrement size
                - move last element to root (if any remain)
                - heapifyDown from root
                - return saved root

        Complexity: O(log n) in the non-empty case.
    */
    public Integer pop() {
        if (size == 0) return null;

        int root = data[0];
        size--;

        if (size > 0) {
            // Move last element into root position
            data[0] = data[size];
            heapifyDown(0);
        }

        return root;
    }
}

/*
    Test harness for MinHeap.

    Exercises:
        - Insert values while checking the running minimum
        - Pop all values to verify ascending (sorted) order
        - Behavior of peek/pop on an empty heap
        - Reinsertion after emptying the heap
*/

public class problem6_1 {

    public static void main(String[] args) {
        System.out.println("=== Testing MinHeap ===");

        // Small initial capacity to force ensureCapacity() to run
        MinHeap h = new MinHeap(4);
        int[] values = {5, 3, 8, 1, 7, 2};

        System.out.print("Inserting values: ");
        for (int v : values) {
            System.out.print(v + " ");
            h.insert(v);

            // After each insert, peek should return the smallest value so far
            System.out.println("\n  After insert " + v + " -> peek (min): " + h.peek());
        }
        System.out.println();

        // Repeated popping should produce values in ascending order
        System.out.println("Extracting values in sorted order:");
        while (true) {
            Integer x = h.pop();
            if (x == null) break;
            System.out.print(x + " ");
        }
        System.out.println("\n");

        // Verify empty-heap behavior
        System.out.println("Testing peek/pop on empty heap:");
        System.out.println("peek(): " + h.peek());
        System.out.println("pop(): " + h.pop());
        System.out.println();

        // Confirm the heap can be reused after being emptied
        System.out.println("Reinserting values: 10, 4, 6");
        h.insert(10);
        h.insert(4);
        h.insert(6);

        System.out.println("peek(): " + h.peek());

        System.out.print("Final pop sequence: ");
        while (true) {
            Integer x = h.pop();
            if (x == null) break;
            System.out.print(x + " ");
        }
        System.out.println();
    }
}
