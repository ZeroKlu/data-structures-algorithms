// ============================================================
// MIN-HEAP OVERVIEW
// ============================================================
//
// We store a binary heap in an array using standard indexing:
//
//   parent(i) = Math.floor((i - 1) / 2)
//   left(i)   = 2*i + 1
//   right(i)  = 2*i + 2
//
// Min-heap property:
//   every parent <= its children
//
// This file includes:
// 1) MinHeap (insert only)                 -> demonstrates heapify-up
// 2) MinHeapWithExtract (insert + extract) -> adds heapify-down on extract
// 3) Array helpers for build-heap + heapsort
// 4) Tests (same style as your other languages)

class MinHeap {
    constructor() {
        // Underlying storage: heap elements in level-order
        this.data = [];
    }

    // Insert a value into the min-heap.
    //
    // Steps:
    // 1) Push to the end of the array
    // 2) Heapify up (bubble up): while parent > child, swap upward
    insert(value) {
        // 1) Add new value at the end (next free spot)
        this.data.push(value);

        // i is index of newly inserted element
        let i = this.data.length - 1;

        // 2) Heapify up (bubble up)
        while (i > 0) {
            const parent = Math.floor((i - 1) / 2);

            // If heap property holds, stop
            if (this.data[parent] <= this.data[i]) break;

            // Otherwise swap child with parent
            [this.data[parent], this.data[i]] = [this.data[i], this.data[parent]];

            // Continue at parent's index
            i = parent;
        }
    }
}

class MinHeapWithExtract {
    constructor() {
        // Underlying storage: heap elements in level-order
        this.data = [];
    }

    // Insert a value into the heap (same heapify-up logic as MinHeap).
    insert(value) {
        this.data.push(value);
        let i = this.data.length - 1;

        // Heapify up
        while (i > 0) {
            const parent = Math.floor((i - 1) / 2);
            if (this.data[parent] <= this.data[i]) break;

            [this.data[parent], this.data[i]] = [this.data[i], this.data[parent]];
            i = parent;
        }
    }

    // Remove and return the minimum element (root).
    //
    // Steps:
    // 1) Save the root value
    // 2) Move last element to the root
    // 3) Pop the last element (shrinks heap)
    // 4) Heapify down (bubble down) to restore heap property
    extractMin() {
        if (this.data.length === 0) {
            throw new Error("heap underflow");
        }

        // 1) Minimum is at root
        const minVal = this.data[0];

        // 2+3) Remove last element; if that was the only element, we're done
        const last = this.data.pop();
        if (this.data.length === 0) {
            return minVal;
        }

        // Put last element at root, then restore heap property
        this.data[0] = last;

        // 4) Heapify down
        const n = this.data.length;
        let i = 0;

        while (true) {
            const left = 2 * i + 1;
            const right = 2 * i + 2;
            let smallest = i;

            // Pick the smallest among node and children
            if (left < n && this.data[left] < this.data[smallest]) {
                smallest = left;
            }
            if (right < n && this.data[right] < this.data[smallest]) {
                smallest = right;
            }

            // If node is already smallest, heap property holds
            if (smallest === i) break;

            // Otherwise swap down and continue
            [this.data[i], this.data[smallest]] = [this.data[smallest], this.data[i]];
            i = smallest;
        }

        return minVal;
    }
}

// ============================================================
// ARRAY-BASED HEAP HELPERS (build heap + heapsort)
// ============================================================

// Heapify-down on a plain array a, treating a[0..n-1] as the heap.
// i is the index to heapify down from.
function heapifyDownArray(a, n, i) {
    while (true) {
        const left = 2 * i + 1;
        const right = 2 * i + 2;
        let smallest = i;

        // Find smallest among node and children
        if (left < n && a[left] < a[smallest]) smallest = left;
        if (right < n && a[right] < a[smallest]) smallest = right;

        // If already smallest, stop
        if (smallest === i) break;

        // Swap with smallest child and continue
        [a[i], a[smallest]] = [a[smallest], a[i]];
        i = smallest;
    }
}

// Build a min-heap in-place using bottom-up construction (O(n)):
// start from the last parent (Math.floor(n/2) - 1) down to root.
function buildMinHeap(a) {
    const n = a.length;
    for (let i = Math.floor(n / 2) - 1; i >= 0; i--) {
        heapifyDownArray(a, n, i);
    }
}

// Heapsort using a MIN-heap produces DESCENDING order:
// each pass moves the minimum (root) to the end.
function heapsortDescMinHeap(a) {
    buildMinHeap(a);

    // end is the last index of the active heap region [0..end]
    for (let end = a.length - 1; end > 0; end--) {
        // Move min to its final position at end
        [a[0], a[end]] = [a[end], a[0]];

        // Restore heap on reduced region [0..end-1]
        heapifyDownArray(a, end, 0);
    }
}

// ============================================================
// TESTS
// ============================================================

function printArray(a) {
    console.log(a.join(" "));
}

// Sample values
const valuesToInsert = [5, 3, 8, 1, 7, 2];

// --- Test MinHeap.insert (no extract) ---
console.log("=== Testing MinHeap.insert ===");
const h1 = new MinHeap();

// This test just exercises insert + heapify-up (no extract API here)
process.stdout.write("Inserting into MinHeap (no extract available): ");
for (const v of valuesToInsert) {
    process.stdout.write(v + " ");
    h1.insert(v);
}
console.log("\n");

// --- Test MinHeapWithExtract.insert / extractMin ---
console.log("=== Testing MinHeapWithExtract insert / extractMin ===");
const h2 = new MinHeapWithExtract();

process.stdout.write("Inserting: ");
for (const v of valuesToInsert) {
    process.stdout.write(v + " ");
    h2.insert(v);
}
console.log();

// Extract until empty; should print ascending order: 1 2 3 5 7 8
process.stdout.write("Extracting in sorted order: ");
while (true) {
    try {
        const minVal = h2.extractMin();
        process.stdout.write(minVal + " ");
    } catch (e) {
        if (e.message === "heap underflow") {
            break; // done
        }
        throw e; // unexpected error
    }
}
console.log("\n");

// --- Test buildMinHeap ---
console.log("=== Testing buildMinHeap ===");
let arr1 = [9, 4, 7, 1, 0, 3, 2];

process.stdout.write("Original array: ");
printArray(arr1);

buildMinHeap(arr1);
process.stdout.write("After buildMinHeap (min-heap array): ");
printArray(arr1);
console.log();

// --- Test heapsortDescMinHeap ---
console.log("=== Testing heapsortDescMinHeap (descending sort) ===");
let arr2 = [12, 3, 17, 8, 34, 25, 1];

process.stdout.write("Original array: ");
printArray(arr2);

heapsortDescMinHeap(arr2);
process.stdout.write("After heapsortDescMinHeap (descending): ");
printArray(arr2);
