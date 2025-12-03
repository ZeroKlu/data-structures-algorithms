class MinHeapK {
    constructor() { this.data = []; }

    _bubbleUp(i) {
        while (i > 0) {
            const p = Math.floor((i - 1) / 2);
            if (this.data[i] >= this.data[p]) break;
            [this.data[i], this.data[p]] = [this.data[p], this.data[i]];
            i = p;
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

    push(x) {
        this.data.push(x);
        this._bubbleUp(this.data.length - 1);
    }

    pop() {
        if (!this.data.length) return null;
        if (this.data.length === 1) return this.data.pop();
        const root = this.data[0];
        this.data[0] = this.data.pop();
        this._heapify(0);
        return root;
    }

    peek() {
        return this.data.length ? this.data[0] : null;
    }

    size() { return this.data.length; }
}

class KthLargest {
    constructor(k, nums) {
        this.k = k;
        this.heap = new MinHeapK();
        for (const x of nums) {
            this.add(x);
        }
    }

    add(val) {
        if (this.heap.size() < this.k) {
            this.heap.push(val);
        } else if (val > this.heap.peek()) {
            this.heap.pop();
            this.heap.push(val);
        }
        return this.heap.peek();
    }
}

// ===== Helpers =====

function printArray(label, arr) {
    console.log(label + arr.join(" "));
}

// ===== Tests =====

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
*/
