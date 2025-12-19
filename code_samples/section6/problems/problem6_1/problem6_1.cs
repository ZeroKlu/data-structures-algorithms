// ===== Tests =====
//
// This file contains BOTH:
//   1) A lightweight "test harness" at the top (top-level statements),
//   2) A MinHeap implementation below.
//
// The tests are intentionally simple and print-oriented:
//   - Insert several values while showing the running minimum (Peek).
//   - Pop until empty to verify ascending (sorted) order.
//   - Confirm behavior when peeking/popping an empty heap (null results).
//   - Reinsert after emptying to confirm the heap remains usable.
//
// NOTE: This code uses modern C# features:
//   - Top-level statements (no explicit Main method).
//   - Collection expression syntax: int[] values = [ ... ];
//   - Index-from-end operator: data[^1]

static void Print(string label, IEnumerable<int> seq) =>
    // Utility method for printing a label followed by a sequence of ints.
    // Example output: "Values: 1 2 3"
    // This isn't used in the current test script, but can be handy
    // for quickly dumping arrays/lists during debugging.
    Console.WriteLine(label + string.Join(" ", seq));

Console.WriteLine("=== Testing MinHeap ===");

var h = new MinHeap();

// Sample values chosen to exercise multiple bubble-up operations and ordering.
// The resulting sorted pop sequence should be: 1 2 3 5 7 8
int[] values = [5, 3, 8, 1, 7, 2];

Console.Write("Inserting values: ");
foreach (var v in values) {
    // Print the value as we insert it
    Console.Write(v + " ");

    // Insert into heap (O(log n))
    h.Insert(v);

    // Peek returns the current minimum (O(1)); should track the smallest inserted so far
    Console.WriteLine($"\n  After insert {v} -> peek (min): {h.Peek()}");
}
Console.WriteLine();

// Pop repeatedly until the heap is empty.
// This should print values in ascending order if heap invariants are correct.
Console.Write("Extracting in sorted order: ");
while (true) {
    // Pop returns null when empty (instead of throwing).
    var x = h.Pop();
    if (x == null) break; // stop when heap is empty
    Console.Write(x + " ");
}
Console.WriteLine("\n");

// These calls demonstrate the "empty heap" behavior.
// Since Peek/Pop return nullable int (int?), we expect null outputs here.
Console.WriteLine("Testing peek/pop on empty heap:");
Console.WriteLine("peek(): " + h.Peek());
Console.WriteLine("pop(): " + h.Pop());
Console.WriteLine();

// Confirm the heap is reusable after becoming empty.
Console.WriteLine("Reinserting values: 10, 4, 6");
h.Insert(10);
h.Insert(4);
h.Insert(6);

// After inserting 10, 4, 6 the minimum should be 4.
Console.WriteLine($"peek(): {h.Peek()}");

// Pop again to verify ordering in a second run: expected 4 6 10.
Console.Write("Final pop sequence: ");
while (true) {
    var x = h.Pop();
    if (x == null) break;
    Console.Write(x + " ");
}
Console.WriteLine();

class MinHeap {
    /*
        MinHeap (priority queue) for integers backed by List<int>.

        Invariant:
            For every node i (except root), data[parent(i)] <= data[i]

        Array index mapping (0-based):
            parent(i) = (i - 1) / 2
            left(i)   = 2*i + 1
            right(i)  = 2*i + 2

        Complexity:
            Insert: O(log n)
            Peek:   O(1)
            Pop:    O(log n)

        Empty-heap behavior:
            - Peek() returns null when empty
            - Pop() returns null when empty

        Returning null avoids "sentinel value" ambiguity (e.g., INT_MIN),
        but it does mean callers must handle nullable results.
    */

    // Internal storage for heap elements in heap-order.
    // data[0] is always the minimum element (if non-empty).
    private readonly List<int> data = new List<int>();

    /*
        Swap
        ----
        Swaps elements at indices i and j.

        Uses tuple deconstruction assignment for a concise swap:
            (a, b) = (b, a);

        Assumes i and j are valid indices.
    */
    private void Swap(int i, int j) {
        (data[j], data[i]) = (data[i], data[j]);
    }

    /*
        BubbleUp
        --------
        Restores heap invariant after inserting an element.

        Insert places the new element at the end of the list.
        If the new element is smaller than its parent, we swap upward until:
            - we reach the root, or
            - parent <= child (heap property satisfied)

        i is the index of the newly inserted element.
    */
    private void BubbleUp(int i) {
        while (i > 0) {
            // Parent index in a binary heap stored in an array
            int p = (i - 1) / 2;

            // If current node is >= parent, heap property is satisfied
            if (data[i] >= data[p]) break;

            // Otherwise swap with parent and continue upward
            Swap(i, p);
            i = p;
        }
    }

    /*
        HeapifyDown
        -----------
        Restores heap invariant after removing the root (minimum).

        Pop typically:
            - replaces the root with the last element
            - removes the last element
            - then "pushes down" the moved root until heap property holds

        At each step:
            - compare current node with its children
            - swap with the smallest child if a child is smaller
            - stop when the current node is <= both children or has no children
    */
    private void HeapifyDown(int i) {
        int n = data.Count; // snapshot of size for bounds checks

        while (true) {
            int smallest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            // Select the smallest among (i, left child, right child), if children exist
            if (left < n && data[left] < data[smallest]) smallest = left;
            if (right < n && data[right] < data[smallest]) smallest = right;

            // If i is already the smallest, the heap property holds here
            if (smallest == i) break;

            // Swap down and continue from the child we swapped with
            Swap(i, smallest);
            i = smallest;
        }
    }

    /*
        Insert
        ------
        Adds a value to the heap.

        Steps:
            1) Append to the end of the list
            2) BubbleUp from that index to restore heap property
    */
    public void Insert(int x) {
        data.Add(x);
        BubbleUp(data.Count - 1);
    }

    /*
        Peek
        ----
        Returns the current minimum (root) WITHOUT removing it.

        Returns:
            - null if heap is empty
            - otherwise data[0]

        This is O(1).
    */
    public int? Peek() {
        return data.Count == 0 ? (int?)null : data[0];
    }

    /*
        Pop
        ---
        Removes and returns the current minimum (root).

        Cases:
            1) Empty heap:
                - return null
            2) Single-element heap:
                - remove and return that element
            3) General case:
                - save root
                - move last element to root
                - remove last element
                - heapifyDown from root
                - return saved root

        Complexity: O(log n) in the general case.
    */
    public int? Pop() {
        if (data.Count == 0) return null;

        // Fast path: one element only
        if (data.Count == 1) {
            int v = data[0];

            // RemoveAt(0) is fine here because count==1 (no shifting cost concerns)
            data.RemoveAt(0);
            return v;
        }

        // Save the minimum value to return
        int root = data[0];

        // Move the last element into the root position
        data[0] = data[^1]; // ^1 = last element

        // Remove the last element (now duplicated at the root)
        data.RemoveAt(data.Count - 1);

        // Restore heap property starting from root
        HeapifyDown(0);

        return root;
    }
}
