#include <vector>
#include <stdexcept>
#include <queue>
#include <stack>
#include <string>
#include <iostream>

// Defines a stack of integers
class IntStack {
public:
    // Add value to the stack
    void push(int value) {
        data_.push_back(value);
    }

    // Remove value from the stack
    int pop() {
        if (data_.empty()) {
            throw std::runtime_error("stack underflow");
        }
        int value = data_.back();
        data_.pop_back();
        return value;
    }

    // Check if the stack is empty
    bool empty() const {
        return data_.empty();
    }

private:
    // The data stored in the stack
    std::vector<int> data_;
};

// Defines a queue of integers
class IntQueue {
public:
    // Add value to the queue
    void enqueue(int value) {
        q_.push(value);
    }

    // Remove value from the queue
    int dequeue() {
        if (q_.empty()) {
            throw std::runtime_error("queue underflow");
        }
        int value = q_.front();
        q_.pop();
        return value;
    }

    // Check if the queue is empty
    bool empty() const {
        return q_.empty();
    }

private:
    // The data stored in the queue
    std::queue<int> q_;
};

// Check if two parentheses are matching
bool isMatching(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

// Check if a string of parentheses is valid
bool isValidParentheses(const std::string& s) {
    std::stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (st.empty()) return false;
            char open = st.top();
            st.pop();
            if (!isMatching(open, c)) return false;
        }
    }
    return st.empty();
}

/**
 * This function implements the Breadth-first search algorithm.
 * It starts from a given node and explores all the connected nodes in a graph.
 * The graph is represented by an adjacency list.
 *
 * @param start: The starting node for the BFS traversal.
 * @param graph: The adjacency list representing the graph.
 */
void bfs(int start, const std::vector<std::vector<int>>& graph) {
    int n = (int)graph.size();
    std::vector<bool> visited(n, false);
    std::queue<int> q;

    // Mark the start node as visited and enqueue it
    visited[start] = true;
    q.push(start);

    // Print the BFS traversal starting from the start node
    std::cout << "BFS starting from " << start << ":\n";

    while (!q.empty()) {
        // Dequeue a node from the queue and print it
        int node = q.front();
        q.pop();
        std::cout << "  visiting " << node << "\n";

        // Look at all possible neighbors of 'node'
        for (int neighbor : graph[node]) {
            // If there's an edge from node to neighbor and neighbor is not visited
            if (!visited[neighbor]) {
                // Mark the neighbor as visited and enqueue it
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

int main() {
    std::cout << "==== TESTING IntStack ====\n";
    IntStack stack;

    std::cout << "Pushing 10, 20, 30...\n";
    stack.push(10);
    stack.push(20);
    stack.push(30);

    std::cout << "Popping values:\n";
    while (!stack.empty()) {
        int v = stack.pop();
        std::cout << "  popped: " << v << "\n";
    }

    std::cout << "\n==== TESTING IntQueue ====\n";
    IntQueue q;

    std::cout << "Enqueuing 1, 2, 3...\n";
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);

    std::cout << "Dequeuing values:\n";
    while (!q.empty()) {
        int v = q.dequeue();
        std::cout << "  dequeued: " << v << "\n";
    }

    std::cout << "\n==== TESTING isValidParentheses ====\n";

    std::vector<std::string> tests = {
        "()",
        "([])",
        "{[()]}",
        "([)]",
        "((())",
        "abc(def[ghi]{jkl})",
        "",
        "{[}]"
    };

    for (const auto& s : tests) {
        std::cout << "Test \"" << s << "\": "
                  << (isValidParentheses(s) ? "valid" : "invalid")
                  << "\n";
    }

    std::cout << "\n==== TESTING bfs ====\n";

    // Graph:
    // 0 -- 1
    // |  /
    // 2
    std::vector<std::vector<int>> graph(3);
    graph[0] = {1, 2};
    graph[1] = {0, 2};
    graph[2] = {0, 1};

    std::cout << "BFS starting from node 0:\n";
    bfs(0, graph);

    std::cout << "\n==== ALL TESTS COMPLETE ====\n";

    return 0;
}
