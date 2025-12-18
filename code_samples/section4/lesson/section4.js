// Defines a stack of integers
class IntStack {
    // Constructor
    constructor() {
        // The data stored in the stack
        this.data = [];
    }

    // Pushes a value onto the stack
    push(value) {
        this.data.push(value);
    }

    // Pops a value off the stack
    pop() {
        if (this.data.length === 0) {
            throw new Error("stack underflow");
        }
        return this.data.pop();
    }

    // Checks if the stack is empty
    isEmpty() {
        return this.data.length === 0;
    }
}

// Defines a queue of integers
class IntQueue {
    // Constructor
    constructor() {
        // The data stored in the queue
        this.data = [];
        // The head of the queue
        this.head = 0; // for O(1) dequeue
    }

    // Adds a value to the queue
    enqueue(value) {
        this.data.push(value);
    }

    // Removes a value from the queue
    dequeue() {
        if (this.head >= this.data.length) {
            throw new Error("queue underflow");
        }
        const value = this.data[this.head];
        this.head += 1;
        return value;
    }

    // Checks if the queue is empty
    isEmpty() {
        return this.head >= this.data.length;
    }
}

// Check if two parentheses are matching
function isMatching(open, close) {
    return (open === "(" && close === ")") ||
           (open === "[" && close === "]") ||
           (open === "{" && close === "}");
}

// Check if a string of parentheses is valid
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

/**
 * This function implements the Breadth-first search algorithm.
 * It starts from a given node and explores all the connected nodes in a graph.
 * The graph is represented by an adjacency list.
 *
 * @param start: The starting node for the BFS traversal.
 * @param graph: The adjacency list representing the graph.
 */
function bfs(start, graph) {
    const n = graph.length;
    const visited = new Array(n).fill(false);
    const queue = [];

    // Mark the start node as visited and enqueue it
    visited[start] = true;
    queue.push(start);

    // Print the BFS traversal starting from the start node
    console.log("BFS starting from " + start + ":");

    let head = 0;
    while (head < queue.length) {
        // Dequeue a node from the queue and print it
        const node = queue[head++];
        console.log("  visiting " + node);

        // Look at all possible neighbors of 'node'
        for (const neighbor of graph[node]) {
            // If there's an edge from node to neighbor and neighbor is not visited
            if (!visited[neighbor]) {
                // Mark the neighbor as visited and enqueue it
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
