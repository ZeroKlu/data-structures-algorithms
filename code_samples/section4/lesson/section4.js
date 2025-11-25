class IntStack {
    constructor() {
        this.data = [];
    }

    push(value) {
        this.data.push(value);
    }

    pop() {
        if (this.data.length === 0) {
            throw new Error("stack underflow");
        }
        return this.data.pop();
    }

    isEmpty() {
        return this.data.length === 0;
    }
}

class IntQueue {
    constructor() {
        this.data = [];
        this.head = 0; // for O(1) dequeue
    }

    enqueue(value) {
        this.data.push(value);
    }

    dequeue() {
        if (this.head >= this.data.length) {
            throw new Error("queue underflow");
        }
        const value = this.data[this.head];
        this.head += 1;
        return value;
    }

    isEmpty() {
        return this.head >= this.data.length;
    }
}

function isMatching(open, close) {
    return (open === "(" && close === ")") ||
           (open === "[" && close === "]") ||
           (open === "{" && close === "}");
}

function isValidParentheses(s) {
    const stack = [];
    for (const c of s) {
        if (c === "(" || c === "[" || c === "{") {
            stack.push(c);
        } else if (c === ")" || c === "]" || c === "}") {
            if (stack.length === 0) return false;
            const open = stack.pop();
            if (!isMatching(open, c)) return false;
        }
    }
    return stack.length === 0;
}

function bfs(start, graph) {
    const n = graph.length;
    const visited = new Array(n).fill(false);
    const queue = [];

    visited[start] = true;
    queue.push(start);

    let head = 0;
    while (head < queue.length) {
        const node = queue[head++];
        console.log(node);

        for (const neighbor of graph[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                queue.push(neighbor);
            }
        }
    }
}

// ===== TESTING IntStack =====
console.log("==== TESTING IntStack ====");
const stack = new IntStack();

console.log("Pushing 10, 20, 30...");
stack.push(10);
stack.push(20);
stack.push(30);

console.log("Popping values:");
while (!stack.isEmpty()) {
    const v = stack.pop();
    console.log("  popped:", v);
}

// ===== TESTING IntQueue =====
console.log("\n==== TESTING IntQueue ====");
const q = new IntQueue();

console.log("Enqueuing 1, 2, 3...");
q.enqueue(1);
q.enqueue(2);
q.enqueue(3);

console.log("Dequeuing values:");
while (!q.isEmpty()) {
    const v = q.dequeue();
    console.log("  dequeued:", v);
}

// ===== TESTING isValidParentheses =====
console.log("\n==== TESTING isValidParentheses ====");

const tests = [
    "()",
    "([])",
    "{[()]}",
    "([)]",
    "((())",
    "abc(def[ghi]{jkl})",
    "",
    "{[}]"
];

for (const s of tests) {
    console.log(
        `Test "${s}":`,
        isValidParentheses(s) ? "valid" : "invalid"
    );
}

// ===== TESTING bfs =====
console.log("\n==== TESTING bfs ====");

// Graph:
// 0 -- 1
// |  /
// 2
const graph = [
    [1, 2], // neighbors of 0
    [0, 2], // neighbors of 1
    [0, 1]  // neighbors of 2
];

console.log("BFS starting from node 0:");
bfs(0, graph);

console.log("\n==== ALL TESTS COMPLETE ====");
