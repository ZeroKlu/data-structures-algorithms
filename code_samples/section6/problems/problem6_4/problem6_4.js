class ListNode {
    constructor(val, next = null) {
        this.val = val;
        this.next = next;
    }
}

// Minimal min-heap of ListNode* by node.val
class NodeMinHeap {
    constructor() { this.data = []; }
    _bubbleUp(i) {
        while (i > 0) {
            const p = Math.floor((i - 1) / 2);
            if (this.data[i].val >= this.data[p].val) break;
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
            if (left < n && this.data[left].val < this.data[smallest].val) smallest = left;
            if (right < n && this.data[right].val < this.data[smallest].val) smallest = right;
            if (smallest === i) break;
            [this.data[i], this.data[smallest]] = [this.data[smallest], this.data[i]];
            i = smallest;
        }
    }
    push(node) {
        this.data.push(node);
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
    get size() { return this.data.length; }
}

function mergeKLists(lists) {
    const heap = new NodeMinHeap();
    for (const node of lists) {
        if (node) heap.push(node);
    }
    const dummy = new ListNode(0);
    let cur = dummy;
    while (heap.size > 0) {
        const node = heap.pop();
        cur.next = node;
        cur = cur.next;
        if (node.next) heap.push(node.next);
    }
    return dummy.next;
}

// ===== Helpers =====

function buildList(arr) {
    let head = null;
    let tail = null;
    for (const v of arr) {
        const node = new ListNode(v);
        if (!head) {
            head = tail = node;
        } else {
            tail.next = node;
            tail = node;
        }
    }
    return head;
}

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
