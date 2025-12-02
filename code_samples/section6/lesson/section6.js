class MinHeap {
    constructor() {
        this.data = [];
    }

    insert(value) {
        this.data.push(value);
        let i = this.data.length - 1;
        // heapify up
        while (i > 0) {
            const parent = Math.floor((i - 1) / 2);
            if (this.data[parent] <= this.data[i]) break;
            [this.data[parent], this.data[i]] = [this.data[i], this.data[parent]];
            i = parent;
        }
    }
}

class MinHeapWithExtract {
    constructor() {
        this.data = [];
    }

    insert(value) {
        this.data.push(value);
        let i = this.data.length - 1;
        while (i > 0) {
            const parent = Math.floor((i - 1) / 2);
            if (this.data[parent] <= this.data[i]) break;
            [this.data[parent], this.data[i]] = [this.data[i], this.data[parent]];
            i = parent;
        }
    }

    extractMin() {
        if (this.data.length === 0) {
            throw new Error("heap underflow");
        }
        const minVal = this.data[0];
        const last = this.data.pop();
        if (this.data.length === 0) {
            return minVal;
        }
        this.data[0] = last;

        const n = this.data.length;
        let i = 0;
        while (true) {
            const left = 2 * i + 1;
            const right = 2 * i + 2;
            let smallest = i;
            if (left < n && this.data[left] < this.data[smallest]) {
                smallest = left;
            }
            if (right < n && this.data[right] < this.data[smallest]) {
                smallest = right;
            }
            if (smallest === i) break;
            [this.data[i], this.data[smallest]] = [this.data[smallest], this.data[i]];
            i = smallest;
        }
        return minVal;
    }
}

function heapifyDownArray(a, n, i) {
    while (true) {
        const left = 2 * i + 1;
        const right = 2 * i + 2;
        let smallest = i;
        if (left < n && a[left] < a[smallest]) smallest = left;
        if (right < n && a[right] < a[smallest]) smallest = right;
        if (smallest === i) break;
        [a[i], a[smallest]] = [a[smallest], a[i]];
        i = smallest;
    }
}

function buildMinHeap(a) {
    const n = a.length;
    for (let i = Math.floor(n / 2) - 1; i >= 0; i--) {
        heapifyDownArray(a, n, i);
    }
}

function heapsortDescMinHeap(a) {
    buildMinHeap(a);
    for (let end = a.length - 1; end > 0; end--) {
        [a[0], a[end]] = [a[end], a[0]];
        heapifyDownArray(a, end, 0);
    }
}

// ----- Tests -----
function printArray(a) {
    console.log(a.join(" "));
}

// Sample values to reuse
const valuesToInsert = [5, 3, 8, 1, 7, 2];

// --- Test MinHeap.insert (no extract) ---
console.log("=== Testing MinHeap.insert ===");
const h1 = new MinHeap();

process.stdout.write("Inserting into MinHeap (no extract available): ");
for (const v of valuesToInsert) {
    process.stdout.write(v + " ");
    h1.insert(v); // just exercising insert
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

process.stdout.write("Extracting in sorted order: ");
while (true) {
    try {
        const minVal = h2.extractMin();
        process.stdout.write(minVal + " ");
    } catch (e) {
        if (e.message === "heap underflow") {
            break; // done
        } else {
            throw e; // unexpected error
        }
    }
}
console.log("\n");

// --- Test buildMinHeap (uses heapifyDownArray) ---
console.log("=== Testing buildMinHeap ===");
let arr1 = [9, 4, 7, 1, 0, 3, 2];

process.stdout.write("Original array: ");
printArray(arr1);

buildMinHeap(arr1);
process.stdout.write("After buildMinHeap (min-heap array): ");
printArray(arr1);
console.log();

// --- Test heapsortDescMinHeap (also uses buildMinHeap & heapifyDownArray) ---
console.log("=== Testing heapsortDescMinHeap (descending sort) ===");
let arr2 = [12, 3, 17, 8, 34, 25, 1];

process.stdout.write("Original array: ");
printArray(arr2);

heapsortDescMinHeap(arr2);
process.stdout.write("After heapsortDescMinHeap (descending): ");
printArray(arr2);
