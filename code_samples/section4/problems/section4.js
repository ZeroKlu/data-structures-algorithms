function isValid(s) {
    const stack = [];
    const match = {
        ')': '(',
        ']': '[',
        '}': '{'
    };
    for (const ch of s) {
        if (ch === '(' || ch === '[' || ch === '{') {
            stack.push(ch);
        } else {
            if (stack.length === 0) return false;
            const top = stack.pop();
            if (top !== match[ch]) return false;
        }
    }
    return stack.length === 0;
}

class MyQueue {
    constructor() {
        this._in = [];
        this._out = [];
    }

    _moveInToOut() {
        if (this._out.length === 0) {
            while (this._in.length > 0) {
                this._out.push(this._in.pop());
            }
        }
    }

    enqueue(x) {
        this._in.push(x);
    }

    dequeue() {
        this._moveInToOut();
        return this._out.pop();
    }

    front() {
        this._moveInToOut();
        return this._out[this._out.length - 1];
    }

    empty() {
        return this._in.length === 0 && this._out.length === 0;
    }
}

function nextGreaterElements(nums) {
    const n = nums.length;
    const ans = new Array(n).fill(-1);
    const stack = []; // values

    for (let i = n - 1; i >= 0; i--) {
        const x = nums[i];
        while (stack.length > 0 && stack[stack.length - 1] <= x) {
            stack.pop();
        }
        ans[i] = stack.length > 0 ? stack[stack.length - 1] : -1;
        stack.push(x);
    }
    return ans;
}

function dailyTemperatures(temps) {
    const n = temps.length;
    const ans = new Array(n).fill(0);
    const stack = []; // indices

    for (let i = 0; i < n; i++) {
        while (stack.length > 0 && temps[i] > temps[stack[stack.length - 1]]) {
            const idx = stack.pop();
            ans[idx] = i - idx;
        }
        stack.push(i);
    }
    return ans;
}

function levelOrder(root) {
    const result = [];
    if (!root) return result;

    const q = [];
    q.push(root);

    while (q.length > 0) {
        const size = q.length;
        const level = [];
        for (let i = 0; i < size; i++) {
            const node = q.shift();
            level.push(node.val);
            if (node.left) q.push(node.left);
            if (node.right) q.push(node.right);
        }
        result.push(level);
    }
    return result;
}

function maxSlidingWindow(nums, k) {
    const n = nums.length;
    if (n === 0 || k === 0) return [];
    const deque = []; // indices
    const result = [];

    for (let i = 0; i < n; i++) {
        while (deque.length > 0 && deque[0] <= i - k) {
            deque.shift();
        }
        while (deque.length > 0 && nums[deque[deque.length - 1]] <= nums[i]) {
            deque.pop();
        }
        deque.push(i);
        if (i >= k - 1) {
            result.push(nums[deque[0]]);
        }
    }
    return result;
}

// Helper TreeNode class for levelOrder tests
class TreeNode {
    constructor(val, left = null, right = null) {
        this.val = val;
        this.left = left;
        this.right = right;
    }
}

// ==== TEST isValid ====
console.log("==== TEST isValid ====");
const parenTests = [
    "()",
    "([])",
    "{[()]}",
    "([)]",
    "((())",
    "abc(def[ghi]{jkl})",
    "",
    "{[}]"
];

for (const s of parenTests) {
    console.log(`Test "${s}":`, isValid(s) ? "valid" : "invalid");
}

// ==== TEST MyQueue ====
console.log("\n==== TEST MyQueue ====");
const q = new MyQueue();
console.log("Enqueue: 1, 2, 3");
q.enqueue(1);
q.enqueue(2);
q.enqueue(3);

console.log("Front:", q.front());
console.log("Dequeue:", q.dequeue());
console.log("Dequeue:", q.dequeue());
console.log("Empty?", q.empty() ? "true" : "false");
console.log("Dequeue:", q.dequeue());
console.log("Empty?", q.empty() ? "true" : "false");

// ==== TEST nextGreaterElements ====
console.log("\n==== TEST nextGreaterElements ====");
const nums = [2, 1, 2, 4, 3];
const nge = nextGreaterElements(nums);
console.log("nums:", nums);
console.log("NGE :", nge);

// ==== TEST dailyTemperatures ====
console.log("\n==== TEST dailyTemperatures ====");
const temps = [73, 74, 75, 71, 69, 72, 76, 73];
const days = dailyTemperatures(temps);
console.log("temps:", temps);
console.log("wait :", days);

// ==== TEST levelOrder ====
console.log("\n==== TEST levelOrder ====");

// Build this tree:
//
//        1
//       / \
//      2   3
//     /   / \
//    4   5   6
//
const node4 = new TreeNode(4);
const node5 = new TreeNode(5);
const node6 = new TreeNode(6);
const node2 = new TreeNode(2, node4, null);
const node3 = new TreeNode(3, node5, node6);
const root  = new TreeNode(1, node2, node3);

const levels = levelOrder(root);
levels.forEach((level, i) => {
    console.log(`level ${i}:`, level);
});

// ==== TEST maxSlidingWindow ====
console.log("\n==== TEST maxSlidingWindow ====");
const arr = [1, 3, -1, -3, 5, 3, 6, 7];
const k = 3;
const msw = maxSlidingWindow(arr, k);
console.log("nums:", arr);
console.log(`max window size ${k}:`, msw);

console.log("\n==== ALL TESTS COMPLETE ====");
