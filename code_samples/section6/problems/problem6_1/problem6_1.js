class MinHeap {
    constructor() {
        this.data = [];
    }

    insert(x) {
        this.data.push(x);
        this._bubbleUp(this.data.length - 1);
    }

    peek() {
        return this.data.length ? this.data[0] : null;
    }

    pop() {
        if (this.data.length === 0) return null;
        if (this.data.length === 1) return this.data.pop();
        const root = this.data[0];
        this.data[0] = this.data.pop();
        this._heapify(0);
        return root;
    }

    _bubbleUp(i) {
        while (i > 0) {
            const parent = Math.floor((i - 1) / 2);
            if (this.data[i] >= this.data[parent]) break;
            [this.data[i], this.data[parent]] = [this.data[parent], this.data[i]];
            i = parent;
        }
    }

    _heapify(i) {
        const n = this.data.length;
        while (true) {
            let smallest = i;
            const left = 2 * i + 1;
            const right = 2 * i + 2;

            if (left < n && this.data[left] < this.data[smallest]) smallest = left;
            if (right < n && this.data[right] < this.data[smallest]) smallest = right;
            if (smallest === i) break;

            [this.data[i], this.data[smallest]] = [this.data[smallest], this.data[i]];
            i = smallest;
        }
    }
}

// ===== Tests =====

function printArray(arr) {
    console.log(arr.join(" "));
}

console.log("=== Testing MinHeap ===");

const h = new MinHeap();
const values = [5, 3, 8, 1, 7, 2];

process.stdout.write("Inserting values: ");
for (const v of values) {
    process.stdout.write(v + " ");
    h.insert(v);
    console.log(`\n  After insert ${v} -> peek (min): ${h.peek()}`);
}
console.log();

console.log("Extracting in sorted order:");
while (true) {
    const v = h.pop();
    if (v === null) break;
    process.stdout.write(v + " ");
}
console.log("\n");

console.log("Testing peek/pop on empty heap:");
console.log("peek():", h.peek());
console.log("pop():", h.pop());
console.log();

console.log("Reinserting values: 10, 4, 6");
h.insert(10);
h.insert(4);
h.insert(6);

console.log("peek():", h.peek());

process.stdout.write("Final pop sequence: ");
while (true) {
    const v = h.pop();
    if (v === null) break;
    process.stdout.write(v + " ");
}
console.log();
