/*
    This file merges K sorted singly linked lists into one sorted list using
    a custom min-heap of ListNode objects (ordered by node.val).

    High-level algorithm (k-way merge):
      1) Push the head of each non-empty list into a MIN-HEAP.
      2) Repeatedly:
           - Pop the smallest node from the heap
           - Append it to the merged result
           - If that node has a next node, push node.next into the heap
      3) Stop when the heap is empty.

    Why a min-heap?
      - At any moment, the next smallest value must be among the current heads
        of the remaining lists.
      - A min-heap allows extracting that smallest head in O(log k).

    Complexity:
      - Let N be the total number of nodes across all lists.
      - Heap size is at most k (one "front" node per list).
      - Each node is pushed/popped at most once → O(N log k) time, O(k) heap space.

    Memory behavior:
      - mergeKLists REUSES existing nodes (it relinks them); no new nodes are created
        except the dummy node used to simplify list construction.
*/

// Basic singly-linked list node.
class ListNode {
    constructor(val, next = null) {
        // Node value (used for ordering)
        this.val = val;

        // Pointer to next node (or null if end of list)
        this.next = next;
    }
}

// Minimal min-heap of ListNode by node.val
class NodeMinHeap {
    constructor() {
        // Backing array representing the binary heap structure.
        // For a min-heap, data[0] is always the smallest node.
        this.data = [];
    }

    /*
        _bubbleUp(i)
        ------------
        Fixes heap after inserting a new element at index i.

        Insertion strategy:
          - push() appends new element at end of array
          - then bubble it up while it is smaller than its parent

        Stops when:
          - it reaches the root, or
          - parent value <= child value (min-heap property satisfied)

        Complexity: O(log n)
    */
    _bubbleUp(i) {
        while (i > 0) {
            const p = Math.floor((i - 1) / 2); // parent index

            // If child >= parent, min-heap property holds.
            if (this.data[i].val >= this.data[p].val) break;

            // Swap child and parent, then continue upward.
            [this.data[i], this.data[p]] = [this.data[p], this.data[i]];
            i = p;
        }
    }

    /*
        _heapify(i)
        -----------
        Fixes heap after removing the root / moving last element to the root.

        Pop strategy:
          - Save root
          - Move last element to root
          - Sift down: swap with smallest child until heap property holds

        Complexity: O(log n)
    */
    _heapify(i) {
        const n = this.data.length;

        while (true) {
            let smallest = i;
            const left = 2 * i + 1;
            const right = 2 * i + 2;

            // Pick the smallest among i and its children (if they exist).
            if (left < n && this.data[left].val < this.data[smallest].val) smallest = left;
            if (right < n && this.data[right].val < this.data[smallest].val) smallest = right;

            // If i is already smallest, heap property holds.
            if (smallest === i) break;

            // Otherwise swap downward and continue.
            [this.data[i], this.data[smallest]] = [this.data[smallest], this.data[i]];
            i = smallest;
        }
    }

    /*
        push(node)
        ----------
        Inserts a ListNode into the heap.

        NOTE:
          - Caller should not pass null, because we access node.val.

        Complexity: O(log n)
    */
    push(node) {
        this.data.push(node);
        this._bubbleUp(this.data.length - 1);
    }

    /*
        pop()
        -----
        Removes and returns the smallest node (root) from the heap.

        Returns:
          - null if heap is empty
          - otherwise the smallest ListNode

        Complexity: O(log n)
    */
    pop() {
        if (!this.data.length) return null;

        if (this.data.length === 1) {
            // Single element is both root and last element.
            return this.data.pop();
        }

        const root = this.data[0];

        // Move last element to root and restore heap property.
        this.data[0] = this.data.pop();
        this._heapify(0);

        return root;
    }

    /*
        size getter
        -----------
        Exposes current heap size.
    */
    get size() {
        return this.data.length;
    }
}

/*
    mergeKLists
    -----------
    Merges an array of list heads into a single sorted list.

    Parameters:
      - lists: array of ListNode heads (may include nulls; may be empty array)

    Returns:
      - head of merged list, or null if there are no nodes to merge

    Implementation details:
      - Uses NodeMinHeap to always choose the smallest available "front" node.
      - Uses a dummy node to simplify append logic.
*/
function mergeKLists(lists) {
    const heap = new NodeMinHeap();

    // Seed heap with the head of each non-empty list.
    for (const node of lists) {
        if (node) heap.push(node);
    }

    // Dummy node lets us build the result without special-casing the first append.
    const dummy = new ListNode(0);
    let cur = dummy;

    // Repeatedly extract the smallest node and push its successor.
    while (heap.size > 0) {
        const node = heap.pop(); // smallest node by value

        // node should never be null here because heap.size > 0,
        // but we keep logic straightforward.
        cur.next = node;
        cur = cur.next;

        // Push the next node from that list, if any.
        if (node.next) heap.push(node.next);
    }

    // Ensure result terminates cleanly (usually already true).
    cur.next = null;

    return dummy.next;
}

// ===== Helpers =====

/*
    buildList
    ---------
    Builds a linked list from an array of numbers.

    Returns:
      - head node, or null if arr is empty

    Note:
      - Allocates new nodes via 'new ListNode(...)'.
*/
function buildList(arr) {
    let head = null;
    let tail = null;

    for (const v of arr) {
        const node = new ListNode(v);

        if (!head) {
            // First node initializes head and tail.
            head = tail = node;
        } else {
            // Append node and advance tail.
            tail.next = node;
            tail = node;
        }
    }

    return head;
}

/*
    printList
    ---------
    Prints a list as space-separated values preceded by a label.

    If head is null, prints only the label and newline.
*/
function printList(label, head) {
    let s = label;
    let cur = head;

    while (cur) {
        s += cur.val + " ";
        cur = cur.next;
    }

    console.log(s);
}

// ===== Test Cases =====

/*
    Tests cover:
      1) Three non-empty sorted lists
      2) Mix of empty and non-empty lists
      3) All lists empty
      4) Single list
      5) Zero lists (empty input array)

    Note:
      - The merged lists reuse nodes from the original lists.
      - This test harness does not explicitly free memory (JS GC handles it).
*/

console.log("=== Test 1: three sorted lists ===");
const l1 = buildList([1, 4, 7]);
const l2 = buildList([2, 5, 8]);
const l3 = buildList([3, 6, 9]);

printList("List 1: ", l1);
printList("List 2: ", l2);
printList("List 3: ", l3);

let merged1 = mergeKLists([l1, l2, l3]);
printList("Merged: ", merged1);
console.log("(Expected: 1 2 3 4 5 6 7 8 9)\n");

console.log("=== Test 2: mix of empty and non-empty lists ===");
const a = buildList([1, 3, 5]);
const b = null;
const c = buildList([2, 4, 6, 8]);
const d = null;

printList("List A: ", a);
printList("List B: ", b);
printList("List C: ", c);
printList("List D: ", d);

let merged2 = mergeKLists([a, b, c, d]);
printList("Merged: ", merged2);
console.log("(Expected: 1 2 3 4 5 6 8)\n");

console.log("=== Test 3: all empty lists ===");
let merged3 = mergeKLists([null, null, null]);
printList("Merged: ", merged3);
console.log("(Expected: <empty>)\n");

console.log("=== Test 4: single list ===");
const single = buildList([10, 20, 30]);
printList("Input:  ", single);

let merged4 = mergeKLists([single]);
printList("Merged: ", merged4);
console.log("(Expected: 10 20 30)\n");

console.log("=== Test 5: zero lists ===");
let merged5 = mergeKLists([]);
printList("Merged: ", merged5);
console.log("(Expected: <empty>)\n");
