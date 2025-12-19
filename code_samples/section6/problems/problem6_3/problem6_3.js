/*
    This file implements:
      1) A simple array-backed MaxHeap for integers
      2) kSmallest(arr, k): finds the k smallest values using that max-heap
      3) kSmallestSafe(arr, k): guards edge cases (k <= 0, null/empty array)
      4) A print-based test harness

    Why use a MAX-HEAP to find k smallest?
      - We want to keep the k smallest elements seen so far.
      - If we store those k elements in a max-heap, the root is the *largest*
        among the kept k.
      - That root value is the "cutoff": any new value smaller than the cutoff
        belongs in the set, and can replace the cutoff.

    Heap index math (0-based array):
      - parent(i) = Math.floor((i - 1) / 2)
      - left(i)   = 2*i + 1
      - right(i)  = 2*i + 2

    Complexity:
      - Heap operations are O(log k)
      - Processing n elements → O(n log k) time
      - Heap stores at most k elements → O(k) space

    Note:
      - The returned k smallest elements are NOT sorted (heap order / insertion effects).
      - Tests sort a copy for easy verification.
*/

// Simple max-heap for integers:
class MaxHeap {
    constructor() {
        // Backing array that stores heap elements in "heap order".
        // For a max-heap, data[0] is always the maximum element (when non-empty).
        this.data = [];
    }

    /*
        _bubbleUp(i)
        ------------
        Restores the max-heap invariant after inserting a new element.

        Insertion strategy:
          1) push() appends new element at the end of the array.
          2) That element may be larger than its parent.
          3) While child > parent, swap upward.

        Stops when:
          - i reaches 0 (root), or
          - data[i] <= data[parent] (heap property satisfied)

        Complexity: O(log n) (heap height)
    */
    _bubbleUp(i) {
        while (i > 0) {
            const p = Math.floor((i - 1) / 2);

            // Max-heap: if child <= parent, the property holds.
            if (this.data[i] <= this.data[p]) break;

            // Swap child with parent.
            [this.data[i], this.data[p]] = [this.data[p], this.data[i]];
            i = p;
        }
    }

    /*
        _heapify(i)
        -----------
        Restores the max-heap invariant after removing/replacing the root.

        Pop strategy:
          1) Save root (max).
          2) Move last element to root.
          3) Sift down until parent >= both children.

        At each step:
          - Choose the largest among node i and its children.
          - If a child is larger, swap and continue from that child.

        Complexity: O(log n)
    */
    _heapify(i) {
        const n = this.data.length;
        while (true) {
            let largest = i;
            const left = 2 * i + 1;
            const right = 2 * i + 2;

            // Compare against left child (if it exists)
            if (left < n && this.data[left] > this.data[largest]) largest = left;

            // Compare against right child (if it exists)
            if (right < n && this.data[right] > this.data[largest]) largest = right;

            // If i is already the largest, heap property holds here.
            if (largest === i) break;

            // Swap current node with the larger child and keep going.
            [this.data[i], this.data[largest]] = [this.data[largest], this.data[i]];
            i = largest;
        }
    }

    /*
        push(x)
        -------
        Inserts a value into the max-heap.

        Steps:
          1) Append x to end
          2) Bubble it up

        Complexity: O(log n)
    */
    push(x) {
        this.data.push(x);
        this._bubbleUp(this.data.length - 1);
    }

    /*
        peek()
        ------
        Returns the maximum element without removing it.

        Returns:
          - null if heap is empty
          - otherwise the root at data[0]

        Complexity: O(1)
    */
    peek() {
        return this.data.length ? this.data[0] : null;
    }

    /*
        pop()
        -----
        Removes and returns the maximum element (root).

        Cases:
          - empty heap → null
          - one element → pop and return it
          - otherwise:
              * save root
              * move last element to root
              * heapify down
              * return saved root

        Complexity: O(log n)
    */
    pop() {
        if (!this.data.length) return null;
        if (this.data.length === 1) return this.data.pop();

        const root = this.data[0];
        this.data[0] = this.data.pop();
        this._heapify(0);
        return root;
    }

    /*
        size()
        ------
        Returns the current number of elements in the heap.
    */
    size() {
        return this.data.length;
    }
}

/*
    kSmallest(arr, k)
    -----------------
    Returns the k smallest elements from arr (any order), using a max-heap of size k.

    Algorithm:
      - Maintain a max-heap of at most k elements.
      - For each x in arr:
          * If heap.size < k: push x
          * Else if x < heap.peek(): pop the current max, push x
          * Else ignore x

    Edge cases:
      - If k <= 0, this function will behave oddly because heap.peek() can be null.
        That’s why kSmallestSafe exists.

    Complexity:
      O(n log k)
*/
function kSmallest(arr, k) {
    const heap = new MaxHeap();

    for (const x of arr) {
        if (heap.size() < k) {
            // Fill heap until it holds k elements
            heap.push(x);
        } else if (x < heap.peek()) {
            /*
                heap.peek() is the largest among the currently kept k smallest.
                If x is smaller, x belongs in the kept set.
            */
            heap.pop();
            heap.push(x);
        }
        // Else x is not among the k smallest; ignore it.
    }

    /*
        Return a shallow copy of the heap array.

        Important:
          - This exposes internal heap order (not sorted).
          - We copy so callers can't accidentally mutate heap.data in-place.
          - If you wanted them sorted, you could sort the returned copy.
    */
    return heap.data.slice(); // k smallest, any order
}

// ===== Safe wrapper to avoid null peek() issues when k <= 0 =====
function kSmallestSafe(arr, k) {
    /*
        Guard against:
          - k <= 0 (no elements requested)
          - null/undefined arr
          - empty array

        Returning [] makes the behavior explicit and avoids calling kSmallest
        in a situation where heap.peek() could be null.
    */
    if (k <= 0 || !arr || arr.length === 0) return [];
    return kSmallest(arr, k);
}

// ===== Helpers =====

/*
    printArray
    ----------
    Utility function to print a label followed by array values.
*/
function printArray(label, arr) {
    console.log(label + arr.join(" "));
}

// ===== Tests =====
//
// These tests validate:
//   1) k <= n → returns exactly k smallest elements
//   2) k > n  → returns all elements
//   3) k = 0  → returns empty array
//
// For readability, each result is also sorted (without changing the original result).

const arr = [7, 2, 9, 4, 1, 5, 8, 3, 6];

// ---- Test 1: k <= n ----
console.log("=== Test 1: k <= n ===");
printArray("Input array: ", arr);

let k = 4;
console.log("k =", k);

let res1 = kSmallestSafe(arr, k);
printArray("kSmallest result (heap order): ", res1);

// Sort for easy verification; expected: 1 2 3 4
let sorted1 = [...res1].sort((a, b) => a - b);
printArray("kSmallest result (sorted): ", sorted1);
console.log("(Expected: 1 2 3 4)\n");

// ---- Test 2: k > n ----
console.log("=== Test 2: k > n ===");
k = 20;
console.log("k =", k);

let res2 = kSmallestSafe(arr, k);
printArray("kSmallest result (heap order): ", res2);

// With k > n, expect all elements sorted
let sorted2 = [...res2].sort((a, b) => a - b);
printArray("kSmallest result (sorted): ", sorted2);
console.log("(Expected: all elements sorted)\n");

// ---- Test 3: k = 0 ----
console.log("=== Test 3: k = 0 ===");
k = 0;
console.log("k =", k);

let res3 = kSmallestSafe(arr, k);
printArray("kSmallest result: ", res3);
console.log("(Expected: empty array)\n");
