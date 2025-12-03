// Simple max-heap for integers:
class MaxHeap {
    constructor() { this.data = []; }

    _bubbleUp(i) {
        while (i > 0) {
            const p = Math.floor((i - 1) / 2);
            if (this.data[i] <= this.data[p]) break;
            [this.data[i], this.data[p]] = [this.data[p], this.data[i]];
            i = p;
        }
    }

    _heapify(i) {
        const n = this.data.length;
        while (true) {
            let largest = i;
            const left = 2 * i + 1;
            const right = 2 * i + 2;
            if (left < n && this.data[left] > this.data[largest]) largest = left;
            if (right < n && this.data[right] > this.data[largest]) largest = right;
            if (largest === i) break;
            [this.data[i], this.data[largest]] = [this.data[largest], this.data[i]];
            i = largest;
        }
    }

    push(x) {
        this.data.push(x);
        this._bubbleUp(this.data.length - 1);
    }

    peek() {
        return this.data.length ? this.data[0] : null;
    }

    pop() {
        if (!this.data.length) return null;
        if (this.data.length === 1) return this.data.pop();
        const root = this.data[0];
        this.data[0] = this.data.pop();
        this._heapify(0);
        return root;
    }

    size() { return this.data.length; }
}

function kSmallest(arr, k) {
    const heap = new MaxHeap();
    for (const x of arr) {
        if (heap.size() < k) {
            heap.push(x);
        } else if (x < heap.peek()) {
            heap.pop();
            heap.push(x);
        }
    }
    return heap.data.slice(); // k smallest, any order
}

// ===== Safe wrapper to avoid null peek() issues when k <= 0 =====
function kSmallestSafe(arr, k) {
    if (k <= 0 || !arr || arr.length === 0) return [];
    return kSmallest(arr, k);
}

// ===== Helpers =====
function printArray(label, arr) {
    console.log(label + arr.join(" "));
}

// ===== Tests =====

const arr = [7, 2, 9, 4, 1, 5, 8, 3, 6];

// ---- Test 1: k <= n ----
console.log("=== Test 1: k <= n ===");
printArray("Input array: ", arr);

let k = 4;
console.log("k =", k);

let res1 = kSmallestSafe(arr, k);
printArray("kSmallest result (heap order): ", res1);

let sorted1 = [...res1].sort((a, b) => a - b);
printArray("kSmallest result (sorted): ", sorted1);
console.log("(Expected: 1 2 3 4)\n");

// ---- Test 2: k > n ----
console.log("=== Test 2: k > n ===");
k = 20;
console.log("k =", k);

let res2 = kSmallestSafe(arr, k);
printArray("kSmallest result (heap order): ", res2);

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
