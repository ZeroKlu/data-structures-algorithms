/*
    This file implements the "Kth Largest Element in a Stream" pattern in JavaScript.

    Components:
      1) MinHeapK: a minimal array-backed MIN-HEAP for integers.
      2) KthLargest: maintains the k-th largest value seen so far using MinHeapK.

    Core idea (KthLargest):
      - Keep a MIN-HEAP of size at most k containing the k largest values seen so far.
      - The smallest value in that heap (heap.peek()) is the current k-th largest overall.
      - When a new value arrives:
          * If heap size < k: push it.
          * Else if value > heap.peek(): remove the smallest (pop) and push the new value.
          * Else: ignore it (it’s not in the top k).
      - Always return heap.peek() as the k-th largest.

    Complexity:
      - Each add(val) performs at most one pop and one push → O(log k)
      - Extra space is O(k)
*/

class MinHeapK {
    constructor() {
        // Backing array storing heap items in level-order.
        // Heap invariant: for every index i, data[parent(i)] <= data[i]
        this.data = [];
    }

    /*
        _bubbleUp(i)
        ------------
        Restores min-heap property after inserting a new element at index i.

        Insert pattern:
          - push(x) appends x to the end of the array
          - then bubbleUp swaps x with its parent while x < parent

        Complexity: O(log n)
    */
    _bubbleUp(i) {
        while (i > 0) {
            const p = Math.floor((i - 1) / 2); // parent index

            // If child >= parent, heap property is satisfied.
            if (this.data[i] >= this.data[p]) break;

            // Swap child and parent.
            [this.data[i], this.data[p]] = [this.data[p], this.data[i]];
            i = p; // continue from parent position
        }
    }

    /*
        _heapify(i)
        -----------
        Restores min-heap property after removing the root.

        Pop pattern:
          - Replace root with the last element
          - Then sift down: swap with the smaller child while needed

        Complexity: O(log n)
    */
    _heapify(i) {
        const n = this.data.length;

        while (true) {
            let smallest = i;
            const left = 2 * i + 1;
            const right = 2 * i + 2;

            // Find the smallest among node and its children.
            if (left < n && this.data[left] < this.data[smallest]) smallest = left;
            if (right < n && this.data[right] < this.data[smallest]) smallest = right;

            // If i is already the smallest, we're done.
            if (smallest === i) break;

            // Swap downward and continue.
            [this.data[i], this.data[smallest]] = [this.data[smallest], this.data[i]];
            i = smallest;
        }
    }

    /*
        push(x)
        -------
        Inserts a value into the min-heap.

        Steps:
          - Append to end
          - Bubble it up to restore heap property
    */
    push(x) {
        this.data.push(x);
        this._bubbleUp(this.data.length - 1);
    }

    /*
        pop()
        -----
        Removes and returns the smallest element (root) of the heap.

        Returns:
          - null if heap is empty
          - otherwise the minimum value

        Steps:
          - Save root
          - Move last element to root
          - Sift down (_heapify) to restore heap property
    */
    pop() {
        if (!this.data.length) return null;

        if (this.data.length === 1) {
            // Only element is both root and last.
            return this.data.pop();
        }

        const root = this.data[0];
        this.data[0] = this.data.pop(); // move last to root
        this._heapify(0);
        return root;
    }

    /*
        peek()
        ------
        Returns the smallest element without removing it.

        Returns:
          - null if heap is empty
          - otherwise root value
    */
    peek() {
        return this.data.length ? this.data[0] : null;
    }

    /*
        size()
        ------
        Returns the number of elements currently in the heap.
    */
    size() {
        return this.data.length;
    }
}

/*
    KthLargest
    ----------
    Maintains the k-th largest element in a stream of numbers.

    Invariant:
      - this.heap contains the k largest values seen so far (or fewer until enough values arrive).
      - Because it is a MIN-HEAP, heap.peek() is the smallest among those kept values,
        i.e., the k-th largest overall.
*/
class KthLargest {
    /*
        constructor(k, nums)
        --------------------
        Parameters:
          - k: the rank (k-th largest)
          - nums: initial array of numbers in the stream

        Strategy:
          - Save k
          - Initialize an empty MinHeapK
          - Feed all nums through add() to build initial state
    */
    constructor(k, nums) {
        this.k = k;
        this.heap = new MinHeapK();

        for (const x of nums) {
            this.add(x);
        }
    }

    /*
        add(val)
        --------
        Adds val to the stream and returns the current k-th largest.

        Algorithm:
          - If heap size < k: push val (we still need more values in our top-k set)
          - Else if val > heap.peek(): val belongs in top k → replace the cutoff:
                pop smallest (cutoff) and push val
          - Else: ignore val

        Returns:
          - heap.peek() (the current k-th largest), or null if k <= 0 or no elements

        Note:
          - This implementation assumes k > 0. If k == 0, comparisons against peek()
            and the meaning of "k-th largest" are undefined; you'd typically validate
            input and throw/return something explicit.
    */
    add(val) {
        if (this.heap.size() < this.k) {
            // Not yet holding k elements—include val.
            this.heap.push(val);
        } else if (val > this.heap.peek()) {
            // val is larger than the current cutoff (k-th largest),
            // so it should replace the cutoff in the top-k set.
            this.heap.pop();
            this.heap.push(val);
        }

        // The smallest element in the top-k heap is the k-th largest overall.
        return this.heap.peek();
    }
}

// ===== Helpers =====

/*
    printArray
    ----------
    Prints an array with a label.
*/
function printArray(label, arr) {
    console.log(label + arr.join(" "));
}

// ===== Tests =====

/*
    Test scenario:
      - Initial nums: [4, 5, 8, 2]
      - k = 3
      - Add: [3, 5, 10, 9, 4]

    Expected:
      add(3)  -> 4
      add(5)  -> 5
      add(10) -> 5
      add(9)  -> 8
      add(4)  -> 8
*/

console.log("=== Test KthLargest ===");

const nums = [4, 5, 8, 2];
const k = 3;

printArray("Initial nums: ", nums);
console.log("k =", k);
console.log();

const kth = new KthLargest(k, nums);

const toAdd = [3, 5, 10, 9, 4];

console.log("Adding values:");
for (const val of toAdd) {
    const kthVal = kth.add(val);
    console.log(`add(${val}) -> kth largest = ${kthVal}`);
}

/*
Expected sequence for k = 3 and nums = [4, 5, 8, 2]:

    add(3)  -> 4
    add(5)  -> 5
    add(10) -> 5
    add(9)  -> 8
    add(4)  -> 8

Explanation:
    The min-heap always stores exactly the current k largest elements.
    The smallest among them is the k-th largest overall.
*/
