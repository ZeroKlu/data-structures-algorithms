package code_samples.section4.lesson;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class section4 {
    // Check if two parentheses are matching
    boolean isMatching(char open, char close) {
        return (open == '(' && close == ')') ||
            (open == '[' && close == ']') ||
            (open == '{' && close == '}');
    }

    // Check if a string of parentheses is valid
    boolean isValidParentheses(String s) {
        java.util.Deque<Character> stack = new java.util.ArrayDeque<>();
        for (char c : s.toCharArray()) {
            if (c == '(' || c == '[' || c == '{') {
                stack.push(c);
            } else if (c == ')' || c == ']' || c == '}') {
                if (stack.isEmpty()) return false;
                char open = stack.pop();
                if (!isMatching(open, c)) return false;
            }
        }
        return stack.isEmpty();
    }

    /**
     * This function implements the Breadth-first search algorithm.
     * It starts from a given node and explores all the connected nodes in a graph.
     * The graph is represented by an adjacency list.
     *
     * @param start: The starting node for the BFS traversal.
     * @param graph: The adjacency list representing the graph.
     */
    void bfs(int start, java.util.List<java.util.List<Integer>> graph) {
        int n = graph.size();
        boolean[] visited = new boolean[n];
        java.util.Queue<Integer> queue = new java.util.ArrayDeque<>();

        // Mark the start node as visited and enqueue it
        visited[start] = true;
        queue.add(start);

        // Print the BFS traversal starting from the start node
        System.out.println("BFS starting from " + start + ":");

        while (!queue.isEmpty()) {
            // Dequeue a node from the queue and print it
            int node = queue.poll();
            System.out.println("  visiting " + node);

            // Look at all possible neighbors of 'node'
            for (int neighbor : graph.get(node)) {
                // If there's an edge from node to neighbor and neighbor is not visited
                if (!visited[neighbor]) {
                    // Mark the neighbor as visited and enqueue it
                    visited[neighbor] = true;
                    queue.add(neighbor);
                }
            }
        }
    }

    public static void main(String[] args) {

        System.out.println("==== TESTING IntStack ====");
        IntStack stack = new IntStack();

        System.out.println("Pushing 10, 20, 30...");
        stack.push(10);
        stack.push(20);
        stack.push(30);

        System.out.println("Popping values:");
        while (!stack.isEmpty()) {
            int v = stack.pop();
            System.out.println("  popped: " + v);
        }

        System.out.println("\n==== TESTING IntQueue ====");
        IntQueue q = new IntQueue();

        System.out.println("Enqueuing 1, 2, 3...");
        q.enqueue(1);
        q.enqueue(2);
        q.enqueue(3);

        System.out.println("Dequeuing values:");
        while (!q.isEmpty()) {
            int v = q.dequeue();
            System.out.println("  dequeued: " + v);
        }

        System.out.println("\n==== TESTING isValidParentheses ====");
        section4 helper = new section4();

        List<String> tests = Arrays.asList(
            "()",
            "([])",
            "{[()]}",
            "([)]",
            "((())",
            "abc(def[ghi]{jkl})",
            "",
            "{[}]"
        );

        for (String s : tests) {
            System.out.println(
                "Test \"" + s + "\": "
                + (helper.isValidParentheses(s) ? "valid" : "invalid")
            );
        }

        System.out.println("\n==== TESTING bfs ====");

        // Graph:
        // 0 -- 1
        // |  /
        // 2
        List<List<Integer>> graph = new ArrayList<>();
        graph.add(Arrays.asList(1, 2)); // neighbors of 0
        graph.add(Arrays.asList(0, 2)); // neighbors of 1
        graph.add(Arrays.asList(0, 1)); // neighbors of 2

        System.out.println("BFS starting from node 0:");
        helper.bfs(0, graph);

        System.out.println("\n==== ALL TESTS COMPLETE ====");
    }
}
