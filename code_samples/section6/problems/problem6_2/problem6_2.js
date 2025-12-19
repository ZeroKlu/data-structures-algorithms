/*
    This file demonstrates building a MIN-HEAP in-place from an array, then
    validating correctness by repeatedly extracting the minimum from a COPY.

    Min-heap invariant (0-based indexing):
        For every i > 0:
            arr[parent(i)] <= arr[i]

    Index formulas:
        parent(i) = Math.floor((i - 1) / 2)
        left(i)   = 2*i + 1
        right(i)  = 2*i + 2

    Main pieces:
      - heapify(arr, n, i): "sift down" (recursive) to fix subtree rooted at i
      - buildHeap(arr): bottom-up heap construction (O(n))
      - popMin(arr, sizeObj): test helper that pops min from heap prefix [0..size-1]
      - tests: print original array, build heap, extract sorted order

    Notes:
      - heapify is recursive. For very large heaps, an iterative version avoids deep recursion.
      - popMin returns null when empty (clearer than sentinel values).
*/

/*
    heapify (recursive sift-down)
    -----------------------------
    Ensures that the subtree rooted at index i satisfies the min-heap property,
    assuming the subtrees rooted at its children already satisfy the property.

    Parameters:
      arr : array storing heap elements
      n   : current heap size (only indices [0..n-1] are considered part of heap)
      i   : root index of the subtree to fix

    Algorithm:
      1) Find the smallest among i, left child, right child (if children exist).
      2) If a child is smaller than arr[i], swap with the smallest child.
      3) Recurse on the child position we swapped into (because that subtree may now be invalid).

    Complexity:
      O(log n) worst case (height of heap).
*/
function heapify(arr, n, i) {
    let smallest = i;
    const left = 2 * i + 1;
    const right = 2 * i + 2;

    // If left child exists and is smaller than current smallest, update candidate.
    if (left < n && arr[left] < arr[smallest]) smallest = left;

    // If right child exists and is smaller than current smallest, update candidate.
    if (right < n && arr[right] < arr[smallest]) smallest = right;

    // If the smallest isn't the current node, swap and continue heapifying downward.
    if (smallest !== i) {
        [arr[i], arr[smallest]] = [arr[smallest], arr[i]];
        heapify(arr, n, smallest);
    }
}

/*
    buildHeap
    ---------
    Converts an arbitrary array into a min-heap IN PLACE using bottom-up construction.

    Key insight:
      - Leaves (indices >= floor(n/2)) already satisfy the heap property (no children).
      - The last internal node is floor(n/2) - 1.
      - Heapify each internal node from that index down to 0.

    Complexity:
      O(n) overall.
*/
function buildHeap(arr) {
    const n = arr.length;

    // Start from the last internal node and sift down each node toward the root.
    for (let i = Math.floor(n / 2) - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

// ===== Tests =====

/*
    printArray
    ----------
    Utility function to print a label + array contents.
    (When printing a heap, remember it's "heap order", not sorted order.)
*/
function printArray(label, arr) {
    console.log(label + arr.join(" "));
}

/*
    popMin
    ------
    Test helper: pops the minimum element from an array-based min-heap that
    lives in the prefix arr[0..size-1].

    Why sizeObj:
      - JavaScript numbers are passed by value, so we wrap "size" in an object
        to emulate pass-by-reference and allow popMin to decrement it.

    Parameters:
      arr     : array containing heap elements
      sizeObj : object like { size: <current heap size> }

    Returns:
      - null when heap is empty
      - otherwise the removed minimum value

    Steps:
      1) Save root (arr[0]).
      2) Decrease size.
      3) If still non-empty:
           - move last heap element into root
           - heapify down using the NEW size as n
*/
function popMin(arr, sizeObj) {
    let size = sizeObj.size;
    if (size === 0) return null;

    // Root holds the minimum in a min-heap.
    const root = arr[0];

    // Shrink the heap by one element.
    size--;
    sizeObj.size = size;

    if (size > 0) {
        // Move the last element in the heap prefix to the root position.
        // Then restore heap property within [0..size-1].
        arr[0] = arr[size];
        heapify(arr, size, 0);
    }

    return root;
}

// ===== Test run =====

/*
    1) Start with an unsorted array.
    2) buildHeap => transforms it to a min-heap in-place.
    3) Extract repeatedly from a copy => should yield ascending order.

    Expected sorted output for [9, 4, 7, 1, 0, 3, 2]:
        0 1 2 3 4 7 9
*/
let arr = [9, 4, 7, 1, 0, 3, 2];

printArray("Original array: ", arr);

buildHeap(arr);
printArray("After buildHeap (min-heap array): ", arr);

// Extract values to test correctness
console.log("Extracting elements in sorted order:");

// Clone heap since extraction mutates the heap structure.
let heapCopy = arr.slice();

// Track logical heap size separately as it shrinks on each pop.
let sizeObj = { size: heapCopy.length };

let out = [];
while (sizeObj.size > 0) {
    // popMin returns null only when empty; loop condition prevents null entries.
    out.push(popMin(heapCopy, sizeObj));
}

// out should be in ascending order if heap is valid.
printArray("", out);
