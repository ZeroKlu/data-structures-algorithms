/*
    MinHeap implementation (priority queue) for numbers in JavaScript.

    Min-heap invariant:
        For every node i (except the root),
            data[parent(i)] <= data[i]

    With this invariant:
        - The minimum element is always at data[0]
        - insert(x): O(log n) via bubble-up
        - peek():    O(1)
        - pop():     O(log n) via heapify-down

    Array index relationships (0-based):
        parent(i) = Math.floor((i - 1) / 2)
        left(i)   = 2*i + 1
        right(i)  = 2*i + 2

    Empty-heap behavior:
        - peek() returns null if empty
        - pop() returns null if empty

    Notes:
        - This heap stores "numbers" (or any values that are safely comparable using < / >=).
        - For objects/custom ordering, you'd typically pass a comparator function instead.
*/

class MinHeap {
    constructor() {
        // Backing array that stores elements in heap-order.
        // data[0] is always the minimum element (when non-empty).
        this.data = [];
    }

    /*
        insert(x)
        ---------
        Inserts a value into the heap.

        Steps:
          1) Append x to the end of the array.
          2) Bubble it up until the heap invariant is restored.

        Complexity: O(log n).
    */
    insert(x) {
        this.data.push(x);
        this._bubbleUp(this.data.length - 1);
    }

    /*
        peek()
        ------
        Returns the minimum element without removing it.

        Returns:
          - null if heap is empty
          - otherwise the root at index 0

        Complexity: O(1).
    */
    peek() {
        return this.data.length ? this.data[0] : null;
    }

    /*
        pop()
        -----
        Removes and returns the minimum element (root).

        Cases:
          1) Empty heap:
                return null
          2) One element:
                pop and return it (fast path)
          3) General case:
                - save root
                - move last element to root position
                - heapify down from root
                - return saved root

        Complexity: O(log n) in the general case.
    */
    pop() {
        if (this.data.length === 0) return null;

        // Fast path: single element; Array#pop returns it and leaves heap empty.
        if (this.data.length === 1) return this.data.pop();

        // Save min value to return.
        const root = this.data[0];

        // Move the last element to the root position and shrink the array.
        // After this, the heap property may be violated at index 0.
        this.data[0] = this.data.pop();

        // Restore heap invariant by pushing the new root down as needed.
        this._heapify(0);

        return root;
    }

    /*
        _bubbleUp(i)
        ------------
        Internal helper used after insertion.

        The new element starts at index i (the last position).
        While it is smaller than its parent, swap with parent.

        Stops when:
          - i reaches 0 (root), OR
          - element >= parent (heap invariant satisfied)

        Complexity: O(log n).
    */
    _bubbleUp(i) {
        while (i > 0) {
            const parent = Math.floor((i - 1) / 2);

            // If parent is already <= current, heap property holds.
            if (this.data[i] >= this.data[parent]) break;

            // Swap current with parent using destructuring assignment.
            [this.data[i], this.data[parent]] = [this.data[parent], this.data[i]];

            // Continue bubbling up from the parent's index.
            i = parent;
        }
    }

    /*
        _heapify(i)
        -----------
        Internal helper used after removing the root (pop).

        Starting from index i (usually 0), repeatedly:
          - find the smallest among current node and its children
          - swap with the smallest child if the child is smaller
          - continue from the swapped index

        Stops when:
          - current node is <= both children, OR
          - it has no children

        Complexity: O(log n).
    */
    _heapify(i) {
        const n = this.data.length;

        while (true) {
            let smallest = i;
            const left = 2 * i + 1;
            const right = 2 * i + 2;

            // Compare with left child (if it exists)
            if (left < n && this.data[left] < this.data[smallest]) smallest = left;

            // Compare with right child (if it exists)
            if (right < n && this.data[right] < this.data[smallest]) smallest = right;

            // If the current node is already the smallest, heap property holds here.
            if (smallest === i) break;

            // Otherwise swap down and keep going.
            [this.data[i], this.data[smallest]] = [this.data[smallest], this.data[i]];
            i = smallest;
        }
    }
}

// ===== Tests =====
//
// This is a simple console-based test harness that validates the heap behavior by:
//   - inserting a set of values and printing the running minimum,
//   - popping all values to confirm sorted (ascending) output,
//   - checking peek/pop behavior on an empty heap,
//   - reinserting values and repeating the pop test.
//
// Note: printArray() is included but not used below; it can help debug arrays quickly.

function printArray(arr) {
    // Prints elements separated by spaces on a single line.
    console.log(arr.join(" "));
}

console.log("=== Testing MinHeap ===");

const h = new MinHeap();
const values = [5, 3, 8, 1, 7, 2];

process.stdout.write("Inserting values: ");
for (const v of values) {
    process.stdout.write(v + " ");

    // Insert each value, then confirm the heap's minimum via peek().
    h.insert(v);

    console.log(`\n  After insert ${v} -> peek (min): ${h.peek()}`);
}
console.log();

// Repeated pop should produce ascending order if heap is correct.
console.log("Extracting in sorted order:");
while (true) {
    const v = h.pop();
    if (v === null) break; // stop once heap reports empty
    process.stdout.write(v + " ");
}
console.log("\n");

// Confirm empty-heap behavior (both should print null).
console.log("Testing peek/pop on empty heap:");
console.log("peek():", h.peek());
console.log("pop():", h.pop());
console.log();

// Confirm the heap can be reused after being emptied.
console.log("Reinserting values: 10, 4, 6");
h.insert(10);
h.insert(4);
h.insert(6);

// After inserting 10, 4, 6, the minimum should be 4.
console.log("peek():", h.peek());

// Pop again (expected order: 4 6 10).
process.stdout.write("Final pop sequence: ");
while (true) {
    const v = h.pop();
    if (v === null) break;
    process.stdout.write(v + " ");
}
console.log();
